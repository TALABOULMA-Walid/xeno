# ----------------------------------------------------------------------
# Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
# (C) 2012 - Christophe BLAESS
# http://christophe.blaess.fr
# ----------------------------------------------------------------------

APPLICATIONS =                       \
	exemple-hello-01             \
	exemple-hello-02             \
	exemple-hello-03             \
	exemple-periodique-01        \
	exemple-periodique-02        \
	exemple-periodique-03        \
	exemple-periodique-04        \
	exemple-alarme-01            \
	exemple-watchdog-01          \
	exemple-mutex-01             \
	exemple-mutex-02             \
	exemple-semaphore-01         \



XENOCONFIG=/usr/xenomai/bin/xeno-config
#XENOCONFIG=~/Projets/Panda/xenomai/bin/xeno-config

CC=$(shell      $(XENOCONFIG) --cc)
MY_CFLAGS=-g
CFLAGS=$(shell  $(XENOCONFIG) --skin=native --cflags)  $(MY_CFLAGS)
LDFLAGS=$(shell $(XENOCONFIG) --skin=native --ldflags) $(MY_LDFLAGS)
LIBDIR=$(shell  $(XENOCONFIG) --skin=native --libdir)  $(MY_LIBDIR)
LDFLAGS+=-lnative 
LDLIBS=-lnative -lxenomai

all:: $(APPLICATIONS)

clean::
	$(RM) -f $(APPLICATIONS) *.o *~
