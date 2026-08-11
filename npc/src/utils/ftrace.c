#include "dir.h"
#include <elf.h>
#include "common.h"

Elf32_Sym *symtab = NULL;
char *strtab = NULL;
int sym_num = 0;

void init_ftrace(const char *elf_file)
{
    if(elf_file == NULL)
    {
        printf("NO FILE! ! !\n");
        return;
    }
    
    FILE *F = fopen(elf_file, "rb");
    if(F == NULL)
    {
        printf("Error: failed to open file\n");
        return;
    }

    Elf32_Ehdr ehdr;
    if(fread(&ehdr, sizeof(ehdr), 1, F) != 1 || strncmp((const char *)ehdr.e_ident, ELFMAG, SELFMAG) != 0 || !ehdr.e_shoff)
    {
        printf("不是有效的ELF文件\n");
        return;
    }

    long shdr_size = ehdr.e_shentsize * ehdr.e_shnum;
    Elf32_Shdr *shdr = (Elf32_Shdr *)malloc(shdr_size);
    fseek(F, ehdr.e_shoff, SEEK_SET);
    if(fread(shdr, shdr_size, 1, F) != 1)
    {
        printf("读取节头表失败\n");
        fclose(F);
        return;
    }

    Elf32_Shdr *shstrtab_hdr = &shdr[ehdr.e_shstrndx];
    char *shstrtab = (char *)malloc(shstrtab_hdr->sh_size);
    fseek(F, shstrtab_hdr->sh_offset, SEEK_SET);
    if(fread(shstrtab, shstrtab_hdr->sh_size, 1, F) != 1)
    {
        printf("读取.shstrtab失败\n");
        fclose(F);
        return;
    }

    Elf32_Shdr *symtab_hdr = NULL;
    Elf32_Shdr *strtab_hdr = NULL;
    for(int i = 0; i < ehdr.e_shnum; ++i)
    {
        char *str = shdr[i].sh_name + shstrtab;
        if(strcmp(str, ".strtab") == 0)
            strtab_hdr = &shdr[i];
        else if(strcmp(str, ".symtab") == 0)
            symtab_hdr = &shdr[i];
    }

    strtab = (char *)malloc(strtab_hdr->sh_size);
    fseek(F, strtab_hdr->sh_offset, SEEK_SET);
    if(fread(strtab, strtab_hdr->sh_size, 1, F) != 1)
    {
        printf("读取.strtab失败\n");
        fclose(F);
        return;
    }

    symtab = (Elf32_Sym *)malloc(symtab_hdr->sh_size);
    sym_num = sizeof(symtab_hdr->sh_size) / sizeof(Elf32_Sym);
    fseek(F, symtab_hdr->sh_offset, SEEK_SET);
    if(fread(symtab, symtab_hdr->sh_size, 1, F) != 1)
    {
        printf("读取.shstrtab失败\n");
        fclose(F);
        return;
    }
    printf("ELF文件解析成功\n");
    for(int i = 0; i < sym_num; ++i)
    {
        if((symtab[i].st_info & 0xf) == STT_FUNC)
        {
            char * tmp = strtab + symtab[i].st_name;
            if(symtab[i].st_name != 0) 
                printf("function name is '%s'\n",tmp);
            printf("0x%08x\n",symtab[i].st_value);
        }
    }

    fclose(F);
}

static bool check_value(vaddr_t addr, int id)
{
    bool flag = false;
    if((symtab[id].st_info & 0xf) == STT_FUNC && 
                (symtab[id].st_value <= addr && addr < symtab[id].st_value + symtab[id].st_size))
        flag = true;

    return flag;
}

int cnt = 1;
void ftrace_call(vaddr_t pc, vaddr_t next_pc)
{
    for(int i = 0; i < sym_num; ++i)
    {
        if(check_value(next_pc, i))
        {
            char *tmp = strtab + symtab[i].st_name;
            if(symtab[i].st_name != 0)
            {
                printf("0x%08x: ",pc);
                for(int j = 0; j < cnt; ++j)
                    printf(" ");
                printf("call [%s@0x%08x]\n",tmp,next_pc);
            }
        }
    }
    cnt++;
}

void ftrace_ret(vaddr_t pc)
{
    cnt--;
    for(int i = 0; i < sym_num; ++i)
    {
        if(check_value(pc, i))
        {
            char *tmp = strtab + symtab[i].st_name;
            if(symtab[i].st_name != 0)
            {
                printf("0x%08x: ",pc);
                for(int j = 0; j < cnt; ++j)
                    printf(" ");
                printf("ret [%s]\n",tmp);
            }
        }
    }
}
