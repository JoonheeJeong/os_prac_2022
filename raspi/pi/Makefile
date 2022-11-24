TARGETS = led1_blink

all : $(TARGETS)

led1_blink : led1_blink.o
	gcc $< -o $@

%.o : %.c
	gcc -c $<

clean:
	rm -rf $(TARGETS) *.o
