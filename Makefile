CC = gcc
RM = rm -f
SCHEMAGEN = glib-compile-schemas
INSTALL_DATA    = install -m 0644
INSTALL_PROGRAM = install -m 0755

CFLAGS = $(shell pkg-config --cflags xcb xcb-randr) -Wall
LFLAGS = $(shell pkg-config --libs xcb xcb-randr)

INDICATOR_LIBS = gtk+-3.0 ayatana-appindicator3-0.1
INDICATOR_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags $(INDICATOR_LIBS)) -I.
INDICATOR_LFLAGS = $(LFLAGS) $(shell pkg-config --libs $(INDICATOR_LIBS))

# GtkStatusIcon is deprecated in Gtk-3.0
INDICATOR_CFLAGS += -Wno-deprecated-declarations

ZORA_OBJECTS = zora/zora.o zora/randr.o zora/colourramp.o
INDICATOR_OBJECTS = zora/randr.o zora/colourramp.o indicator/zora-main.o  \
	indicator/zora-application.o indicator/zora-indicator.o           \
	indicator/zora-control.o indicator/zora-dock.o


all: zora/zora indicator/zora-indicator data/gschemas.compiled data/zora.desktop

run: all
	GSETTINGS_SCHEMA_DIR=data indicator/zora-indicator

include $(wildcard */*.d)

zora/zora: $(ZORA_OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

indicator/zora-indicator: $(INDICATOR_OBJECTS)
	$(CC) $^ -o $@ $(INDICATOR_LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

indicator/%.o: indicator/%.c
	$(CC) -MMD -MP -c $< -o $@ $(INDICATOR_CFLAGS)

data/zora.desktop: data/zora.desktop.in
	cat $< >$@

data/gschemas.compiled: data/com.vlastavesely.zora.gschema.xml
	$(SCHEMAGEN) data

install-files:
	$(INSTALL_PROGRAM) zora/zora indicator/zora-indicator /usr/bin
	$(INSTALL_PROGRAM) data/zora.desktop /usr/share/applications
	$(INSTALL_DATA) data/com.vlastavesely.zora.gschema.xml /usr/share/glib-2.0/schemas
	$(INSTALL_DATA) data/zora.svg data/zora-disabled.svg /usr/share/icons/hicolor/scalable/apps
	$(SCHEMAGEN) /usr/share/glib-2.0/schemas
	ln -sf /usr/share/applications/zora.desktop /etc/xdg/autostart/zora.desktop

install: install-files
	gtk-update-icon-cache /usr/share/icons/hicolor

uninstall-files:
	$(RM) /usr/bin/zora /usr/bin/zora-indicator
	$(RM) /usr/share/applications/zora.desktop
	$(RM) /usr/share/icons/hicolor/scalable/apps/zora*.svg
	$(RM) /usr/share/glib-2.0/schemas/com.vlastavesely.zora.gschema.xml
	unlink /etc/xdg/autostart/zora.desktop

uninstall: uninstall-files
	gtk-update-icon-cache /usr/share/icons/hicolor

clean:
	$(RM) zora/zora indicator/zora-indicator */*.o */*.d data/*.compiled data/*.desktop
