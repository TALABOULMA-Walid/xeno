XENOCONFIG=/usr/xenomai/bin/xeno-config
CC=		$(shell $(XENOCONFIG) --cc)
CFLAGS= $(shell $(XENOCONFIG) --skin=native --cflags)
LDFLAGS=$(shell $(XENOCONFIG) --skin=native --ldflags)
LDFLAGS+=-lnative
LDLIBS=-lnative -lxenomai


all:: task_test
clean::
	rm -f task_test *.o *~

