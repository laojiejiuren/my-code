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
// head用于组织使用中的监视点结构
// free_用于组织空闲的监视点结构
#include "sdb.h"

#define NR_WP 32
#define MAX_SIZE 256
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  char expr_str[MAX_SIZE];
  word_t val;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

WP* new_wp(char *ch)
{
  if(free_ == NULL)
    assert(0);
  WP* tmp = free_;
  free_ = free_ -> next;//将节点后移

  //将传入的表达式写入
  int i;
  for(i = 0;i < MAX_SIZE && ch[i] != '\0'; ++i)
    tmp->expr_str[i] = ch[i];

  tmp->expr_str[i] = '\0';

  bool flag;
  word_t val = expr(tmp->expr_str,&flag);

  if(!flag) assert(0);
  tmp->val = val;

  tmp->next = head;//头插法
  head = tmp;
  return head;
}

void free_wp(WP *wp)
{
  if(wp == NULL)
    return;

  if(wp == head)
  {
    WP * tmp = head;
    head = head->next;

    tmp->next = free_;
    free_ = tmp;
  }
  else
  {
    WP * p = head;
    while(p != NULL && p->next != wp)
      p = p->next;

    //现在p->next = wp的前一个节点
    p->next = wp->next;
    wp->next = free_;
    free_ = wp;
  } 
}

void show_wp()
{
  WP * p = head;
  while(p != NULL)
  {
    printf("watchpoint:%d %s %u\n",p->NO,p->expr_str,p->val);
    p = p->next;
  }
}

void delete_wp(int id)
{
  if(id < 0 || id >= 32)
    assert(0);

  WP * p = head;
  while(p != NULL && p->NO != id)
    p = p->next;
  free_wp(p);

  printf("Be deleted watchpoint is :%d %s\n",p->NO,p->expr_str);
}

/* TODO: Implement the functionality of watchpoint */

