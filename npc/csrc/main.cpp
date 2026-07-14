#include <stdio.h>
//#include <nvboard.h>
#include "Vtop.h"
#include <vector>

vluint64_t main_time = 0;
vector<uint32_t> mem(256,0);

int main(int argc,char** argv) 
{
  //要先初始化verilator->实例化顶层模块->初始化波形->正式开始仿真
  Verilated::commandArgs(argc,argv);

  Vtop * top = new Vtop;

  top->clk = 0;
  top->reset = 1;
  for (int i = 0; i < 10; i++) 
  {
    top->clk = !top->clk;
    top->eval();
  }
    top->reset = 0; 

  while(1)
  {
    top->clk = !top->clk;
    printf("R2:%02x\n",top->out);
    top->eval();
  }
  delete top;
  return 0;
}
while (???) {
  ...
  top->inst = pmem_read(top->pc);
  top->eval();
  ...
}