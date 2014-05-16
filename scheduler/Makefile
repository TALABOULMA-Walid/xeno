XENOCONFIG=/usr/xenomai/bin/xeno-config
CC=		$(shell $(XENOCONFIG) --cc)
CFLAGS= $(shell $(XENOCONFIG) --skin=native --cflags)
LDFLAGS=$(shell $(XENOCONFIG) --skin=native --ldflags)
LDFLAGS+=-lnative
LDLIBS=-lnative -lxenomai


all:: scheduler
clean::
	rm -f scheduler *.o *~

