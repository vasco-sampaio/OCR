#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

typedef struct
{
  int w;
  int h;
}couple;

typedef struct
{
  couple topLeft;
  couple botRight;
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

int is_foreground(SDL_Surface *image_surface, Uint32 pixel);
void verti_histo(SDL_Surface *image_surface, int *histo, int topLw, int topLh, int botRw, int botRh);
void hori_histo(SDL_Surface *image_surface, int *histo, int topLw, int topLh, int botRw, int botRh);
void hori_lines(SDL_Surface *image_surface, int *vertHisto, int topLw, int topLh, int botRw, int botRh);
void vert_lines(SDL_Surface *image_surface, int *hori_histo,int topLw, int topLh, int botRw, int botRh );

lineZones init_lineZones(int nbLines);
int is_red(SDL_Surface *image_surface, Uint32 pixel);

int count_get_lines(SDL_Surface *image_surface);
void get_lines(SDL_Surface *image_surface, lineZones all);

#endif
