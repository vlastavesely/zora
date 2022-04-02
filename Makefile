CC = gcc
RM = rm -f
INSTALL_DATA    = install -m 0644
INSTALL_PROGRAM = install -m 0755

CFLAGS = $(shell pkg-config --cflags xcb xcb-randr) -Wall
LFLAGS = $(shell pkg-config --libs xcb xcb-randr)

ZORA_OBJECTS = zora/zora.o zora/randr.o zora/colourramp.o


all: zora/zora
	zora/zora -o 1600
	sleep 1
	zora/zora -x

include $(wildcard */*.d)

zora/zora: $(ZORA_OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CFLAGS)

install:
	$(INSTALL_PROGRAM) zora/zora /usr/bin

uninstall:
	$(RM) /usr/bin/zora

clean:
	$(RM) zora/zora */*.o */*.d
