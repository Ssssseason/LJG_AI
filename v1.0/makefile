CC = g++
CPP_FLAGS = -I include -std=c++11 -O2
LIB_FLAGS = -L/usr/local/lib -lcurl -O2
VPATH = src include
OBJS = main.o minmax.o mc_minmax.o mcts.o iteration_minmax.o net.o

.PHONY: all
all: $(OBJS)
	$(CC) -o reversi.out $^ $(LIB_FLAGS)

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
