CC = gcc
# asked options
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -pedantic-errors -std=c99 -O2 -g -I/usr/include/json-c/ 
# grant access to the math.h lib
LDLIBS = -lm -ljson-c `pkg-config --libs sdl` -lSDL_image

LDFLAGS = -g -ljson-c

xor: nn.o matrix.o xor.o train.o utility.o neuralIO.o load_set.o pixel_functions.o


data_set:
	gcc load_set.c pixel_functions.c -lSDL_image -g -I/usr/include/SDL

clean:
	${RM} xor
	${RM} nn.o matrix.o train.o utility.o neuralIO.o xor.o
	${RM} *.json
# END
