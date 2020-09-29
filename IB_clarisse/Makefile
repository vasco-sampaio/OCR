# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: grayscale

grayscale: pixel_functions.o grayscale.o

pixel_functions.o: pixel_functions.c 

grayscale.o: grayscale.c pixel_functions.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} grayscale

# END
