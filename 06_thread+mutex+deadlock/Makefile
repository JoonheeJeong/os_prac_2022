outputs = a b \
		  mutex-01 mutex-hw1 mutex-hw2 \
		  deadlock 

all : $(outputs)

clean:
	rm -f $(outputs)

CC = gcc

CFLAGS = -Wall -pthread -o

a: thread_create.c
	$(CC) $(CFLAGS) $@ $<
b: thread_create_with_return_args.c
	$(CC) $(CFLAGS) $@ $<

mutex-01: mutex-01.c
	$(CC) $(CFLAGS) $@ $<
mutex-hw1: mutex-hw1.c
	$(CC) $(CFLAGS) $@ $<
mutex-hw2: mutex-hw2.c
	$(CC) $(CFLAGS) $@ $<

deadlock: deadlock.c
	$(CC) $(CFLAGS) $@ $<

