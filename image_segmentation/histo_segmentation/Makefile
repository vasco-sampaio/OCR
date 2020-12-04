#Makefile for image segmentation

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: structures.o pixel_functions.o utility.o segmentation.o matrix_letters.o main.o

structures.o: structures.c structures.h

pixel_functions.o: pixel_functions.c pixel_functions.h structures.h

utility.o: utility.c utility.h pixel_functions.h structures.h

segmentation.o: segmentation.c pixel_functions.h structures.h utility.h

matrix_letters.o: matrix_letters.c matrix_letters.h structures.h

main.o: main.c pixel_functions.h segmentation.h structures.h matrix_letters.h utility.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} *.bmp


# END
