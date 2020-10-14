#ifndef CHAR_SEG_H_
#define CHAR_SEG_H_

int isBlack(SDL_Surface *image_surface, Uint32 pixel);
void horiHistogram(SDL_Surface *image_surface, long *histo);
int average_black_pixels(int len, long *histo);
void moving_average(int len, long *histo, int window);
size_t char_seg_count(long *histo, int lenH);
void char_seg(long *histo, int lenH, int *chars);
void char_seg_drawing(SDL_Surface *image_surface, int *chars, size_t lenC);

#endif
