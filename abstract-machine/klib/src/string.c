#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t cnt = 0;
  while(*s != '\0')
  {
    cnt++;
    s++;
  }

  return cnt;
}

char *strcpy(char *dst, const char *src) {
  char * res = dst;
  while(*src != '\0')
  {
    *dst = *src;
    src++;
    dst++;
  }
  *dst = '\0';

  return res;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t n_tmp = strlen(src);//获取src的长度，如果长度大于n就直接复制，如果小于n就复制n-n_tmp
    
  if(n_tmp < n)
  {
    for(size_t i = 0; i < n_tmp; ++i)
    	dst[i] = src[i];
    for(size_t j = n_tmp; j < n; ++j)
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
}

int strcmp(const char *s1, const char *s2) {
  while((*s1 == *s2) && *s1 && *s2)
	{
		s1++;s2++;
	}
  return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	if(n == 0)
		return 0;
		
	size_t n_tmp = n;
	int val = 0;
  for(size_t i = n_tmp;i > 0; --i)
  {
    if(!*s1 || !*s2)
    {
    	val = (unsigned char)*s1 - (unsigned char)*s2;
    	break;
		}
		if(*s1 != *s2)
		{
			val = (unsigned char)*s1 - (unsigned char)*s2;
			break;
		}
		else val = 0;
		s1++;s2++;
	}
  return val;
}

void *memset(void *s, int c, size_t n) {
  unsigned char * tmp = (unsigned char *)s;	
	while(n--)
	{
		*tmp = (unsigned char)c;
		tmp++;
	}
  return s;
}

void* memmove(void* dst, const void* src, size_t n) {
	unsigned char* d = (unsigned char*)dst;
	const unsigned char* s = (const unsigned char*)src;

    if (d < s)
    {
      while (n--)
        *d++ = *s++;
    }
    else if (d > s)
    {
      while (n--)
        *(d + n) = *(s + n);
    }
    return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  unsigned char* d = (unsigned char*)out;
  const unsigned char* s = (const unsigned char*)in;

  while (n--)
  {
    *d++ = *s++;
  }
    return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char* d1 = (const unsigned char*)s1;
  const unsigned char* d2 = (const unsigned char*)s2;

  while (n-- && *d1 == *d2)
  {
    d1++; d2++;
  }
  if (n == (size_t)-1) return 0;
  int val = (unsigned char)*d1 - (unsigned char)*d2;

  if (val == 0) return 0;
  else if (val < 0) return -1;
  else return 1;
}

#endif
