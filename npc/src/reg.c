#include "../include/reg.h"
#include <stdio.h>
#include <string.h>
#include "Vtop__Dpi.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

extern void gpr_get(int id, svBitVecVal *reg_data);
extern void pc_get(svBitVecVal *pc_);

uint32_t pc_gets()
{
  svBitVecVal val;
  pc_get(&val);
  return val;
}

uint32_t reg_gets(int id)
{
  svBitVecVal val;
  gpr_get(id, &val);
  return val;
}

void reg_display()
{
  for(int i = 0; i < 32; ++i)
  {
    svBitVecVal val;
    gpr_get(i, &val);
    printf("%-3s 0x%08x\n", regs[i], val);
  }
}

uint32_t reg_str2val(const char *s, bool *success)
{
  uint32_t res = 0;
  *success = false;

  if(strcmp(s, "PC") == 0)
  {
    *success = true;
    res = pc_gets();
    return res;
  }

  for(int i = 0; i < 32; ++i)
  {
    if(strcmp(s, regs[i]) == 0)
    {
      res = reg_gets(i);
      *success = true;
      break;
    }
  }
  return res;
}
