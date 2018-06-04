CC = g++
CPP_FLAGS = -I include -I Alpha-Reversi -std=c++11
VPATH = src include Alpha-Reversi Alpha-Reversi/core
OBJS = main.o mcts.o minmax.o mc_minmax.o battlemain.o uct.o reversi.o

.PHONY: all
all: $(OBJS)
	$(CC) $(CPP_FLAGS) -o reversi.out $^

%.o: %.cpp
	$(CC) $(CPP_FLAGS) -c -o $@ $<

include $(OBJS:%.o=%.d)

%.d: %.cpp
	@echo Generating dependencies for $<
	@set -e;\
	$(CC) $(CPP_FLAGS) -MM $< > $@.tmp;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@; \
	rm -f $@.tmp

.PHONY: clean
clean:
	rm -f *.out *.o *.d
