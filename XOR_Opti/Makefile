CC = gcc
# asked options
CFLAGS = -Wall -Wextra -pedantic-errors -std=c99 -O2 -g
# grant access to the math.h lib
LDLIBS = -lm

LDFLAGS = -g

main: nn.o matrix.o main.o train.o utility.o neuralIO.o

clean:
	${RM} main
	${RM} main.o nn.o matrix.o train.o utility.o neuralIO.o
	${RM} *.nn
# END
