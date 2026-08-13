#include "dir.h"
#include <assert.h>
#include <dlfcn.h>

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;

#if CONFIG_DIFFTEST

static int skip_dut_nr_inst = 0;

void npc_init_difftest(char *file_name, long img_size, int port)
{
    assert(filename);

    void *handle;
    handle = dlopen(file_name,)







}

#else
void npc_init_difftest(char *file_name, long img_size, int port){ }
#endif