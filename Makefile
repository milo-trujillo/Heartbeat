obj-m += hb.o
KVERSION = $(shell uname -r)
CLAGS = -fno-stack-protector -O3
V=1
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
