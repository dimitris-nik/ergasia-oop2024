# Makefile for oop24 project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I include

# Directories
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Executable name
TARGET = oop24

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# Run Target
run: $(TARGET)
	$(TARGET) files/categories.txt files/products.txt files/users.txt
.PHONY: all clean run