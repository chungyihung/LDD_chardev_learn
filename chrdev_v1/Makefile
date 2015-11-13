obj-m += chardev.o

KDIR = /usr/src/linux-headers-3.8.0-42-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	#rm -rf *.o *.ko *.mod.* *.symvers *.order .*.cmd

install:
	cp -a *.ko /lib/modules/3.8.0-42-generic/kernel/drivers/char
	depmod -ae
uninstall:
	depmod -ae
