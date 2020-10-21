#Makefile to test grayscale

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: pixel_functions.o preprocessing.o main.o

pixel_functions.o: pixel_functions.c pixel_functions.h

preprocessing.o: preprocessing.c pixel_functions.h

main.o: main.c pixel_functions.h preprocessing.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} test.bmp


# END
