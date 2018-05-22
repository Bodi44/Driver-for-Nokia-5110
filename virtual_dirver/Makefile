obj-m += hello.o

KDIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(shell pwd) modules
clean:
	rm -rf *.o *.ko *.mod *.symvers *.order
