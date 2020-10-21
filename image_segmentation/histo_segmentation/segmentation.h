#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

typedef struct
{
  int w;
  int h;
}couple;

typedef struct
{
  couple topL;
  couple botR;
}coord;

typedef struct
{
  int nbLetters;
  coord *letters;
}line;

typedef struct
{
  int nbZones;
  coord *zones;
}lineZones;

typedef struct
{
  int nbLines;
  line *allLines;
}doc;

//functions in segmentation.c

int is_black(SDL_Surface *image_surface, int w, int h);
int is_red(SDL_Surface *image_surface, int w, int h);

void verti_histo(SDL_Surface *image_surface, int *histo, coord rect);
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect);

void hori_lines(SDL_Surface *image_surface, int *vertHisto, coord rect);
void vert_lines(SDL_Surface *image_surface, int *hori_histo, coord rect);

int average_black_pixels(int len, long *histo);
void moving_average(int len, long *histo, int window);
int count_peaks(int average, int *histo, int lenH);

void free_doc(doc image);
lineZones init_lineZones(int nbLines);
doc init_doc(int nbLines);
line init_line(int nbLetters);

int r_sum(SDL_Surface *image_surface, int wMin, int wMax, int h);
int bot_sum(SDL_Surface *image_surface, int hMin, int hMax, int w);

int count_get_lines(SDL_Surface *image_surface, coord rect);
void get_lines(SDL_Surface *image_surface, coord rect, lineZones all);

int count_get_letters(SDL_Surface *image_surface, coord rect);
void get_letters(SDL_Surface *image_surface, coord rect, line l);

void marking_lines(SDL_Surface *image_surface, int height, int width);
lineZones marking_letters(SDL_Surface *image_surface, int w, int h);
doc keep_letters(SDL_Surface *image_surface, lineZones all);
void resize_letter(SDL_Surface *image_surface, doc image);

void resize_letter2(SDL_Surface *image_surface, doc image);

#endif
