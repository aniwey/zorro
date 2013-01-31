PROG = main
OBJ_DIR = obj
SRC_DIR = src
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g -lsfml-system -lsfml-window -lsfml-graphics -lboost_serialization

all: dir_obj $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ -c $< ${CXXFLAGS}

.PHONY: clean depend dir_obj

dir_obj:
	mkdir -p ./obj

clean:
	rm -f $(PROG) $(OBJS)

depend: $(SRCS)
	rm -f $(SRC_DIR)/.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > $(SRC_DIR)/.depend;

redo: clean depend all

-include $(SRC_DIR)/.depend
