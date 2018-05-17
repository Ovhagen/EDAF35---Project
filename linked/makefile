CC		= gcc

CFLAGS		= -g -Wall -pedantic

LDFLAGS		= -g

OUT		= malloc

OBJS		= malloc.o list.o

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)
	./malloc

clean:
	rm -f *.o sh core out
