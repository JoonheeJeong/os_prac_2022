obj-m := led1_gpio_driver.o
KDIR := $(HOME)/rpi/linux
PWD := $(shell pwd)

default:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- \
	-C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
