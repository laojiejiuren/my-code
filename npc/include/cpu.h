#ifndef __CPU_H__
#define __CPU_H__


#include <stdio.h>
#include <stdint.h>
#include <cstdint>

// -------- PC --------

uint32_t pc_gets();
uint32_t inst_gets();

// -------- reg --------

extern const char *regs[];
void reg_display();
uint32_t reg_gets(int id);
uint32_t reg_str2val(const char *s, bool *success);

// -------- state --------

enum { NPC_RUNNING, NPC_STOP, NPC_END, NPC_ABORT, NPC_QUIT };

typedef struct {
  int state;
  uint32_t halt_pc;
  uint32_t halt_ret;
} NPCState;

extern NPCState npc_state;

// -------- cpu.state --------
typedef struct{
  uint32_t pc;
  uint32_t inst;
  char logbuf[128];
  char ringbuf[20][128];
} Decode;

#endif
