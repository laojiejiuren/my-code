#ifndef __FTRACE_H__
#define __FTRACE_H__

#include "common.h"

void init_ftrace(const char *elf_file);
void ftrace_call(vaddr_t pc, vaddr_t next_pc);
void ftrace_ret(vaddr_t pc);

#endif