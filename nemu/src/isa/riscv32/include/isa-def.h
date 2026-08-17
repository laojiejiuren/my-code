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

#ifndef __ISA_RISCV_H__
#define __ISA_RISCV_H__

#include <common.h>

typedef struct {
  word_t gpr[MUXDEF(CONFIG_RVE, 16, 32)];
  vaddr_t pc;
} MUXDEF(CONFIG_RV64, riscv64_CPU_state, riscv32_CPU_state);

typedef struct{
  word_t mtvec;   //入口地址
  word_t mcause;  //异常原因
  word_t mepc;    //触发异常的PC地址
  word_t mstatus; //状态
} CSR;

typedef enum {
  INSTRUCTION_ADDRESS_MISALIGNED = 0,  //指令地址未对齐
  INSTRUCTION_ACCESS_FAULT       = 1,  //指令访问错误
  ILLEGAL_INSTRUCTION            = 2,  //非法指令
  BREAKPOINT                     = 3,  //断点
  LOAD_ADDRESS_MISALIGNED        = 4,  //加载地址未对齐
  LOAD_ACCESS_FAULT              = 5,  //加载访问错误
  STORE_ADDRESS_MISALIGNED       = 6,  //存储地址未对齐
  STORE_ACCESS_FAULT             = 7,  //存储访问错误
  ENVIRONMENT_CALL_FROM_U_MODE   = 8,  //用户模式的环境调用
  ENVIRONMENT_CALL_FROM_S_MODE   = 9,  //特权模式的环境调用
  ENVIRONMENT_CALL_FROM_M_MODE   = 11, //机器模式的环境调用
  INSTRUCTION_PAGE_FAULT         = 12, //指令页错误
  LOAD_PAGE_FAULT                = 13, //加载页错误
  STORE_PAGE_FAULT               = 15, //存储页错误
} EVENT;

// decode
typedef struct {
  uint32_t inst;
} MUXDEF(CONFIG_RV64, riscv64_ISADecodeInfo, riscv32_ISADecodeInfo);

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
