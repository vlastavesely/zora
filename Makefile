PROGNAME = zora
PROG_CFLAGS = -Wall
PROG_LFLAGS = $(shell pkg-config --cflags --libs xcb xcb-randr)
OBJECTS = main.o randr.o colourramp.o

CC = gcc
RM = rm -f

all:

include $(wildcard *.d)

all: $(PROGNAME)
	./$(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) $^ -o $@ $(PROG_LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(PROG_CFLAGS)

clean:
	$(RM) $(PROGNAME) *.o *.d
