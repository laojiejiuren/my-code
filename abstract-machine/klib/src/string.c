#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if(s == NULL)
  {
    panic("This is an empty string.");
    return -1;
  }
  size_t cnt = 0;
  while(*s != '\0')
  {
    cnt++;
    s++;
  }

  return cnt;
}

char *strcpy(char *dst, const char *src) {

  while(*src != '\0')
  {
    *dst = *src;
    src++;
    dst++;
  }
  *dst = '\0';

  return dst;
  //panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t n_tmp = strlen(src);//获取src的长度，如果长度大于n就直接复制，如果小于n就复制n-n_tmp
    
  if(n_tmp < n)
  {
    for(size_t i = 0; i < n_tmp; ++i)
    	dst[i] = src[i];
    for(size_t j = n - n_tmp - 1; j < n; ++j)
    	dst[j] = '\0';
  }
  else if(n_tmp >= n)
  {
  	for(size_t p = 0; p < n; ++p)
  		dst[p] = src[p];
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  size_t n = strlen(src);
	size_t m = strlen(dst);
   
	size_t i;
	for(i = 0;i < n; ++i)
	{
		dst[m++] = src[i];
	}
	dst[m] = '\0';
	return dst;
  
  //panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  while((*s1 == *s2) && *s1 && *s2)
	{
		s1++;
		s2++;
	}
	int val = (unsigned char)*s1 - (unsigned char)*s2;
	if(val > 0) return 1;
	else if(val == 0) return 0;
	else return -1;
  //panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  panic("Not implemented");
}

#endif
