# Target output
OUTPUT = rtl_synth



# Native Compiler
CXX_X86 = g++
# Directories
INCLUDE_DIR = -I./src -I./src/benchmarks
SRC_DIR = ./src



# Compilation flags
CXXFLAGS = $(INCLUDE_DIR) -I$(DTL_DIR) -O1 -g -D __aarch64__ -std=c++17 -pthread

# C++ source files
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) 

# Object files
OBJS = $(CPP_SOURCES:.cpp=.o)

# Default target
all:  $(OUTPUT)

# Link final binary
$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -L$(DTL_DIR) -ldtl -o $@


run: $(OUTPUT)
	chmod +x ./$(OUTPUT)
	./$(OUTPUT)



# Compile .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm ./$(SRC_DIR)/*.o