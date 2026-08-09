#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#define MAX_SIZE 256
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  char expr_str[MAX_SIZE];
  uint32_t val;
  /* TODO: Add more members if necessary */

} WP;

void show_wp();
WP* new_wp(char *ch);
void free_wp(WP *wp);
void delete_wp(int id);
bool check_wp();
#endif