CC=gcc
INCLUDES= -I/usr/include/upnp 
LIBS= -lpthread -lupnp -lixml -lthreadutil

CFLAGS += -Wall -g $(OPT)

all: upnpd

upnpd: main.o gatedevice.o sample_util.o pmlist.o util.o
	$(CC)  $(CFLAGS) main.o gatedevice.o sample_util.o pmlist.o util.o $(LIBS) -o  $@ 
	@echo "make $@ finished on `date`"

%.o:	%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o upnpd

install: upnpd
	@install -d /etc/linuxigd
	@install etc/* /etc/linuxigd
	@install upnpd /usr/sbin

