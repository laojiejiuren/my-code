#ifndef __SDB_H__
#define __SDB_H__

#include <stdint.h>
#include <stdio.h>
char* rl_gets();
void sdb_npc();
void init_sdb();
uint32_t expr(char *e, bool *success);

void init_regex();
void init_wp_pool();

#endif
