TOPNAME = top
VERILATOR = verilator

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = ./build/obj_dir/V$(TOPNAME)
INC_DIR = ./csrc/include

VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.cpp")

VERILATOR_FLAGS = --cc --exe --build -j \
				  -O3 --x--assign fast --x-initial fast --noassert \
				  --top-module $(TOPNAME) \
				  -Mdir $(OBJ_DIR)

INC_PATH = $(INC_DIR)
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGX += $(INCFLAGS) -DTOP_NAME="\"$(TOPNAME)"\"

default:$(BIN)

$(BIN):$(VSRCS) $(CSRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) \
	$(VSRCS) $(CSRCS) \
	-CFLAGS "&CXXFLAGS"
	@echo "Build finished: $@"

run:$(BIN)
	$(call git_commit, "run RTL") # DO NOT REMOVE THIS LINE!!!
	@$^

clean:
	rm -rf ./build

.PHONY: clean run all
