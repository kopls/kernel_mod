# Makefile — сценарий 2: strutil_module + strutil_test (отдельные .ko)
#
# Использование:
#   make              — собрать оба модуля
#   make clean        — удалить артефакты
#   make load         — загрузить оба модуля (основной + тест)
#   make load-test    — загрузить только тест (если основной уже загружен)
#   make unload       — выгрузить оба
#   make dmesg        — показать вывод KUnit из dmesg

KDIR  ?= /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

.PHONY: all clean load load-test unload dmesg

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod strutil_module.ko
	sudo insmod strutil_test.ko

load-test:
	sudo insmod strutil_test.ko

unload:
	sudo rmmod strutil_test  2>/dev/null || true
	sudo rmmod strutil_module 2>/dev/null || true

dmesg:
	sudo dmesg | grep -E "kunit|PASS|FAIL|strutil" | tail -60
