PWD = $(shell pwd)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

SRC_DIR = $(PWD)
BUILD_DIR = $(PWD)
BUILD_FILES = $(wildcard $(SRC_DIR)/*.ko)

CLANG_FORMAT_VERS ?= 19
CLANG_FORMAT = clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES = $(SRC_DIR)/*.c

$(shell mkdir -p $(BUILD_DIR))

kbuild:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules V=1
	mv $(BUILD_FILES) $(BUILD_DIR)/.

format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

.PHONY: kbuild clean format
