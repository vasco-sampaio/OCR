#Makefile for image segmentation

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: pixel_functions.o segmentation.o main.o

pixel_functions.o: pixel_functions.c pixel_functions.h

segmentation.o: segmentation.c pixel_functions.h

main.o: main.c pixel_functions.h segmentation.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} line_seg.bmp


# END
