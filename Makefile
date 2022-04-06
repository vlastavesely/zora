CC = gcc
RM = rm -f
SCHEMAGEN = glib-compile-schemas
INSTALL_DATA    = install -m 0644
INSTALL_PROGRAM = install -m 0755

CFLAGS = $(shell pkg-config --cflags xcb xcb-randr) -Wall
LFLAGS = $(shell pkg-config --libs xcb xcb-randr)

INDICATOR_LIBS = gtk+-3.0 ayatana-appindicator3-0.1
INDICATOR_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags $(INDICATOR_LIBS)) -I. -Wno-deprecated-declarations
INDICATOR_LFLAGS = $(LFLAGS) $(shell pkg-config --libs $(INDICATOR_LIBS))

ZORA_OBJECTS = zora/zora.o zora/randr.o zora/colourramp.o
INDICATOR_OBJECTS = zora/randr.o zora/colourramp.o indicator/zora-main.o \
	indicator/zora-indicator.o indicator/zora-control.o              \
	indicator/zora-dock.o


all: zora/zora indicator/zora-indicator data/gschemas.compiled
#	GSETTINGS_SCHEMA_DIR=data
	indicator/zora-indicator
#	zora/zora -o 1600
#	sleep 1
#	zora/zora -x

include $(wildcard */*.d)

zora/zora: $(ZORA_OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

indicator/zora-indicator: $(INDICATOR_OBJECTS)
	$(CC) $^ -o $@ $(INDICATOR_LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

indicator/%.o: indicator/%.c
	$(CC) -MMD -MP -c $< -o $@ $(INDICATOR_CFLAGS)

data/gschemas.compiled: data/com.vlastavesely.zora.gschema.xml
	$(SCHEMAGEN) data

install:
	$(INSTALL_PROGRAM) zora/zora /usr/bin
	$(INSTALL_DATA) data/com.vlastavesely.zora.gschema.xml /usr/share/glib-2.0/schemas
	$(SCHEMAGEN) /usr/share/glib-2.0/schemas

uninstall:
	$(RM) /usr/bin/zora

clean:
	$(RM) zora/zora indicator/zora-indicator */*.o */*.d data/*.compiled
