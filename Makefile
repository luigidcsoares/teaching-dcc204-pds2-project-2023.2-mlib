CXX := g++
CXXFLAGS := -std=c++17 -Wall -fPIC

INCLUDE := -I include/
DOCTEST += -I ../ # doctest (não é o melhor lugar...)

SRC_DIR := src
TEST_DIR := test

BIN_DIR := bin
BUILD_DIR := build

STORAGE_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))users

define fake_inbox =
- from: "someone0"
  to: "fake-user"
  subject: "subject0"
  content: "received0"
- from: "someone1"
  to: "fake-user"
  subject: "subject1"
  content: "received1"
endef
export fake_inbox

define fake_sent =
- from: "fake-user"
  to: "someone0"
  subject: "subject0"
  content: "sent0"
- from: "fake-user"
  to: "someone1"
  subject: "subject1"
  content: "sent1"
endef
export fake_sent

SRC_FILES := $(wildcard $(SRC_DIR)/*/*.cpp)
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp $(TEST_DIR)/*/*.cpp)

DEP_FILES := $(SRC_FILES:.cpp=.d)
DEP_FILES += $(TEST_FILES:.cpp=.d)

OBJ_SRC_FILES := $(SRC_FILES:.cpp=.o)
OBJ_TEST_FILES := $(OBJ_SRC_FILES)
OBJ_TEST_FILES += $(TEST_FILES:.cpp=.o)

-include $(addprefix $(BUILD_DIR)/,$(DEP_FILES))

build: $(BUILD_DIR)/libmail.so

$(BUILD_DIR)/libmail.so: $(addprefix $(BUILD_DIR)/,$(OBJ_SRC_FILES))
	@echo "🔧 Preparing library ..."
	@mkdir -p $(BUILD_DIR)
	@$(CXX) -shared $^ -o $(BUILD_DIR)/libmail.so

test: $(BIN_DIR)/test
	@$(BIN_DIR)/test

$(BIN_DIR)/test: $(addprefix $(BUILD_DIR)/,$(OBJ_TEST_FILES))
	@echo "🔧 Preparing test suite ..."
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "🏛️ Building $< ..."
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -D STORAGE_DIR=\"$(STORAGE_DIR)\" -MMD -MP -c $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo "🏛️ Building $< ..."
	@mkdir -p $(dir $@)
	@mkdir -p $(STORAGE_DIR)/fake-load
	@echo "$$fake_inbox" > $(STORAGE_DIR)/fake-load/inbox.yaml
	@echo "$$fake_sent" > $(STORAGE_DIR)/fake-load/sent.yaml
	@$(CXX) $(CXXFLAGS) $(INCLUDE) $(DOCTEST) -D STORAGE_DIR=\"$(STORAGE_DIR)\" -MMD -MP -c $< -o $@

clean:
	@echo "🧹 Cleaning ..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
