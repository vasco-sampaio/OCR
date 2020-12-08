#Makefile for image segmentation

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99 -I../../neunet_HD/ -I/usr/include/json-c 
LDFLAGS= -ljson-c -lm 
LDLIBS= `pkg-config --libs sdl` -lSDL_image -I../../neunet_HD/ 

EXEC=main

#all: $(#EXEC)

main: structures.o pixel_functions.o utility.o resizing.o segmentation.o matrix_letters.o main.o ../neunet_HD/neuralIO.o ../neunet_HD/neunet.o ../neunet_HD/utils.o 

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} *.bmp


# END
