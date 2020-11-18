CC = gcc
# asked options
CFLAGS = -Wall -Wextra -Werror -pedantic-errors -std=c99 -g -I/usr/include/json-c/ 
# grant access to the math.h lib
LDLIBS = -lm -ljson-c

LDFLAGS = -g

main: neuralNet.o utils.o forProp.o backprop.o neuralTraining.o

clean:
	${RM} main
	${RM} neuralNet.o utils.o forProp.o backprop.o neuralTraining.o
	${RM} *.json
# END
