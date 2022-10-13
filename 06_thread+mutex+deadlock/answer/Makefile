outputs = mutex-01-ans mutex-hw1-ans mutex-hw2-ans \
		  circular_wait hold-and-wait no_preemption

all : $(outputs)

clean:
	rm -f $(outputs)

CC = gcc

CFLAGS = -Wall -pthread -o

mutex-01-ans: mutex-01-ans.c
	$(CC) $(CFLAGS) $@ $<
mutex-hw1-ans: mutex-hw1-ans.c
	$(CC) $(CFLAGS) $@ $<
mutex-hw2-ans: mutex-hw2-ans.c
	$(CC) $(CFLAGS) $@ $<

circular_wait: circular_wait.c
	$(CC) $(CFLAGS) $@ $<
hold-and-wait: hold-and-wait.c
	$(CC) $(CFLAGS) $@ $<
no_preemption: no_preemption.c
	$(CC) $(CFLAGS) $@ $<

