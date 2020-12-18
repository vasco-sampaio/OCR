#Makefile to test grayscale

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm
EXEC=main

all: $(EXEC)

main: pixel_operations.o illum_compensation.o main.o 

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *~
	${RM} *.bmp


# END
