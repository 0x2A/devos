#include "acpi.h"
#include <system.h>
#include <terminal.h>
#include <ioapic.h>
#include <local_apic.h>
#include <string.h>
#include <stdbool.h>


// ------------------------------------------------------------------------------------------------
// Globals
uint32_t g_acpiCpuCount;
uint8_t g_acpiCpuIds[MAX_CPU_COUNT];


// ------------------------------------------------------------------------------------------------
typedef struct AcpiHeader
{
	uint32_t signature;
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	uint8_t oem[6];
	uint8_t oemTableId[8];
	uint32_t oemRevision;
	uint32_t creatorId;
	uint32_t creatorRevision;
} PACKED AcpiHeader;

typedef struct AcpiFadt
{
	AcpiHeader header;
	uint32_t firmwareControl;
	uint32_t dsdt;
	uint8_t reserved;
	uint8_t preferredPMProfile;
	uint16_t sciInterrupt;
	uint32_t smiCommandPort;
	uint8_t acpiEnable;
	uint8_t acpiDisable;
	// TODO - fill in rest of data
} PACKED AcpiFadt;

// ------------------------------------------------------------------------------------------------
typedef struct AcpiMadt
{
	AcpiHeader header;
	uint32_t localApicAddr;
	uint32_t flags;
} PACKED AcpiMadt;

// ------------------------------------------------------------------------------------------------
typedef struct ApicHeader
{
	uint8_t type;
	uint8_t length;
} PACKED ApicHeader;

// APIC structure types
#define APIC_TYPE_LOCAL_APIC            0
#define APIC_TYPE_IO_APIC               1
#define APIC_TYPE_INTERRUPT_OVERRIDE    2

// ------------------------------------------------------------------------------------------------
typedef struct ApicLocalApic
{
	ApicHeader header;
	uint8_t acpiProcessorId;
	uint8_t apicId;
	uint32_t flags;
} PACKED ApicLocalApic;

// ------------------------------------------------------------------------------------------------
typedef struct ApicIoApic
{
	ApicHeader header;
	uint8_t ioApicId;
	uint8_t reserved;
	uint32_t ioApicAddress;
	uint32_t globalSystemInterruptBase;
} PACKED ApicIoApic;

// ------------------------------------------------------------------------------------------------
typedef struct ApicInterruptOverride
{
	ApicHeader header;
	uint8_t bus;
	uint8_t source;
	uint32_t interrupt;
	uint16_t flags;
} PACKED ApicInterruptOverride;

// ------------------------------------------------------------------------------------------------
static AcpiMadt *s_madt;

// ------------------------------------------------------------------------------------------------
static void AcpiParseFacp(AcpiFadt *facp)
{
	if (facp->smiCommandPort)
	{
		//ConsolePrint("Enabling ACPI\n");
		//IoWrite8(facp->smiCommandPort, facp->acpiEnable);

		// TODO - wait for SCI_EN bit
	}
	else
	{
		puts("ACPI already enabled\n");
	}
}

// ------------------------------------------------------------------------------------------------
static void AcpiParseApic(AcpiMadt *madt)
{
	s_madt = madt;

	DbgPrintf("Local APIC Address = 0x%x\n", madt->localApicAddr);
	g_localApicAddr = (uint8_t *)(uintptr_t)madt->localApicAddr;

	uint8_t *p = (uint8_t *)(madt + 1);
	uint8_t *end = (uint8_t *)madt + madt->header.length;

	while (p < end)
	{
		ApicHeader *header = (ApicHeader *)p;
		uint8_t type = header->type;
		uint8_t length = header->length;

		if (type == APIC_TYPE_LOCAL_APIC)
		{
			ApicLocalApic *s = (ApicLocalApic *)p;

			DbgPrintf("Found CPU: %d %d %x\n", s->acpiProcessorId, s->apicId, s->flags);
			if (g_acpiCpuCount < MAX_CPU_COUNT)
			{
				g_acpiCpuIds[g_acpiCpuCount] = s->apicId;
				++g_acpiCpuCount;
			}
		}
		else if (type == APIC_TYPE_IO_APIC)
		{
			ApicIoApic *s = (ApicIoApic *)p;

			DbgPrintf("Found I/O APIC: %d 0x%x %d\n", s->ioApicId, s->ioApicAddress, s->globalSystemInterruptBase);
			g_ioApicAddr = (uint8_t *)(uintptr_t)s->ioApicAddress;
		}
		else if (type == APIC_TYPE_INTERRUPT_OVERRIDE)
		{
			ApicInterruptOverride *s = (ApicInterruptOverride *)p;

			DbgPrintf("Found Interrupt Override: %d %d %d 0x%x\n", s->bus, s->source, s->interrupt, s->flags);
		}
		else
		{
			DbgPrintf("Unknown APIC structure %d\n", type);
		}

		p += length;
	}
}

