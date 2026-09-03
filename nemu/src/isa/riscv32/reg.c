/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  for(int i = 0; i < 32 ; ++i)
  {
    printf("%-3s: 0x%08x \n",reg_name(i),gpr(i));
  }
}

word_t isa_reg_str2val(const char *s, bool *success) 
{
  word_t result = 0;
  *success = false;

  if(strcmp(s,"PC") == 0)
  {
    *success = true;
    return cpu.pc;
  }

  for(int i = 0;i < 32; ++i)
  {
    if(strcmp(regs[i],s) == 0)
    {
      result = gpr(i);
      *success = true;
      break;
    }
  }
  return result;
}

word_t csr_read(vaddr_t addr)
{
  switch(addr)
  {
    case CSR_Mstatus: return cpu_csr.mstatus;
    case CSR_Mepc: return cpu_csr.mepc;
    case CSR_Mtvec: return cpu_csr.mtvec;
    case CSR_Mcause: return cpu_csr.mcause;
    case CSR_Mvendorid: return 0x79737978;
    case CSR_Marchid: return 0x018D7E6E;
    default: return 0;
  }
}

void csr_write(vaddr_t addr, word_t data)
{
  switch(addr)
  {
    case CSR_Mstatus: cpu_csr.mstatus = data; break;
    case CSR_Mepc: cpu_csr.mepc = data; break;
    case CSR_Mtvec: cpu_csr.mtvec = data; break;
    case CSR_Mcause: cpu_csr.mcause = data; break;
    default: return;
  }
}
