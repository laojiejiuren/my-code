#include "cpu.h"

NPCState npc_state = { .state = NPC_STOP };

void set_npc_state(int state, uint32_t pc, int halt_ret)
{
    npc_state.state = state;
    npc_state.halt_pc = pc;
    npc_state.halt_ret = halt_ret;
}

