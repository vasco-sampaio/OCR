CC = gcc
# asked options
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -Werror -pedantic-errors -std=c99 -O2 -g -I/usr/include/json-c/ -I../preprocessing/
LDLIBS = -lm -ljson-c `pkg-config --libs sdl` -lSDL_image

LDFLAGS = -g -ljson-c


all: neural_main.o neunet.o utils.o load_set.o pixel_functions.o neuralIO.o



clean:
	${RM} main
	${RM} *.json
	${RM} *.d
	${RM} *.o
	${RM} vgcore.*
	${RM} test.nn

# END
