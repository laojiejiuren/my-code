#ifndef __SDB_H__
#define __SDB_H__

#include <stdint.h>
#include <stdio.h>
char* rl_gets();
void sdb_npc();
void init_regex();
uint32_t expr(char *e, bool *success);

#endif
