CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

OBJS = $(BUILD_DIR)/module1_staff.o $(BUILD_DIR)/module2_triage.o

TARGET = kch_phase1
TEST_TARGET = test_runner

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/module1_staff.o: $(SRC_DIR)/module1_staff.cpp include/module1_staff.h \vert{}$(BUILD_DIR)
	$(CXX)$(CXXFLAGS) -c $(SRC_DIR)/module1_staff.cpp -o$(BUILD_DIR)/module1_staff.o

$(BUILD_DIR)/module2_triage.o: $(SRC_DIR)/module2_triage.cpp include/module2_triage.h \vert{}$(BUILD_DIR)
	$(CXX)$(CXXFLAGS) -c $(SRC_DIR)/module2_triage.cpp -o$(BUILD_DIR)/module2_triage.o

$(TARGET): $(OBJS)$(SRC_DIR)/phase1_main.cpp | $(BUILD_DIR)$(CXX) $(CXXFLAGS)$(OBJS) $(SRC_DIR)/phase1_main.cpp -o$(TARGET)

tests: $(TEST_TARGET)
	./$(TEST_TARGET)

test: tests

$(TEST_TARGET): $(OBJS)$(TEST_DIR)/test_phase1.cpp | $(BUILD_DIR)$(CXX) $(CXXFLAGS)$(OBJS) $(TEST_DIR)/test_phase1.cpp -o$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)$(TEST_TARGET)

.PHONY: all clean tests test