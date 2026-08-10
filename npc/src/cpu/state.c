#include "cpu.h"

NPCState npc_state = { .state = NPC_STOP };

extern "C" void set_npc_state(int state, uint32_t pc, uint32_t halt_ret)
{
    npc_state.state = state;
    npc_state.halt_pc = pc;
    npc_state.halt_ret = halt_ret;
}

