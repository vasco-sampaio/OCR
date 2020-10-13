
CC = gcc
# asked options
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
# grant access to the math.h lib
LDLIBS = -lm

LDFLAGS = -g

main: nn.o neuron.o main.o layer.o train.o

clean:
	${RM} main
	${RM} main.o nn.o layer.o neuron.o train.o
# END
