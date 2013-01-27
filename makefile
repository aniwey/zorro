PROG = main
OBJ_DIR = obj
SRC_DIR = src
SRCS = *.cpp
OBJS = *.o
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g -lsfml-system -lsfml-window -lsfml-graphics

$(PROG): $(OBJ_DIR)/$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJ_DIR)/$(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(PROG) $(OBJ_DIR)/$(OBJS)

.PHONY: depend
depend: .depend
.depend: $(SRC_DIR)/$(SRCS)
	rm -f ./src/.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./src/.depend;

include .depend
