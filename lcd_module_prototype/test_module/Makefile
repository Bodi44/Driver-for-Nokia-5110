obj-m += project.o

KDIR = /usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
