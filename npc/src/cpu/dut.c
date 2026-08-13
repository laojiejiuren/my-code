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
    handle = dlopen(file_name, RTLD_LAZY);
    assert(handle);

    ref_difftest_memcpy = dlsym(handle, "difftest_memcpy");
    assert(ref_difftest_memcpy);

    ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");
    assert(ref_difftest_regcpy);

    ref_difftest_exec = dlsym(handle, "difftest_exec");
    assert(ref_difftest_exec);

    void (*ref_difftest_init)(int) = dlsym(handle, "difftest_init");
    assert(ref_difftest_init);

    ref_difftest_init(port);
    ref_difftest_memcpy(RESET_VECTOR, guest_to_host(RESET_VECTOR), img_size, DIFFTEST_TO_REF);
    ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);


}

#else
void npc_init_difftest(char *file_name, long img_size, int port){ }
#endif