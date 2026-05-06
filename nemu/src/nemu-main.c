/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

#include <stdio.h>
word_t expr(char *e, bool *success);
void init_regex();
static void expr_test(char *filename)
{
  FILE * fp;
  fp = fopen(filename,"r");
  if(!fp)
  {
    perror("ERROR for opening fiel");
    return;
  }

  char buf[65536 + 10];
  int total = 0,passed = 0;

  while(fgets(buf,sizeof(buf),fp))
  {
    //我需要从buf中取出第一个字符是一个结果，再取出表达式给expr进行计算，然后再比较两个值是否相等
    uint32_t tmp;
    int idx;
    if(sscanf(buf,"%u %n",%tmp,%idx) != 1)
      continue;

    char * expr_str = buf + idx;
    bool success;
    size_t len = strlen(expr_str);
    if(len > 0 && expr_str[len - 1] == '\n')
    {
      expr_str[len - 1] = '\0';
      len--;
    }

    word_t result = expr(expr_str,success);
    total++;

    if(success && result == tmp)
      passed++;
    else
    {
      printf("FAIL: expected %u, got %u (success=%d) for: %s\n",
             expected, (unsigned)result, success, expr_str);
    }

    fclose(fp);
    printf("Test: Total:%d,Passed:%d",total,passed);
  }

}

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  if(argc >= 2 && strcmp(argv[1], "tools/gen-expr/input") == 0)
  {
    char *filename = argv[1];
    expr_test(filename);
    return 0;
  } 
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
