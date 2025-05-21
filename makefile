CFLAGS= -Wall -Wextra -std=c11 -pedantic -g

heap:alloc.c
	$(CC) $(CFLAGS) -o heap alloc.c