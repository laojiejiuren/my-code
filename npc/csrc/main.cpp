#include <stdio.h>
//#include <nvboard.h>
#include "Vtop.h"
#include "svdpi.h"
#include "Vtop__Dpi.h"
#include <vector>
using namespace std;

vluint64_t main_time = 0;
vector<uint32_t> mem(256,0);

uint32_t pmem_read(uint32_t addr)
{
  return mem[addr >> 2];
}

extern "C" void halt()
{
  exit(0);
}

int main(int argc,char** argv) 
{
  mem[0] = 0x00100a93;
  mem[1] = 0x002a8a93;
  mem[2] = 0x003a8a93;
  mem[3] = 0x004a8a93;
  mem[4] = 0x005a8a93;
  mem[5] = 0x006a8a93;

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
    top->inst = pmem_read(top->pc);
    printf("pc:%02x\n",top->pc);
    printf("s5:%d\n",top->data_out);
    top->clk = !top->clk;
    top->eval();
  }
  delete top;
  return 0;
}
