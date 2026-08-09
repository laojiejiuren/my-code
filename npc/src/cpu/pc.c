#include <stdio.h>
#include <string.h>
#include "Vtop__Dpi.h"

// ----------- pc -----------

extern void pc_get(svBitVecVal *pc_);

uint32_t pc_gets()
{
  svBitVecVal val;
  pc_get(&val);
  return val;
}

// ----------- inst -----------

extern void pc_get(svBitVecVal* pc_);
