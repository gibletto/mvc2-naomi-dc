/*
 * arm7.h  The AICA's sound CPU: an ARM7 (ARM instructions, no Thumb), interpreted
 */

#ifndef ARM7_H
#define ARM7_H

#include <stdint.h>

/* its memory, from aica.c: the sound RAM at 0, the AICA's registers at 0x800000 */
uint32_t aica_arm_read32(uint32_t addr);
uint32_t aica_arm_read16(uint32_t addr);
uint32_t aica_arm_read8(uint32_t addr);
void aica_arm_write32(uint32_t addr, uint32_t v);
void aica_arm_write16(uint32_t addr, uint32_t v);
void aica_arm_write8(uint32_t addr, uint32_t v);
extern uint8_t* aica_ram;
extern uint32_t aica_ram_mask;

void arm7_reset(void);
int arm7_run(int cycles);           /* runs about that many cycles; gives back how many it ran */
int arm7_run_fiq(int cycles);       /* the FIQ's handler at once, if it is due */
void arm7_stop(void);               /* from a write it makes: the run ends after that instruction */
void arm7_set_fiq(int on);          /* the AICA's interrupt line */
uint32_t arm7_pc(void);
uint64_t arm7_cycles(void);         /* run so far, for measuring */

#endif
