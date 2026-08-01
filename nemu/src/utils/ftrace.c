#include <common.h>
#include <elf.h>

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

    //先检查是不是elf文件,读取ELF文件头
    Elf32_Ehdr ehdr;
    if(fread(&ehdr,sizeof(ehdr),1,F) != 1 || memcmp(ehdr.e_ident, ELFMAG, 4) != 0 || ehdr.e_shoff == 0)
    {
        printf("不是有效的ELF文件\n");
        fclose(F);
        return;
    }

    //读取字节表
    long long sht_size = ehdr.e_shentsize * ehdr.e_shnum;
    Elf32_Shdr *shdr = malloc(sht_size);
    fseek(F, ehdr.e_shoff, SEEK_SET);
    if(fread(shdr,sht_size,1,F) != 1)
    {
        printf("读取字节表出错\n");
        fclose(F);
        return;
    }

        // 读取 section 名字符串表 (shstrtab)，用于查找 .symtab 和 .strtab
    Elf32_Shdr shstrtab_hdr = shdr[ehdr.e_shstrndx];
    char *shstrtab = malloc(shstrtab_hdr.sh_size);
    fseek(F, shstrtab_hdr.sh_offset, SEEK_SET);
    if (fread(shstrtab, shstrtab_hdr.sh_size, 1, F) != 1) {
        printf("读取 section 名字符串表出错\n");
        free(shstrtab);
        free(shdr);
        fclose(F);
        return;
    }

    // 遍历所有 section header，找到 .symtab 和 .strtab
    Elf32_Shdr *symtab_hdr = NULL;
    Elf32_Shdr *strtab_hdr = NULL;

    for (int i = 0; i < ehdr.e_shnum; i++) {
        const char *name = shstrtab + shdr[i].sh_name;
        if (strcmp(name, ".symtab") == 0) {
            symtab_hdr = &shdr[i];
        } else if (strcmp(name, ".strtab") == 0) {
            strtab_hdr = &shdr[i];
        }
    }

    if (symtab_hdr == NULL || strtab_hdr == NULL) {
        printf("找不到 .symtab 或 .strtab 节\n");
        free(shstrtab);
        free(shdr);
        fclose(F);
        return;
    }

    // 读取符号表
    int sym_count = symtab_hdr->sh_size / sizeof(Elf32_Sym);
    Elf32_Sym *symtab = malloc(symtab_hdr->sh_size);
    fseek(F, symtab_hdr->sh_offset, SEEK_SET);
    if (fread(symtab, symtab_hdr->sh_size, 1, F) != 1) {
        printf("读取符号表出错\n");
        free(shstrtab);
        free(symtab);
        free(shdr);
        fclose(F);
        return;
    }

    // 读取字符串表
    char *strtab = malloc(strtab_hdr->sh_size);
    fseek(F, strtab_hdr->sh_offset, SEEK_SET);
    if (fread(strtab, strtab_hdr->sh_size, 1, F) != 1) {
        printf("读取字符串表出错\n");
        free(shstrtab);
        free(symtab);
        free(strtab);
        free(shdr);
        fclose(F);
        return;
    }

    // 遍历符号表，打印函数符号（可选，用于调试）
    for (int i = 0; i < sym_count; i++) {
        if (symtab[i].st_name != 0) {
            const char *sym_name = strtab + symtab[i].st_name;
            printf("%s\n", sym_name);
        }
    }

    free(shstrtab);
    // symtab 和 strtab 如果是全局用的就不要在这里 free



    //再将符号表和字符串表写入缓冲数组

}
