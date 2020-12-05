//structure.h file
//regroups all the structures needed for segmentation

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

//------------------------------------------------------------------------------

/*
To keep the coordinates of a pixel in the image.
*/
typedef struct
{
  int w;
  int h;
}couple;


/*
Are used to detect the i and accents.
To know the number of groups of black pixels in a row / column.
*/
typedef struct
{
  couple *list;
  int nb;
}tuples;


/*
Regroups 2 coordinates of a rectangles in an image.
The first one is the point in the top left corner.
The other one is the point in the bottom right corner.
They are enough to define a rectangle.
*/
typedef struct
{
  couple topL;
  couple botR;
  int folBySpace; //if a letter is followed by a space equals 1, else 0
}coord;


/*
Contains all the rectangles around all the letters of a line.
To sum up : regroups all the letters of a line.
Keeps the number of letters and the number of spaces (useful for the UI)
*/
typedef struct
{
  int nbLetters;
  int nbSpaces;
  coord *letters;
}line;


/*
Contains all the rectangles that define a line in the image.
To sum up : regroups all the lines of a document.
*/
typedef struct
{
  int nbZones;
  coord *zones;
}lineZones;


/*
Regroups all the parts of a document : letters, lines, lineZones
*/
typedef struct
{
  int nbLines;
  line *allLines;
  lineZones docZone;
}doc;

//------------------------------------------------------------------------------

typedef struct
{
  int width;
  int height;
  double *mat;
}matrix;


typedef struct
{
  int nbLetters;
  matrix *letterMat;
}lineMat;


typedef struct
{
  int nbLines;
  lineMat *lines;
}docMat;

//------------------------------------------------------------------------------

//To initialize
lineZones init_lineZones(int nbLines);
line init_line(int nbLetters);
doc init_doc(int nbLines);

//To free
void free_doc(doc image);
void free_docMat(docMat m);

//To test
void print_line(line *l);

char* line_string(line *l);
int nb_char(doc *image);
char* doc_string(doc *image);

void print_doc(doc *image);
void print_matrix(matrix m);

#endif
