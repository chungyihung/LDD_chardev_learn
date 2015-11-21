TARGET_NAME=chardev

obj-m += $(TARGET_NAME).o

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
	cp -a $(TARGET_NAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char
	depmod -ae
uninstall:
	rm -rf /lib/modules/$(shell uname -r)/kernel/drivers/char/$(TARGET_NAME).ko
	depmod -ae
