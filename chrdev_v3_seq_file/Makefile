obj-m += chardev.o

# Debug Message Switcher
DEBUG = y
ifeq ($(DEBUG),y)
	DEBFLAGS = -O -g -DMYDEBUG
else
	DEBFLAGS = -O2
endif

ccflags-y += $(DEBFLAGS)

#KDIR = /usr/src/linux-headers-3.8.0-42-generic
KDIR = /usr/src/linux-headers-$(shell uname -r)

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
