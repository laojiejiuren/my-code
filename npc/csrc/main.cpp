#include <stdio.h>
//#include <nvboard.h>
#include "Vtop.h"
#include "svdpi.h"
#include "Vtop__Dpi.h"
#include <sys/time.h>
#include "dir.h"
#include "config.h"
#include "ftrace.h"
using namespace std;

struct timeval now;
vector<uint32_t> mem(MAX_SIZE_MEM,0);
uint64_t boot_time = 0;
static bool put_flag = false;
Vtop * top = new Vtop;
char *file_name = NULL;
Decode s;

#if CONFIG_MTRACE
  extern bool flag_rmem;
  extern bool flag_wmem;
  extern char rmembuf[128];
  extern char wmembuf[128];
#endif

static void trace_and_difftest(Decode *s)
{
  #if CONFIG_ITRACE
    //printf("%s\n",s->logbuf);
    FILE *fp = fopen("/home/lv/ysyx-workbench/npc/build/npc-log.txt", "a");
    if(!fp)
    {
      printf("Failed to open the file");
      return;
    }
    fputs(s->logbuf, fp);
    fputc('\n', fp);
    fclose(fp);
  #endif

  #if CONFIG_MTRACE
    if(flag_rmem)
      { puts(rmembuf);flag_rmem = false;}
    if(flag_wmem)
      { puts(wmembuf);flag_wmem = false;}
  #endif

  #if CONFIG_FTRACE
    uint32_t inst_ = s->inst;

    int rd = (inst_ >> 7) & 0x1f;
    int rs1 = (inst_ >> 15) & 0x1f;
    uint32_t opcode = inst_ & 0x7f;
    uint32_t func3 = (inst_ >> 12) & 0x7;

    switch(opcode)
    {
      case 0x6f://jal 只做调用函数
      { 
        //printf("rd: %d rs1: %d opcode: 0x%02x func3: %u\n",rd,rs1,opcode,func3);
        if(rd) ftrace_call(s->pc, s->snpc);
        break;
      }
      case 0x67://jalr 既调用又返回
      {
        if(func3 == 0)
        {
          //printf("rd: %d rs1: %d opcode: 0x%02x func3: %u\n",rd,rs1,opcode,func3);
          if(rd) ftrace_call(s->pc, s->snpc);
          else if(rd == 0 && rs1 == 1) ftrace_ret(s->pc);
        } 
        break;
      }
    }
  #endif

  #if CONFIG_WATCHPOINT
    if(!check_wp())
    {
      printf("触发监视点！！！\n");
      npc_state.state = NPC_STOP;
    }
  #endif
}

static void init_npc()
{
  top->clk = 0;
  top->rst = 1;
  for (int i = 0; i < 10; i++) 
  {
    top->clk = !top->clk;
    top->eval();
  }
  top->rst = 0; 
}

static void npc_exec_once(Decode *s)
{
  top->clk = !top->clk;
  top->eval();
  top->clk = !top->clk;
  top->eval();
  //printf("%u\n",top->data_out);
  s->pc = pc_gets();
  s->snpc = snpc_gets();
  s->inst = inst_gets();

#if CONFIG_ITRACE
  char *p = s->logbuf;
  p += snprintf(p,sizeof(s->logbuf), "0x%08x : ",s->pc);

  uint8_t *inst = (uint8_t *)&s->inst;
  for(int i = 4; i > 0; --i)
    p += snprintf(p, 4, "%02x", inst[i]);

  memset(p, ' ', 2);
  p += 2;
  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, s->logbuf + sizeof(s->logbuf) - p, s->pc, (uint8_t *)&s->inst, 4);
  p = p + '\n';
  p++;
#endif
}

static void execute(uint64_t n)
{ 
  for(; n > 0; --n)
  {
    npc_exec_once(&s);
    trace_and_difftest(&s);
    if(npc_state.state != NPC_RUNNING) break;
  }
} 

void npc_exec(uint64_t n)
{
  switch(npc_state.state)
  {
    case NPC_ABORT: case NPC_END: case NPC_QUIT:
      printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
      exit(0);
    default: npc_state.state = NPC_RUNNING;
  }

  execute(n);

  switch(npc_state.state)
  {
    case NPC_RUNNING: npc_state.state = NPC_STOP; break;
    
    case NPC_ABORT: case NPC_END:
    {
      if(npc_state.state == NPC_ABORT)
      {
        printf("" NPC_RED "ABORT PC = 0x%08x " NPC_NONE " \n",s.pc);
        exit(1);
      }
      else 
      {
        if(npc_state.halt_ret != 0)
        {
          printf("%u\n",npc_state.halt_ret);
          printf("" NPC_RED "NPC HIT BAD TRAP PC = 0x%08x " NPC_NONE " \n",s.pc);
          exit(0);
        }
        else 
        {
          printf("" NPC_GREEN "NPC HIT GOOD TRAP PC = 0x%08x " NPC_NONE " \n",s.pc);
          exit(0);
        }
      }
    }
    case NPC_STOP: printf("NPC STOP\n");
  }


}

int main(int argc,char** argv) 
{
  gettimeofday(&now,NULL);
  boot_time = now.tv_sec * 1000000 + now.tv_usec;
 
  if(argc < 3)
  {
    printf("NO FILE\n");
    return 1;
  }

  file_name = argv[2];

  FILE * F = fopen(argv[1],"rb");
  if(F == NULL )
  {
    printf("ERROR: Failed to open file\n");
    return 1;
  }

  fseek(F,0,SEEK_END);
  long SIZE = ftell(F);
  fseek(F,0,SEEK_SET);

  if(SIZE >= MAX_SIZE_MEM)
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

  svSetScope(svGetScopeFromName("TOP.top.u_idu.u_gpr"));

  init_npc();
  init_sdb();
  sdb_npc();
  delete top;
  return 0;
}
