#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int add_buff(char *out, const char *fmt, va_list ap)
{
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
        unsigned int num_tmp = num;
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
          num_tmp = -(unsigned)num;
        }

        while (num_tmp)
        {
          buf[id++] = (num_tmp % 10) + '0';
          num_tmp /= 10;
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
      else if(*tmp == 'c')
      {
        int s1 = va_arg(ap, int);
        *out++ = (char)s1;
        cnt++;
      }
      else if(*tmp == 'x')
      {
        unsigned int num_x = va_arg(ap, unsigned int);
        char buf_x[20] = {};
        int id_x = 0;

        while(num_x)
        {
          buf_x[id_x++] = (num_x % 16) >= 10 ? (num_x % 16) + 'a' : (num_x % 16) + '0';
          num_x /= 16;
          cnt++;
        }
        for (int i = id_x - 1; i >= 0; --i)
        {
          *out = buf_x[i];
          out++;
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
  return cnt;
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);

  const char *tmp = fmt;
  char buff[256];
  int cnt = add_buff(buff,tmp,ap);

  va_end(ap);
  putstr(buff);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  const char* tmp = fmt;

  int cnt = add_buff(out,tmp,ap);
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
