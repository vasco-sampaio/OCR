
CC = gcc
# asked options
CFLAGS = -Wall -Wextra -std=c99 -O2
# grant access to the math.h lib
LDLIBS = -lm 
LDFLAGS = -g

main: utility.o forwardProp.o neuralNet.o backProp.o train.o

clean:
	${RM} main
	${RM} utility.o forwardProp.o neuralNet.o backProp.o train.o
# END
