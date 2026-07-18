#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int vsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
  const char *tmp = fmt; 
  while (*tmp != '\0')
  {
    if (*tmp == '%')
    {
      tmp++;
      if(*tmp == 'd')
      {
        int num = va_arg(ap, int);
        char buf[20] = {};
        int id = 0;

        if(num == 0)
        {
          *out++ = '0';
          cnt++;
        }
        else if(num < 0)
        {
          *out++ = '-';
          cnt++;
          num = -num;
        }

        while (num)
        {
          buf[id++] = (num % 10) + '0';
          num /= 10;
          cnt++;
        }

        for (int i = id - 1; i >= 0; --i)
        {
          *out = buf[i];
          out++;
        }
      }
      else if (*tmp == 's')
      {
        //printf("string is: %s\n", va_arg(ap, int));
        char * s1 = va_arg(ap, char*);
        while (*s1 != '\0')
        {
          *out++ = *s1++;
          cnt++;
        }
      }
    }
    else
    {
      *out++ = *tmp;
      cnt++;
    }
    tmp++;
  }

  *out = '\0';
  return cnt ? cnt : -1; 
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);

  const char *tmp = fmt;
  char buff[256];
  int cnt = vsprintf(buff,tmp,ap);
  va_end(ap);
  putstr(buff);
  return cnt;
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
  va_start(ap, fmt);
  const char* tmp = fmt;

  int cnt = vsprintf(out,tmp,ap);
  va_end(ap);
  return cnt ? cnt : -1; 
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
