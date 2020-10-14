#Makefile to test grayscale

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: pixel_functions.o char_seg.o main.o

pixel_functions.o: pixel_functions.c pixel_functions.h

char_seg.o: char_seg.c pixel_functions.h

main.o: main.c pixel_functions.h char_seg.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} char_seg.bmp


# END
