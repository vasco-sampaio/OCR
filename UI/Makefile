

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3
LDLIBS = `pkg-config --libs gtk+-3.0`


all: ocr_ui

ocr_ui: ui_utils.o


clean:
	${RM} $(EXE)


