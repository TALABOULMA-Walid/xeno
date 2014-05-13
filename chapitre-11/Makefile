# ----------------------------------------------------------------------
# Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
# (C) 2012 - Christophe BLAESS
# http://christophe.blaess.fr
# ----------------------------------------------------------------------

ifneq (${KERNELRELEASE},)
	obj-m += exemple-squelette-module.o
	obj-m += exemple-read-write.o
	obj-m += exemple-read-irq.o
	obj-m += exemple-read-threadirq.o
	obj-m += exemple-rtdm-read-write.o
	obj-m += exemple-rtdm-irq.o

EXTRA_CFLAGS := -I /usr/xenomai/include/
else
	ARCH ?=
	CROSS_COMPILE ?=
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

	MODULE_DIR := $(shell pwd)
	CFLAGS := -Wall -g 

.PHONY: all
all:: modules executable

.PHONY: modules
modules:
	${MAKE} -C ${KERNEL_DIR} SUBDIRS=${MODULE_DIR}  modules

.PHONY: executable
executable: exemple-user-rtdm-read exemple-user-rtdm-write


XENOCONFIG=/usr/xenomai/bin/xeno-config

exemple-irq-xenomai: exemple-irq-xenomai.c
	$(shell $(XENOCONFIG) --cc) $(shell $(XENOCONFIG) --skin=native --cflags) $(shell $(XENOCONFIG) --skin=native --ldflags) -L $(shell  $(XENOCONFIG) --skin=native --libdir) -o exemple-irq-xenomai exemple-irq-xenomai.c -lnative -lxenomai

exemple-user-rtdm-read: exemple-user-rtdm-read.c
	$(shell $(XENOCONFIG) --cc) $(shell $(XENOCONFIG) --skin=rtdm --cflags) $(shell $(XENOCONFIG) --skin=rtdm --ldflags) -L $(shell  $(XENOCONFIG) --skin=rtdm --libdir) -o exemple-user-rtdm-read exemple-user-rtdm-read.c -lrtdm -lxenomai

exemple-user-rtdm-write: exemple-user-rtdm-write.c
	$(shell $(XENOCONFIG) --cc) $(shell $(XENOCONFIG) --skin=rtdm --cflags) $(shell $(XENOCONFIG) --skin=rtdm --ldflags) -L $(shell  $(XENOCONFIG) --skin=rtdm --libdir) -o exemple-user-rtdm-write exemple-user-rtdm-write.c -lrtdm -lxenomai

.PHONY: clean
clean::
	rm -f  *.o  .*.o  .*.o.* *.ko  .*.ko  *.mod.* .*.mod.* .*.cmd *~
	rm -f Module.symvers Module.markers modules.order 
	rm -rf .tmp_versions
	rm -f exemple-irq-xenomai exemple-user-rtdm-read exemple-user-rtdm-write
endif
