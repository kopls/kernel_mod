# Makefile — сценарий 1: kunit_builtin_example
#
# Использование:
#   make            — сборка модуля против текущего ядра
#   make clean      — удалить артефакты сборки
#   make load       — загрузить модуль (требует root)
#   make unload     — выгрузить модуль
#   make dmesg      — показать вывод KUnit из dmesg

KDIR  ?= /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

.PHONY: all clean load unload dmesg

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod kunit_builtin_example.ko

unload:
	sudo rmmod kunit_builtin_example

dmesg:
	sudo dmesg | grep -E "kunit|PASS|FAIL|mymath" | tail -40
