#include "../include/common.h"
#include <regex>

extern void npc_exec(uint64_t n);
static int cmd_help(char *args);

char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(npc) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_q(char *args)
{
    exit(0);
} 

static int cmd_c(char *args)
{
    npc_exec(-1);
    return 0;
}

static int cmd_si(char *args)
{
    char *arg = strtok(NULL, " ");

    int n = 1;
    if(arg != NULL)
    {
        n = atoi(arg);
        if(n < 0)
        {
            printf("Input is EROOR!!!\n");
            return 0;
        }
    }
    npc_exec(n);
    return 0;
}

static int cmd_info(char *args)
{
  char *arg = strtok(NULL," ");

  if(arg==NULL)
  {
    printf("NO INPUT.Please enter info r or w\n");
    return 0;
  }

  bool flag = false;
  uint32_t val = reg_str2val(arg,&flag);

  if(strcmp(arg,"r") == 0)
    reg_display();
  //else if(strcmp(arg,"w") == 0)
    //show_wp();
  else if(flag)
    printf("%-3s: 0x%08x \n",arg,val);
  else
    printf("Please enter info r or w\n");
  
  return 0;
}

static int cmd_x(char *args)
{
  char *arg1 = strtok(NULL," ");//用来存储要输出的N
  char *arg2 = args + strlen(arg1) + 1;//用来存储表达式，目前是一个地址

  if(arg1 == NULL || arg2 == NULL)
  {
    printf("Please enter x N EXPR\n");
    return 0;
  }

  int n = atoi(arg1);
  if(n < 0)
  {
    printf("N must be positive\n");
    return 0;
  }

  bool flag = false;
  vaddr_t val = expr(arg2,&flag);
  if(!flag)
  {
    printf("ERROR EXPR at cmd_x\n");
    return 0;
  }
  //将字符串转换成vaddr_t类型
  //vaddr_t val = strtoul(arg2,NULL,16);
  for(int i = 0; i < n; ++i)
  {
    vaddr_t next_addr = (val + i * 4) ;//- BASE_ADDR;
    word_t out_data = pmem_read(next_addr);
    vaddr_t real_addr = next_addr + BASE_ADDR;
    printf("0x%08x : 0x%08x\n",real_addr,out_data);
  }

  return 0;
}

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si", "Step one or N instructions",cmd_si},
  {"info","Printf registers: info r or w",cmd_info},
  {"x","Scan memory: x N EXPR",cmd_x},
 // {"p","Expression Evaluation: p EXPR",cmd_p},
 // {"w","Add watchpoint: w EXPR",cmd_w},
 // {"d","Delete watchpoint: d N",cmd_d},
  /* TODO: Add more commands */
};
#define CMD_N ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < CMD_N; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < CMD_N; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_npc()
{
    init_regex();
    for(char *ch; (ch = rl_gets()) != NULL;)//不断读取命令行的字符串
    {
        char *ch_end = ch + strlen(ch);

        //先要匹配对应的命令
        char *cmd = strtok(ch, " ");
        if(cmd == NULL) { continue;}

        char *args = cmd + strlen(cmd) + 1;
        if(args >= ch_end)
            args = NULL;

        int i;
        for(i = 0; i < CMD_N; ++i)
        {
            if(strcmp(cmd, cmd_table[i].name) == 0)
            {
                if(cmd_table[i].handler(args) < 0) {return;}
                break;
            }
        }
        if(i == CMD_N){ printf("Unknown command '%s'\n",cmd); }
    }
}
