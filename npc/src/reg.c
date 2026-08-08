#include "../include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

extern void gpr_get(int id, uint32_t *reg_data);

void reg_diplay()
{
  for(int i = 0; i < 32; ++i)
  {
    uint32_t *val;
    printf("%-3s 0x%08x",res[i], val);
  }
}