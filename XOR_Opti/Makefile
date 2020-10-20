CC = gcc
# asked options
CFLAGS = -Wall -Wextra -Werror -pedantic-errors -std=c99 -O2 -g -I/usr/include/json-c/ 
# grant access to the math.h lib
LDLIBS = -lm -ljson-c

LDFLAGS = -g -ljson-c

xor: nn.o matrix.o xor.o train.o utility.o neuralIO.o

clean:
	${RM} xor
	${RM} nn.o matrix.o train.o utility.o neuralIO.o xor.o
	${RM} *.json
# END
