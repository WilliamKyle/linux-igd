CC=g++
INCLUDES= -I/usr/include/upnp 
LIBS= -lpthread -lupnp -lixml -lthreadutil 
OBJ=gate.o gateway.o sample_util.o ipcon.o portmap.o pmlist.o config.o

DEBUG=0

ifeq ($(DEBUG),1)
OPT = -g
else
OPT = -O2
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
	@install upnpd /usr/sbin

