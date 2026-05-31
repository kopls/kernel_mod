PWD = $(shell pwd)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

SRC_DIR = $(PWD)/src
BUILD_DIR = $(PWD)
BUILD_FILES = $(SRC_DIR)/*.ko

kbuild:
	$(MAKE) -C $(KERNEL_DIR) M=$(SRC_DIR) modules
	mv $(BUILD_FILES) $(BUILD_DIR)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(SRC_DIR) clean

.PHONY: kbuild clean format
