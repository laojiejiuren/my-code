#include <am.h>
#include <riscv/riscv.h>
#include <klib-macros.h>
#include <stdio.h>
#include <string.h>
#define SERIAL_ADDR 0x10000000

extern char _heap_start;
int main(const char *args);

extern char _pmem_start;
#define PMEM_SIZE (128 * 1024 * 1024)
#define PMEM_END  ((uintptr_t)&_pmem_start + PMEM_SIZE)

Area heap = RANGE(&_heap_start, PMEM_END);
static const char mainargs[MAINARGS_MAX_LEN] = TOSTRING(MAINARGS_PLACEHOLDER); // defined in CFLAGS

void putch(char ch) {
  outl(SERIAL_ADDR,ch);
}

void halt(int code) {
  asm volatile("mv a0, %0; ebreak" : :"r"(code));
  while (1);
}

void _trm_init() {
  uint32_t string, id;

  asm volatile ("csrr %0, mvendorid" : "=r"(string));
  asm volatile ("csrr %0, marchid" : "=r"(id));

  char *ch = (char *)&string;
  char tmp[10];
  for(int i = 3; i >=0 ;--i)
    tmp[3 - i] = ch[i]; 

  printf("%s_%d\n",tmp,id);

  int ret = main(mainargs);
  halt(ret);
}
                      