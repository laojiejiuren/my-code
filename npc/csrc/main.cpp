#include <stdio.h>
#include <nvboard.h>
#include "Vtop.h"

vluint64_t main_time = 0;
void nvboard_bind_all_pins(Vtop* top);

int main(int argc,char** argv) 
{
  //要先初始化verilator->实例化顶层模块->初始化波形->正式开始仿真
  Verilated::commandArgs(argc,argv);

  Vtop * top = new Vtop;


  nvboard_bind_all_pins(top);
  nvboard_init();

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
    nvboard_update();
    //if(top->ram_mem2 == 55)
      //break;
    top->eval();
  }
   nvboard_quit();
  delete top;
  
  return 0;
}
