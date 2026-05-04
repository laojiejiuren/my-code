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

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,
  TK_NUM,
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},
  {"\\*", '*'},
  {"\\/", '/'},
  {"\\(", '('},
  {"\\)", ')'},
  {"[0-9]+",TK_NUM},
  {"==", TK_EQ},        // equal
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_NUM://要处理溢出的情况
          {
            if(substr_len > 32 || nr_token > 65536)
            {
              printf("Overflow ! ! !\n");
              return false;
            }
            else
            {
              strcpy(tokens[nr_token].str,substr_start);
              tokens[nr_token].type = TK_NUM;
              nr_token++;
            }
            break;
          }
          case '+':case '-':case '*':case '/':case '(':case ')':
          {
            strcpy(tokens[nr_token].str,substr_start);
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
          }      
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static int check_parentheses(int p,int q)
{
  if(tokens[p].type != '(' || tokens[q].type != ')')//没有被一个括号包裹直接错误
    return false;

  int base = 0;
  for(int i = p; i <= q; ++i)
  {
    if(tokens[i].type == '(')
      base++;
    else if(tokens[i].type == ')')
      base--;

    if(base < 0)
      return false;
    if (base == 0 && i != q) //没有到最后一个括号之前就完成了匹配，也被认为是错误
      return false;
  }
  if(base != 0)
    return false;

  return true;
}

static uint32_t eval(int p,int q)
{
  if(p > q)
  {
    printf("ERROR:p > q\n");
    return 0;
  }

  else if(p == q)
  {
    if(tokens[p].type != TK_NUM)
    {
      printf("NOT NUMBER\n");
      return 0;
    }
    return strtoul(tokens[p].str,NULL,10);
  }
  else if(check_parentheses(p,q))
    return eval(p+1,q-1);

  else
  {
    //先找主运算符
    int op = -1;//记录主运算符位置
    int now = 0;
    int old = 1024;

    int base = 0;
    for(int i = p; i <= q; ++i)
    {
      if(tokens[i].type == TK_NUM)
        continue;

      //处理内部出现括号的情况
      if(tokens[i].type == '(')
        {base++;continue;}
      else if(tokens[i].type == ')')
        {base--;continue;}

      //处理优先级以及op的位置
      else if(base == 0)
      {
        if(tokens[i].type == '+' || tokens[i].type == '-')
          now = 1;
        else if(tokens[i].type == '*' || tokens[i].type == '/')
          now = 2;

        if(old > now || (old == now && i > op))
        {
          old = now;
          op = i;
        }
      }
    }

    if(op == -1)
    {
      printf("NO main operator\n");
      return 0;
    }

    //再求值
    uint32_t val1 = eval(p,op - 1);
    uint32_t val2 = eval(op + 1,q);
    switch(tokens[op].type)
    {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': 
      {
        if(val2 == 0)
        {
          printf("The divisor cannot be zero\n");
          return 0;
        }
        return val1 / val2;
      }
      default: 
      {
        printf("EROOR\n");
        return 0;
      }
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
