
CC = gcc
# asked options
CFLAGS = -Wall -Wextra -std=c99 -O2
# grant access to the math.h lib
LDLIBS = -lm 

main: neuralNetwork.o utility.o main.o backpropagation.o

clean:
	${RM} main
	${RM} neuralNetwork.o utility.o main.o backpropagation.o
# END
