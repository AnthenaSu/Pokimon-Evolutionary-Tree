# COMP2521 - Assignment 1

# !!! DO NOT MODIFY THIS FILE !!!

CC = clang
CFLAGS = -Wall -Wvla -Werror -gdwarf-4

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,leak,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

.PHONY: all
all: testEvoTree

testEvoTree: testEvoTree.c EvoTree.c
	$(CC) $(CFLAGS) -o testEvoTree testEvoTree.c EvoTree.c

########################################################################

.PHONY: clean
clean:
	rm -f *.o testEvoTree

