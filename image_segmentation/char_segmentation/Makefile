#Makefile to test grayscale

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: pixel_functions.o line_seg.o main.o

pixel_functions.o: pixel_functions.c pixel_functions.h

line_seg.o: line_seg.c pixel_functions.h

main.o: main.c pixel_functions.h line_seg.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} line_seg.bmp


# END
