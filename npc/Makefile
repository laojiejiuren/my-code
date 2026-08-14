TOPNAME = top
VERILATOR = verilator

include src/utils/filelist.mk

# ------- minirv -------

BUILD_DIR_MINIRV = ./build/minirv
OBJ_DIR_MINIRV = $(BUILD_DIR_MINIRV)/obj_dir
BIN_MINIRV = $(OBJ_DIR_MINIRV)/V$(TOPNAME)

VSRCS_MINIRV = $(shell find $(abspath ./minirv) -name "*.v")

VERILATOR_FLAGS_MINIRV = --cc --exe --build -j \
				  -O3 -x-assign fast -x-initial fast --noassert \
				  --top-module $(TOPNAME) \
				  -Mdir $(OBJ_DIR_MINIRV)

# ------- riscv32e -------

BUILD_DIR_RV32E = ./build/RV32E
OBJ_DIR_RV32E = $(BUILD_DIR)/obj_dir
BIN_RV32E = $(OBJ_DIR_RV32E)/V$(TOPNAME)

VSRCS_RV32E = $(shell find $(abspath ./riscv32e) -name "*.v")

VERILATOR_FLAGS_RV32E = --cc --exe --build -j \
				  -O3 -x-assign fast -x-initial fast --noassert \
				  --top-module $(TOPNAME) \
				  -Mdir $(OBJ_DIR_RV32E)

# ------- 通用 -------

INC_DIR = $(abspath ./include)
CSRCS = $(shell find $(abspath ./csrc) -name "*.cpp") $(shell find $(abspath ./src) -name "*.c" -or -name "*.cpp")

INC_PATH = $(INC_DIR)
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"$(TOPNAME)"\" -g 

default:$(BIN_MINIRV)
DIFF_REF_SO = $(NEMU_HOME)/build/riscv32-nemu-interpreter-so
ELF_FILE = $(addsuffix .elf, $(basename $(IMG)))
ARGS = $(ELF_FILE) $(DIFF_REF_SO) 
NPC_EXEC := $(IMG) $(ARGS)

# ------- minirv依赖 -------

$(BIN_MINIRV):$(VSRCS_MINIRV) $(CSRCS)
	@mkdir -p $(BUILD_DIR_MINIRV)
	$(VERILATOR) $(VERILATOR_FLAGS_MINIRV) \
	$(VSRCS_MINIRV) $(CSRCS) \
	-CFLAGS "$(CXXFLAGS) $(CFLAGS)" -LDFLAGS "-lreadline"
	@echo "Build minirv finished: $@"

run_minirv:$(BIN_MINIRV)
	$(call git_commit, "run RTL") # DO NOT REMOVE THIS LINE!!!
	@$^ $(NPC_EXEC)

gdb_minirv:$(BIN_MINIRV)
	$(call git_commit, "run GDB")
	gdb -s $(ELF_FILE) --args $(BIN_MINIRV) $(NPC_EXEC)

# ------- RV32E依赖 -------

$(BIN_RV32E):$(VSRCS_RV32E) $(CSRCS)
	@mkdir -p $(BUILD_DIR_RV32E)
	$(VERILATOR) $(VERILATOR_FLAGS_RV32E) \
	$(VSRCS_RV32E) $(CSRCS) \
	-CFLAGS "$(CXXFLAGS) $(CFLAGS)" -LDFLAGS "-lreadline"
	@echo "Build riscv32e finished: $@"

run_riscv32e:$(BIN_RV32E)
	$(call git_commit, "run RTL") # DO NOT REMOVE THIS LINE!!!
	@$^ $(NPC_EXEC)

gdb_riscv32e:$(BIN_RV32E)
	$(call git_commit, "run GDB")
	gdb -s $(ELF_FILE) --args $(BIN_RV32E) $(NPC_EXEC)

clean:
	rm -rf ./build

.PHONY: clean run all
