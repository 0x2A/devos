// local_apic.h

#pragma once

#include <stdint.h>

extern uint8_t *g_localApicAddr;

void LocalApicInit();

uint32_t LocalApicGetId();
void LocalApicSendInit(uint32_t apic_id);
void LocalApicSendStartup(uint32_t apic_id, uint32_t vector);