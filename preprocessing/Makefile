#Makefile to test grayscale

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: pixel_functions.o preprocessing.o main1.o

pixel_functions.o: pixel_functions.c pixel_functions.h

preprocessing.o: preprocessing.c pixel_functions.h

main1.o: main1.c pixel_functions.h preprocessing.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} *.bmp


# END
