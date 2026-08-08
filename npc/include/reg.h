#ifndef __REG_H__
#define __REG_H__

#include <stdint.h>

extern const char *regs[];
void reg_display();
uint32_t pc_display();

#endif
