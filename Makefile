CC = clang
CFLAGS = -Wall -Wvla -Werror -gdwarf-4

.PHONY: all clean

all:
	$(CC) $(CFLAGS) -c Mset.c

clean:
	rm -f *.o