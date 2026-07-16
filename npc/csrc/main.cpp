#define MAX_SIZE (8 * 1024 * 1024)
#include <stdio.h>
//#include <nvboard.h>
#include "Vtop.h"
#include "svdpi.h"
#include "Vtop__Dpi.h"
#include <vector>
using namespace std;

vluint64_t main_time = 0;
vector<uint32_t> mem(MAX_SIZE,0);

extern "C" int pmem_read(int raddr)
{
  uint32_t addr = raddr & ~0x3u;
  return mem[addr >> 2];
}
//0x12345678
extern "C" void pmem_write(int waddr,int wdata,char wmask)
{
  uint32_t id =  (waddr & ~0x3u) >> 2;
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

extern "C" void halt()
{
  exit(0);
}

int main(int argc,char** argv) 
{
  if(argc < 2)
  {
    printf("NO FILE\n");
    return 1;
  }

  FILE * F = fopen(argv[1],"rb");
  if(F == NULL )
  {
    printf("ERROR: Failed to open file\n");
    return 1;
  }

  fseek(F,0,SEEK_END);
  long SIZE = ftell(F);
  fseek(F,0,SEEK_SET);

  if(SIZE >= MAX_SIZE)
  {
    printf("ERROR: File too large. SIZE:%ld",SIZE);
    fclose(F);
    return 1;
  }

  fread(mem.data(),1,SIZE,F);
  fclose(F);
  mem[1220] = 0x00100073;
  //要先初始化verilator->实例化顶层模块->初始化波形->正式开始仿真
  Verilated::commandArgs(argc,argv);

  Vtop * top = new Vtop;

  top->clk = 0;
  top->rst = 1;
  for (int i = 0; i < 10; i++) 
  {
    top->clk = !top->clk;
    top->eval();
  }
  top->rst = 0; 

  int n=50;

  while(n--)
  {
    printf("pc=0x%08x a0=%d\n",top->pc,top->data_out);
    top->clk = !top->clk;
    top->eval();
    printf("pc=0x%08x a0=%d\n",top->pc,top->data_out);
    top->clk = !top->clk;
    top->eval();
  }
  delete top;
  return 0;
}
