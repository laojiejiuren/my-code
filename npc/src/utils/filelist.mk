LIBCAPSTONE = /home/lv/ysyx-workbench/npc/tools/capstone/repo/libcapstone.so.5
CFLAGS += -I /home/lv/ysyx-workbench/npc/tools/capstone/repo/include
src/utils/disasm.c: $(LIBCAPSTONE)
$(LIBCAPSTONE):
	$(MAKE) -C /home/lv/ysyx-workbench/npc/tools/capstone
