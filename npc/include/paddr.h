#ifndef __PADDR_H__
#define __PADDR_H__

#include "Vtop__Dpi.h"
#include <stdint.h>
#include <vector>
//using namespace std;

#define CONFIG_BASE 1
#define MAX_SIZE_MEM     (8 * 1024 * 1024)
#define SERIAL_ADDR  0x10000000
#define CLOCK_ADDR   0x20000000

#if CONFIG_BASE == 1
    #define BASE_ADDR    0x80000000
#else
    #define BASE_ADDR    0x00000000
#endif

extern std::vector<uint32_t> mem;
extern uint64_t boot_time;

//extern "C" void pmem_write(int waddr,int wdata,char wmask);
//extern "C" int pmem_read(int raddr);
//extern "C" void halt(int code);
//extern "C" void mem_en_get(svBit* ren_flag, svBit* wen_flag);

#endif
