CC=gcc
INCLUDES= -I/usr/include/upnp  -I../include
LIBS= -lpthread -lupnp -lixml -lthreadutil -L../libs
FILES= main.o gatedevice.o pmlist.o util.o config.o

CFLAGS += -Wall -g

ifdef HAVE_LIBIPTC
LIBS += -liptc
INCLUDES += -DHAVE_LIBIPTC
FILES += iptc.o
endif

all: upnpd

upnpd: $(FILES)
	$(CC) $(CFLAGS) $(FILES) $(LIBS) -o  $@ 
	@echo "make $@ finished on `date`"

%.o:	%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o upnpd

install: upnpd
	@install -d /etc/linuxigd
	@install etc/gatedesc.xml /etc/linuxigd
	@install etc/gateconnSCPD.xml  /etc/linuxigd
	@install etc/gateicfgSCPD.xml /etc/linuxigd
	@install etc/dummy.xml /etc/linuxigd
	@install upnpd /usr/sbin
	@install etc/upnpd.conf /etc
