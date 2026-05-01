# Target output
OUTPUT = rtl_synth



# CUDA
CUDA_VERSION = cuda-12.6
CUDA_HOME ?= /usr/local/$(CUDA_VERSION)/
NVCC      := $(CUDA_HOME)/bin/nvcc

CUDA_INC  := -I$(CUDA_HOME)/include
CUDA_LIB  := -L$(CUDA_HOME)/lib64 -lcudart

# GPU arch (adjust to your GPU, e.g. sm_86 for Ampere, sm_90 for Hopper)
CUDA_ARCH := -gencode arch=compute_89,code=sm_89 \
             -gencode arch=compute_89,code=compute_89



# Native Compiler
CXX_X86 = g++
# Directories
INCLUDE_DIR = -I./src -I./src/benchmarks
SRC_DIR = ./src



# Compilation flags
CXXFLAGS = $(INCLUDE_DIR) -I$(DTL_DIR) -O3 -g -D __aarch64__ -std=c++17 -pthread #-fsanitize=address -DDEBUG

# C++ source files
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) 

# Object files
OBJS = $(CPP_SOURCES:.cpp=.o)

# Default target
all:  $(OUTPUT)

# Link final binary
$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -L$(DTL_DIR) -ldtl -g -o $@


run: $(OUTPUT)
	chmod +x ./$(OUTPUT)
	./$(OUTPUT)

debug: $(OUTPUT)
	chmod +x ./$(OUTPUT)
	gdb ./$(OUTPUT)

# Compile .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm ./$(SRC_DIR)/*.o