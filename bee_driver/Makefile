ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /home/linux/workdir/kernel/linux-3.0.15/
PWD := $(shell pwd)
CC = arm-none-linux-gnueabi-gcc

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* module*

.PHONY: modules modules_install clean

else
    obj-m := fs4412_bee.o
endif

