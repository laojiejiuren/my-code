TOPNAME = top
VERILATOR = verilator

VSRC_DIR = ./vsrc
CSRC_DIR = ./csrc
INC_DIR  = ./csrc/include  

BUILD_DIR = ./build
OBJ_DIR   = $(BUILD_DIR)/obj_dir
BIN       = ./build/obj_dir/Vtop

VSRCS = $(shell find $(abspath $(VSRC_DIR)) -name "*.v")
CSRCS = $(shell find $(abspath $(CSRC_DIR)) -name "*.cpp")


VERILATOR_FLAGS = --cc --exe --build -j \
                  -O3 --x-assign fast --x-initial fast --noassert \
                  --top-module $(TOPNAME) \
                  -Mdir $(OBJ_DIR)

INC_PATH = $(INC_DIR)
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""

default: $(BIN)

$(BIN): $(VSRCS) $(CSRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) \
		$(VSRCS) $(CSRCS) \
		-CFLAGS "$(CXXFLAGS)"
	@echo "Build finished: $@"


run: $(BIN)
	$(call git_commit, "minirv RTL") # DO NOT REMOVE THIS LINE!!!
	@$^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default run clean
