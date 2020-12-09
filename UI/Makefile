

CC = gcc
CFLAGS = -g `pkg-config --cflags gtk+-3.0` -Wall -O3 -I../image_segmentation/
LDLIBS = `pkg-config --libs gtk+-3.0`


all: ocr_ui

ocr_ui: ui_utils.o segmentation.o


clean:
	${RM} $(EXE)
	${RM} *.o
	${RM} ocr_ui

