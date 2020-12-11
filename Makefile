#Makefile for preprocessing and image segmentation

CC=gcc

TARGET ?= main
SRC_DIRS ?= ./preprocessing ./image_segmentation/ ./neunet_HD ./UI

SRCS = $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.h')
OBJS := $(addsuffix .o,$(basename $(SRCS))) Main.o 
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= `pkg-config --cflags sdl gtk+-3.0` $(INC_FLAGS) -MMD -I/usr/include/json-c/
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm -ljson-c
CFLAGS= -g -Wall -Wextra -pedantic-errors -std=c99 -ljson-c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS) 

.PHONY: clean
clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} *.c~
	${RM} *.h~
	${RM} *.bmp
	${RM} vgcore.*
	for d in $(SRC_DIRS); \
		do \
		make --directory=$$d clean; \
		done






-include $(DEPS)


