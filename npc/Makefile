TOPNAME = top
VERILATOR = verilator

include src/utils/filelist.mk
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = ./build/obj_dir/V$(TOPNAME)
INC_DIR = $(abspath ./include)

VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.cpp") $(shell find $(abspath ./src) -name "*.c" -or -name "*.cpp")

VERILATOR_FLAGS = --cc --exe --build -j \
				  -O3 -x-assign fast -x-initial fast --noassert \
				  --top-module $(TOPNAME) \
				  -Mdir $(OBJ_DIR)

INC_PATH = $(INC_DIR)
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"$(TOPNAME)"\" -g

default:$(BIN)

ELF_FILE = $(addsuffix .elf, $(basename $(IMG)))
NPC_EXEC := $(IMG) #$(ARGS)

$(BIN):$(VSRCS) $(CSRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) \
	$(VSRCS) $(CSRCS) \
	-CFLAGS "$(CXXFLAGS) $(CFLAGS)" -LDFLAGS "-lreadline"
	@echo "Build finished: $@"

run:$(BIN)
	$(call git_commit, "run RTL") # DO NOT REMOVE THIS LINE!!!
	@$^ $(NPC_EXEC) $(ELF_FILE)

gdb:$(BIN)
	$(call git_commit, "run GDB")
	gdb -s $(ELF_FILE) --args $(BIN) $(NPC_EXEC) $(ELF_FILE)

clean:
	rm -rf ./build

.PHONY: clean run all
