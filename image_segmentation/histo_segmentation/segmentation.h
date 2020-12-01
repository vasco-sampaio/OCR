#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

//To keep the coordinates of a pixel in the image.
typedef struct
{
  int w;
  int h;
}couple;

//Regroups 2 coordinates of a rectangles in an image.
//The first one is the point in the top left corner.
//The other one is the point in the bottom right corner.
//They are enough to define a rectangle.
typedef struct
{
  couple topL;
  couple botR;
  int folBySpace; //if a letter is followed by a space equals 1, else 0
}coord;

//Contains all the rectangles around all the letters of a line.
//To sum up : regroups all the letters of a line.
//Keeps the number of letters and the number of spaces (useful for the UI)
typedef struct
{
  int nbLetters;
  int nbSpaces;
  coord *letters;
}line;

//Contains all the rectangles that define a line in the image.
//To sum up : regroups all the lines of a document.
typedef struct
{
  int nbZones;
  coord *zones;
}lineZones;


//Regroups all the parts of a document : letters, lines, lineZones
typedef struct
{
  int nbLines;
  line *allLines;
  lineZones docZone;
}doc;

#endif
