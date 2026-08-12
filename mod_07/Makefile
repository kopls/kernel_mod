KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

DRV_NAME = kernel_alloc

.PHONY: clean install uninstall build run remove

build:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

run:
	sudo insmod $(PWD)/$(DRV_NAME).ko

remove:
	sudo rmmod $(DRV_NAME).ko

install:
	cp $(DRV_NAME).ko /lib/modules/$(shell uname -r)
	depmode -a
	modprobe $(DRV_NAME)

uninstall:
	modprobe -r $(DRV_NAME)
	rm /lib/modules/$(shell uname -r)/$(DRV_NAME).ko
	depmode -a
