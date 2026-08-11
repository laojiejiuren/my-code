#include <stdio.h>
#include <assert.h>

FILE *openfile(const char *filename, const char *mode, long int *size)
{
    assert(filename != NULL);

    FILE *fp = fopen(filename,mode);
    assert(fp != NULL);

    fseek(fp,0,SEEK_END);
    *size = ftell(fp);
    fseek(fp,0,SEEK_SET);

    return fp;
}
