#ifndef __PADDR_H__
#define __PADDR_H__

// #include "common.h"
#include <vector>
#include "Vtop__Dpi.h"
#include <stdint.h>
using namespace std;

#define MAX_SIZE     (8 * 1024 * 1024)
#define BASE_ADDR    0x80000000
#define SERIAL_ADDR  0x10000000
#define CLOCK_ADDR   0x20000000
extern vector<uint32_t> mem;
extern uint64_t boot_time;

extern "C" void pmem_write(int waddr,int wdata,char wmask);
extern "C" int pmem_read(int raddr);
extern "C" void halt(int code);

#endif
