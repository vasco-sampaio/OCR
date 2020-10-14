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
  couple topRight;
  couple botLeft;
  couple botRight;
}coord;

typedef struct
{
  int nbLetters;
  coord *letters;
}line;

typedef struct
{
  int nbLines;
  line *allLines;
}doc;
  

#endif
