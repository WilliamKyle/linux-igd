CC=g++
INCLUDES= -I/usr/include/upnp 
LIBS= -lpthread  /usr/lib/libupnp.so
OBJ=gate.o gateway.o sample_util.o ipcon.o portmap.o pmlist.o config.o

DEBUG=0
USE_CURSES=1

ifeq ($(DEBUG),1)
OPT = -g
else
OPT = -O2
endif

ifeq ($(USE_CURSES),1)
LIBS += -lncurses
OBJ += interactive.o
CFLAGS += -DUSE_CURSES
endif

CFLAGS += -Wall $(OPT)

APPS = upnpd

all: $(APPS)

upnpd:  $(OBJ)
	$(CC)  $(CFLAGS) $(OBJ) $(LIBS) -o  $@ 
	@echo "make $@ finished on `date`"

%.o:	%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o $(APPS)

install: upnpd
	@install -d /etc/linuxigd
	@install etc/* /etc/linuxigd
	@install upnpd /usr/bin

