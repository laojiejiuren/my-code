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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

#define MAX_SIZE 65536
#define Depth 5
static int id;

static uint32_t choose(uint32_t n)
{
  return rand() % n;
}

static void gen_space()
{
  int check = choose(2);//为1就输入空格，为0就不输入
  if(check)
  {
    int cnt = choose(4);//0~3个空格
    for(int i = 0; i < cnt; ++i)
      if((id) < MAX_SIZE - 1)
        buf[id++] = ' ';
  }
}

static void gen_num()//用于生成随机数字
{
  uint32_t num = (uint32_t)rand();
  int cnt = snprintf(buf + id,MAX_SIZE - id,"%uu",num);
  if(cnt > 0) id += cnt;//因为snprintf在出现错误时会返回负值
}

static void gen_nzeronum()
{
  uint32_t num = (uint32_t)rand() + 1;
  int cnt = snprintf(buf + id,MAX_SIZE - id,"%uu",num);
  if(cnt > 0) id += cnt;
}

static char gen_rand_op()//用于生成随机运算符
{
  char tmp[] = {'+','-','*','/'};
  int idx = choose(4);
  return tmp[idx];
}

static void gen_rand_expr_my(int dep)
{
  if(dep > Depth)
  {
    gen_space();
    if(id < MAX_SIZE - 1) gen_num();
    gen_space();
    return;
  }
    
  switch(choose(3))
  {
    case 0://数字
    {
      gen_space();
      gen_num();
      gen_space();
      break;
    }
    case 1://括号
    {
      gen_space();
      if(id < MAX_SIZE - 1) buf[id++] = '(';
      gen_space();
      gen_rand_expr_my(dep + 1);
      gen_space();
      if(id < MAX_SIZE - 1) buf[id++] = ')';
      break;
    }
    default:
    {
      gen_rand_expr_my(dep + 1);

      gen_space();
      char tmp = gen_rand_op();
      if(id < MAX_SIZE - 1) buf[id++] = tmp;
      gen_space();

      if(tmp == '/')
        gen_nzeronum();
      else gen_rand_expr_my(dep + 1);
    }
  }
}

static void gen_rand_expr() //生成表达式
{
  id = 0;
  buf[0] = '\0';
  gen_rand_expr_my(0);
  buf[id] = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
