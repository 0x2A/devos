// ------------------------------------------------------------------------------------------------
// acpi/acpi.h
// ------------------------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

#define MAX_CPU_COUNT 16


extern uint32_t g_acpiCpuCount;
extern uint8_t g_acpiCpuIds[MAX_CPU_COUNT];


void AcpiInit();
uint32_t AcpiRemapIrq(uint32_t irq);