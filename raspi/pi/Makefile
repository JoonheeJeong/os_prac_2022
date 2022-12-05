TARGETS = led1_blink btn1_led1_blink btnN_ledN_blink agility_game

all : $(TARGETS)

led1_blink : led1_blink.o
	gcc $< -o $@

btn1_led1_blink : btn1_led1_blink.o
	gcc $< -o $@

btnN_ledN_blink : btnN_ledN_blink.o
	gcc $< -o $@

agility_game: agility_game.o
	gcc $< -o $@

%.o : %.c
	gcc -c $<

clean:
	rm -rf $(TARGETS) *.o
