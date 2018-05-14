CC		= gcc

CFLAGS		= -g -Wall -pedantic

LDFLAGS		= -g

OUT		= list_malloc

OBJS		= list_malloc.o list.o

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)
	./list_malloc

clean:
	rm -f *.o sh core out
