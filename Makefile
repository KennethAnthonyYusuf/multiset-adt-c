CC = clang
CFLAGS = -Wall -Wvla -Werror

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

.PHONY: all
all: testMset

testMset: testMset.c Mset.c Mset.h MsetStructs.h
	$(CC) $(CFLAGS) -o testMset testMset.c Mset.c

.PHONY: clean
clean:
	rm -f testMset