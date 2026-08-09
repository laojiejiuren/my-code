#include "Vtop__Dpi.h"
#include "../include/common.h"
#include "../include/dir.h"

extern struct timeval now;

int pmem_read(int raddr)
{
  if(raddr == CLOCK_ADDR || raddr == CLOCK_ADDR + 4)
  {
    gettimeofday(&now, NULL);
    uint64_t us = now.tv_sec * 1000000 + now.tv_usec;

    if(raddr == CLOCK_ADDR)
      return (int)(uint32_t)(us - boot_time);
    else if(raddr == CLOCK_ADDR + 4)
      return (int)(uint32_t)((us - boot_time) >> 32);
  }

  uint32_t addr = (raddr & ~0x3u) - BASE_ADDR;
  if(addr >= MAX_SIZE_MEM * 4) return 1;
  return mem[addr >> 2];
}
//0x12345678
void pmem_write(int waddr,int wdata,char wmask)
{
  if(waddr == SERIAL_ADDR)
  {
    putchar(wdata);
    return;
  }

  uint32_t addr =  (waddr & ~0x3u) - BASE_ADDR;
  if(addr >= MAX_SIZE_MEM * 4) return;
  uint32_t id = addr >> 2;
  uint32_t old_data = mem[id];

  uint32_t new_data = old_data;
  if(wmask & 0x1)
    new_data = (new_data & 0xffffff00) | (wdata & 0x000000ff);
  if(wmask & 0x2)
    new_data = (new_data & 0xffff00ff) | (wdata & 0x0000ff00);
  if(wmask & 0x4)
    new_data = (new_data & 0xff00ffff) | (wdata & 0x00ff0000);
  if(wmask & 0x8)
    new_data = (new_data & 0x00ffffff) | (wdata & 0xff000000);

  mem[id] = new_data;
} 

void halt(int code)
{
  //根据nemu的halt实现
  if(code == 0)
  {
    printf("    " NPC_GREEN " HIT GOOD TRAP!!! " NPC_NONE" \n");
    uint32_t pc = pc_gets();
    printf("pc: 0x%08x\n",pc);
    exit(0);
  }
  else
  {
    printf("    " NPC_RED " HIT BAD TRAP!!! " NPC_NONE" \n");
    exit(1);
  }
}
