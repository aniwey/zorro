PROG = main
OBJ_DIR = obj
SRC_DIR = src
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g -lsfml-system -lsfml-window -lsfml-graphics

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) -o $@ -c $< ${CXXFLAGS}

.PHONY: clean
clean:
	rm -f $(PROG) $(OBJS)

.PHONY: depend
depend: .depend
.depend: $(SRCS)
	rm -f ./src/.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./src/.depend;

include .depend
