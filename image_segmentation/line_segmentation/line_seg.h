#ifndef LINE_SEG_H_
#define LINE_SEG_H_

int isBlack(SDL_Surface *image_surface, Uint32 pixel);
void vertHistogram(SDL_Surface *image_surface, long *histo);
int average_black_pixels(int len, long *histo);
void moving_average(int len, long *histo, int window);
size_t line_seg_count(long *histo, int lenH, int a);
void line_seg(long *histo, int lenH, int *lines, int a);
void line_seg_drawing(SDL_Surface *image_surface, int *lines, size_t lenL);

#endif
