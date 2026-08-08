#include "../include/common.h"
extern int NPC_N;

char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(npc) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

/*static int cmd_info_npc()
{
  svLogicVecVal val;
  for(int i = 0; i < 32; ++i)
  {
    reg_display(i, &val);
    printf("%-3s: 0x%08x\n", regs[i], val.aval);
  }
  return 0;
}

*/



