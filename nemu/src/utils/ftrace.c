#include <common.h>
#include <elf.h>

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
    //先读取ELF文件的文件头
    //文件头包括了后续要使用的头节表的偏移量、.shstrtab的起始地址
    //头节表在文件的偏移量 --> e_shoff
    //以及头节表中的数量（节头数量）--> e_shnum
    Elf32_Ehdr ehdr;
    if(fread(&ehdr, sizeof(ehdr), 1, F) != 1 || memcmp(ehdr.e_ident, ELFMAG, 4) != 0 || !ehdr.e_shoff)
    {
        printf("不是有效的ELF文件\n");
        fclose(F);
        return;
    }

    //再读取节头表
    long shdr_size = ehdr.e_shentsize * ehdr.e_shnum;
    Elf32_Shdr *shdr = malloc(shdr_size);
    fseek(F, ehdr.e_shoff, SEEK_SET);
    if(fread(shdr, shdr_size, 1, F) != 1)
    {
        printf("读取头节表失败\n");
        fclose(F);
        return;
    }

    //读取.shstrtab
    Elf32_Shdr *shstrtab_hdr = &shdr[ehdr.e_shstrndx];
    char *shstrtab = malloc(shstrtab_hdr->sh_size);
    fseek(F, shstrtab_hdr->sh_offset, SEEK_SET);
    if(fread(shstrtab, shstrtab_hdr->sh_size, 1, F) != 1)
    {
        printf("读取.shstrtab失败\n");
        fclose(F);
        return;
    }

    //读取strtab和symtab
    Elf32_Shdr *strtab_hdr = NULL;
    Elf32_Shdr *symtab_hdr = NULL;
    for(int i = 0; i < ehdr.e_shnum; ++i)
    {
        char *tmp = shstrtab + shdr[i].sh_name;
        if(memcmp(tmp, ".symtab", 7) == 0)
            symtab_hdr = &shdr[i];
        else if(memcmp(tmp, ".strtab", 7) == 0)
            strtab_hdr = &shdr[i];
    }

    if(!strtab_hdr || !symtab_hdr)
    {
        printf("字符串表 or 符号表节 读取失败\n");
        fclose(F);
        return;
    }
    
    symtab = malloc(symtab_hdr->sh_size);
    sym_num = symtab_hdr->sh_size / sizeof(Elf32_Sym);
    fseek(F, symtab_hdr->sh_offset, SEEK_SET);
    if(fread(symtab, symtab_hdr->sh_size, 1, F) != 1)
    {
        printf("读取symtab失败\n");
        fclose(F);
        return;
    }

    strtab = malloc(strtab_hdr->sh_size);
    fseek(F, strtab_hdr->sh_offset, SEEK_SET);
    if(fread(strtab, strtab_hdr->sh_size, 1, F) != 1)
    {
        printf("读取strtab失败\n");
        fclose(F);
        return;
    }

    /*for(int i = 0; i < sym_num; ++i)
    {
        if((symtab[i].st_info & 0xf) == STT_FUNC)
        {
            char * tmp = strtab + symtab[i].st_name;
            if(symtab[i].st_name != 0) 
                printf("function name is '%s'\n",tmp);
            printf("0x%08x\n",symtab[i].st_value);
        }
    }*/

    fclose(F);
}

bool check_value(vaddr_t addr, int id)
{
    bool flag = false;
    if((symtab[id].st_info & 0xf) == STT_FUNC && 
                (symtab[id].st_value <= addr && addr < symtab[id].st_value + symtab[id].st_size))
        flag = true;

    return flag;
}

void ftrace_call(vaddr_t pc, vaddr_t next_pc)
{
    for(int i = 0; i < sym_num; ++i)
    {
        if(check_value(next_pc, i))
        {
            char *tmp = strtab + symtab[i].st_name;
            if(symtab[i].st_name != 0)
                printf(""FMT_WORD": call [%s@"FMT_WORD"] \n",pc, tmp, next_pc);
        }
    }
}

void ftrace_ret(vaddr_t pc)
{
    for(int i = 0; i < sym_num; ++i)
    {
         if(check_value(pc, i))
        {
            char *tmp = strtab + symtab[i].st_name;
            if(symtab[i].st_name != 0)
                printf(""FMT_WORD": ret [%s] \n",pc, tmp);
        }
    }
}
