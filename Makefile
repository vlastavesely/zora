PROGNAME = zora
PROG_CFLAGS = -Wall
PROG_LFLAGS = $(shell pkg-config --cflags --libs xcb xcb-randr)
OBJECTS = zora.o randr.o colourramp.o

CC = gcc
RM = rm -f
INSTALL_PROGRAM = install -m 0755

all:

include $(wildcard *.d)

all: $(PROGNAME)
	./$(PROGNAME) -o 1600
	sleep 1
	./$(PROGNAME) -x

$(PROGNAME): $(OBJECTS)
	$(CC) $^ -o $@ $(PROG_LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(PROG_CFLAGS)

install:
	$(INSTALL_PROGRAM) $(PROGNAME) /usr/bin

uninstall:
	$(RM) /usr/bin/$(PROGNAME)

clean:
	$(RM) $(PROGNAME) *.o *.d
