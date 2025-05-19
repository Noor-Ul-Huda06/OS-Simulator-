CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -I./include
LDFLAGS = 
BUILD_DIR = build
SRC_DIR = src
TASKS_DIR = tasks
INCLUDE_DIR = include

# Main OS executable
OS_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OS_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(OS_SRCS))
OS_TARGET = os_simulator

# Tasks executables
TASK_SRCS = $(wildcard $(TASKS_DIR)/*.cpp)
TASK_TARGETS = $(patsubst $(TASKS_DIR)/%.cpp,$(BUILD_DIR)/%,$(TASK_SRCS))

all: $(OS_TARGET) tasks

$(OS_TARGET): $(OS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

tasks: $(TASK_TARGETS)

$(BUILD_DIR)/%: $(TASKS_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(OS_TARGET)

.PHONY: all clean tasks 
