PROG = main
SRCS = ./src/*.cpp
OBJS = $(subst .cpp,.o,$(SRCS))
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g -lsfml-system -lsfml-window -lsfml-graphics

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(PROG) $(OBJS)

.PHONY: depend
depend: .depend
.depend: $(SRCS)
	rm -f ./src/.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./src/.depend;

include .depend
