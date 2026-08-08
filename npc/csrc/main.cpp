#include <stdio.h>
//#include <nvboard.h>
#include "Vtop.h"
#include "svdpi.h"
#include "Vtop__Dpi.h"
#include <sys/time.h>
#include "../include/common.h"
using namespace std;

struct timeval now;
vector<uint32_t> mem(MAX_SIZE,0);
uint64_t boot_time = 0;
//int NPC_N = 12330;

int main(int argc,char** argv) 
{
  gettimeofday(&now,NULL);
  boot_time = now.tv_sec * 1000000 + now.tv_usec;
 
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
  //mem[0x224 >> 2] = 0x00100073; 
  //要先初始化verilator->实例化顶层模块->初始化波形->正式开始仿真
  Verilated::commandArgs(argc,argv);

  Vtop * top = new Vtop;

  svSetScope(svGetScopeFromName("TOP.top.u_idu.u_gpr"));

  top->clk = 0;
  top->rst = 1;
  for (int i = 0; i < 10; i++) 
  {
    top->clk = !top->clk;
    top->eval();
  }
  top->rst = 0; 
  //rl_gets();
  while(1)
  {
    //printf("a0: %02x\n",top->data_out);
    //printf("%d\n",boot_time / CLOCKS_PER_SEC);
    top->clk = !top->clk;
    top->eval();
    //NPC_N--;
  }
  delete top;
  return 0;
}
