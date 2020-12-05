#Makefile for preprocessing and image segmentation

CC=gcc

TARGET ?= main
SRC_DIRS ?= ./preprocessing ./image_segmentation/histo_segmentation

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.h)
OBJS := $(addsuffix .o,$(basename $(SRCS))) Main.o
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= `pkg-config --cflags sdl`$(INC_FLAGS) -MMD
LDLIBS= `pkg-config --libs sdl` -lSDL_image
CFLAGS= -g -Wall -Wextra -Werror -pedantic-errors -std=c99

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

-include $(DEPS)
