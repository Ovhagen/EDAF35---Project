CC		= gcc

CFLAGS		= -g -Wall -pedantic

LDFLAGS		= -g

OUT		= test

OBJS		= test.o alloc.o

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)
	./test

clean:
	rm -f *.o sh core out
	rm -f test
