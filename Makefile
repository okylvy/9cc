CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

hicc: $(OBJS)
	$(CC) -o hicc $(OBJS) $(LDFLAGS)

$(OBJS): hicc.h

test: hicc
	./test.sh

clean:
	rm -f hicc *.o *~ tmp*

.PHONY: test clean