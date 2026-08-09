#ifndef __REG_H__
#define __REG_H__

#include <stdint.h>
#include <stdio.h>
extern const char *regs[];
void reg_display();
uint32_t reg_gets(int id);
uint32_t reg_str2val(const char *s, bool *success);

#endif