static void AcpiParseDT(AcpiHeader *header)
{
	uint32_t signature = header->signature;

	char sigStr[5];
	memcpy(sigStr, &signature, 4);
	sigStr[4] = 0;

	DbgPrintf("%s %x\n", sigStr, signature);

	if (signature == 0x50434146)
	{
		puts("sig 1");
		AcpiParseFacp((AcpiFadt *)header);
	}
	else if (signature == 0x43495041)
	{
		puts("sig 2");
		AcpiParseApic((AcpiMadt *)header);
	}
}

static void AcpiParseRsdt(AcpiHeader *rsdt)
{
	uint32_t *p = (uint32_t *)(rsdt + 1);
	uint32_t *end = (uint32_t *)((uint8_t*)rsdt + rsdt->length);

	while (p < end)
	{
		uint32_t address = *p++;
		AcpiParseDT((AcpiHeader *)(uintptr_t)address);
	}
}

static void AcpiParseXsdt(AcpiHeader *xsdt)
{
	uint64_t *p = (uint64_t *)(xsdt + 1);
	uint64_t *end = (uint64_t *)((uint8_t*)xsdt + xsdt->length);


	DbgPrintf("xsdt length: %x\n", xsdt->length);
	while (p < end)
	{
		uint64_t address = *p;
		p++;

		DbgPrintf("curr: %x, end: %x\n", address, end);
		AcpiParseDT((AcpiHeader *)(uintptr_t)address);
	}

	puts("fin\n");
}

static bool AcpiParseRsdp(uint8_t *p)
{
	// Parse Root System Description Pointer
	puts("RSDP found\n");

	// Verify checksum
	uint8_t sum = 0;
	for (uint32_t i = 0; i < 20; ++i)
	{
		sum += p[i];
	}

	if (sum)
	{
		puts("Checksum failed\n");
		return false;
	}

	// Print OEM
	char oem[7];
	memcpy(oem, p + 9, 6);
	oem[6] = '\0';
	DbgPrintf("OEM = %s\n", oem);

	// Check version
	uint8_t revision = p[15];
	if (revision == 0)
	{
		puts("Version 1\n");

		uint32_t rsdtAddr = *(uint32_t *)(p + 16);
		AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr);
	}
	else if (revision == 2)
	{
		puts("Version 2\n");

		uint32_t rsdtAddr = *(uint32_t *)(p + 16);
		uint64_t xsdtAddr = *(uint64_t *)(p + 24);

		//if (xsdtAddr)
		{
			//puts("parse xsdt");
			//AcpiParseXsdt((AcpiHeader *)(uintptr_t)xsdtAddr);
		}
		//else
		{
			//puts("parse rsdt");
			AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr);
		}
	}
	else
	{
		DbgPrintf("Unsupported ACPI version %d\n", revision);
	}

	return true;
}

void AcpiInit()
{
	// TODO - Search Extended BIOS Area

	// Search main BIOS area below 1MB
	uint8_t *p = (uint8_t *)0x000e0000;
	uint8_t *end = (uint8_t *)0x000fffff;

	while (p < end)
	{
		uint64_t signature = *(uint64_t *)p;

		if (signature == 0x2052545020445352) // 'RSD PTR '
		{
			if (AcpiParseRsdp(p))
			{
				break;
			}
		}

		p += 16;
	}
}

uint32_t AcpiRemapIrq(uint32_t irq)
{
	AcpiMadt *madt = s_madt;

	uint8_t *p = (uint8_t *)(madt + 1);
	uint8_t *end = (uint8_t *)madt + madt->header.length;

	while (p < end)
	{
		ApicHeader *header = (ApicHeader *)p;
		uint8_t type = header->type;
		uint8_t length = header->length;

		if (type == APIC_TYPE_INTERRUPT_OVERRIDE)
		{
			ApicInterruptOverride *s = (ApicInterruptOverride *)p;

			if (s->source == irq)
			{
				return s->interrupt;
			}
		}

		p += length;
	}

	return irq;
}