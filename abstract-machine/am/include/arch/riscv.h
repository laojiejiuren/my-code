#ifndef ARCH_H__
#define ARCH_H__

#ifdef __riscv_e
#define NR_REGS 16
#else
#define NR_REGS 32
#endif

struct Context {
  // TODO: fix the order of these members to match trap.S
  uintptr_t gpr[NR_REGS], mcause, mstatus, mepc;
  void *pdir;
};

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
} EVENT_;

#ifdef __riscv_e
#define GPR1 gpr[15] // a5
#else
#define GPR1 gpr[17] // a7
#endif

#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif
