CC = gcc
RM = rm -f
INSTALL_DATA    = install -m 0644
INSTALL_PROGRAM = install -m 0755

CFLAGS = $(shell pkg-config --cflags xcb xcb-randr) -Wall
LFLAGS = $(shell pkg-config --libs xcb xcb-randr)

INDICATOR_LIBS = gtk+-3.0 ayatana-appindicator3-0.1
INDICATOR_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags $(INDICATOR_LIBS)) -I.
INDICATOR_LFLAGS = $(LFLAGS) $(shell pkg-config --libs $(INDICATOR_LIBS))

ZORA_OBJECTS = zora/zora.o zora/randr.o zora/colourramp.o
INDICATOR_OBJECTS = zora/randr.o zora/colourramp.o indicator/main.o	\
	indicator/indicator.o indicator/dock.o


all: zora/zora indicator/zora-indicator
#	zora/zora -o 1600
#	sleep 1
#	zora/zora -x
	indicator/zora-indicator

include $(wildcard */*.d)

zora/zora: $(ZORA_OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

indicator/zora-indicator: $(INDICATOR_OBJECTS)
	$(CC) $^ -o $@ $(INDICATOR_LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

indicator/%.o: indicator/%.c
	$(CC) -MMD -MP -c $< -o $@ $(INDICATOR_CFLAGS)

install:
	$(INSTALL_PROGRAM) zora/zora /usr/bin

uninstall:
	$(RM) /usr/bin/zora

clean:
	$(RM) zora/zora */*.o */*.d
