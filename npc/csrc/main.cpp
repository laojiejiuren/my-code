#include <stdio.h>
#include <verilated.h>
#include <verilater_fst_c.h>
#include <Vtop.h>

vluint64_t main_time = 0;

int main(int argc,char** argv) 
{
  //要先初始化verilator->实例化顶层模块->初始化波形->正式开始仿真
  Verilated::commanArgs(argc,argv);
  Vtop * top = new Vtop;

  verilaterFstC * tfp = new VerilatedFstC;
  top->trace(tfp,99);
  tfp->open("wave.fst");

  while(main_time <= 10000)
  {
    top->clk = !top->clk;
    top->eval();
    tfp->dump(main_time++);
  }
  tfp->close();
  delete top;
  
  return 0;
}
