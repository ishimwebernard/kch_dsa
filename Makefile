CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -I include

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

OBJS = $(BUILD_DIR)/module1_staff.o \
       $(BUILD_DIR)/module2_triage.o \
       $(BUILD_DIR)/module3_appointments.o \
       $(BUILD_DIR)/module4_records.o

TARGET = kch_pmts
TEST_TARGET = test_runner

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/module1_staff.o: $(SRC_DIR)/module1_staff.cpp include/module1_staff.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/module2_triage.o: $(SRC_DIR)/module2_triage.cpp include/module2_triage.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/module3_appointments.o: $(SRC_DIR)/module3_appointments.cpp include/module3_appointments.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/module4_records.o: $(SRC_DIR)/module4_records.cpp include/module4_records.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(SRC_DIR)/main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) $(SRC_DIR)/main.cpp -o $(TARGET)

$(TEST_TARGET): $(OBJS) $(TEST_DIR)/test_runner.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) $(TEST_DIR)/test_runner.cpp -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

tests: test

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test tests