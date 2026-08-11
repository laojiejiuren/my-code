#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdint>
#include <cstdio>
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define word_t uint32_t
#define vaddr_t uint32_t
FILE *openfile(const char *filename, const char *mode, long *size);

#endif
