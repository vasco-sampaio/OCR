#Makefile to test grayscale

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image
EXEC=main

all: $(EXEC)

main: pixel_functions.o preprocessing.o main1.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *~
	${RM} main1
	${RM} *.bmp


# END
