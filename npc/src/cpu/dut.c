#include "dir.h"
#include <assert.h>
#include <dlfcn.h>

void (*ref_difftest_memcpy)(uint32_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;

#if CONFIG_DIFFTEST

bool is_skip_ref = false;
static int skip_dut_nr_inst = 0;

static void npc_reg_up(CPU_state *cpu)
{
    for(int i = 0; i < 32; ++i)
        cpu->gpr[i] = reg_gets(i);
}

void npc_init_difftest(char *file_name, long img_size, int port)
{
    assert(file_name);

    void *handle;
    handle = dlopen(file_name, RTLD_LAZY);
    assert(handle);

    ref_difftest_memcpy = (void (*)(uint32_t, void*, size_t, bool))dlsym(handle, "difftest_memcpy");
    assert(ref_difftest_memcpy);

    ref_difftest_regcpy = (void (*)(void*, bool))dlsym(handle, "difftest_regcpy");
    assert(ref_difftest_regcpy);

    ref_difftest_exec = (void (*)(uint64_t))dlsym(handle, "difftest_exec");
    assert(ref_difftest_exec);

    void (*ref_difftest_init)(int) = (void (*)(int))dlsym(handle, "difftest_init");
    assert(ref_difftest_init);

    ref_difftest_init(port);
    ref_difftest_memcpy(BASE_ADDR, mem.data(), img_size, DIFFTEST_TO_REF);

    cpu.pc = pc_gets();
    npc_reg_up(&cpu);
    ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}

static void checkregs(CPU_state *ref_r, uint32_t pc)
{
    bool flag = true;

    if(ref_r->pc != pc)
    {
        flag =false;
        printf("REF PC: 0x%08x DUT PC: 0x%08x", ref_r->pc, pc);
    }
    for(int i = 0; i < 32; ++i)
    {
        if(ref_r->gpr[i] != cpu.gpr[i])
        {
            flag = false;
            break;
        }
    }
    if(!flag)
    {
        npc_state.state = NPC_ABORT;
        npc_state.halt_pc = pc;
        reg_display();

        printf("------REF regs------\n");
        for(int i = 0; i < 32; ++i)
            printf("%-3s 0x%08x\n", regs[i], ref_r->gpr[i]);

    }
}

void difftest_step(uint32_t pc, uint32_t npc)
{
    CPU_state ref_r;
    npc_reg_up(&cpu);
    if(skip_dut_nr_inst > 0)
    {
        ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);
        if(ref_r.pc == npc)
        {
            skip_dut_nr_inst = 0;
            checkregs(&ref_r, npc);
            return;
        }
        skip_dut_nr_inst--;
        if(skip_dut_nr_inst == 0)
            printf("can not catch up with ref.pc = 0x%08x at pc = 0x%08x", ref_r.pc, pc);
    }

    if(is_skip_ref)
    {
        ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
        is_skip_ref = false;
        return;
    }

    ref_difftest_exec(1);
    ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

    checkregs(&ref_r, pc);
}

#else
void npc_init_difftest(char *file_name, long img_size, int port){ }
#endif
