obj-m := driver.o
KDIR := /home/kyuwon/embeded/linux

default:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KDIR) M=$(shell pwd) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

version:
	echo $(shell pwd)