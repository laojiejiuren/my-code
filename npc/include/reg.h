#ifndef __REG_H__
#define __REG_H__

#include <stdint.h>

extern const char *regs[];
extern void reg_display(int id, uint32_t *reg_data);

#endif
