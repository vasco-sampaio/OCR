#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_functions.h"
#include "segmentation.h"

/*
This file is in charge of the segmentation
using histograms.
*/
double rect_ratio(coord r);
/*
Function that checks if whether or not, a pixel
that has the coordinates h and w, is black.
Returns an int : 
-0 if black
-1 if white
*/
int is_black(SDL_Surface *image_surface, int w, int h)
{
  Uint32 pixel = get_pixel(image_surface, w, h);
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is black
  //printf("r = %d\n", r);
  if (r == 0)
    return 0;
  return 1;
}


/*
Function that check whether or not a pixel is a
red pixel.
Returns an int :
-0 if red
-1 if not
*/
int is_red(SDL_Surface *image_surface, int w, int h)
{
  Uint32 pixel = get_pixel(image_surface, w, h);
  //variables for the rgb values of each pixel
  Uint8 r, g, b;
  SDL_GetRGB(pixel, image_surface->format,&r, &g, &b);

  //checks if the pixel is red
  //if it is, returns 0
  if (r == 255 && g == 0 && b == 0)
    return 0;
  return 1;
}

/*
Function that creates a vertical histogram that counts the foreground 
pixels (black pixels) of each row, of a given rectangle.
*/
void verti_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
  int ind = 0;
  for(int i = rect.topL.h ; i < rect.botR.h ; i++)
    {
      for(int j = rect.topL.w ; j < rect.botR.w ; j++)
	{
	  if (is_black(image_surface, j, i) == 0)
	      *(histo + ind) += 1;
	}
      ind++;
    }
}


/*
Function that creates a horizontal histogram that counts the foreground 
pixels of each column, of a given rectangle.
 */
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect)
{
  int ind = 0;
  for(int j = rect.topL.w ; j < rect.botR.w ; j++)
    {
      for(int i = rect.topL.h ; i < rect.botR.h ; i++)
	{
	  if(is_black(image_surface, j, i) == 0)
	    *(histo + ind) += 1;
	}
      ind++;
    }
}


/*
Function that draws horizontal lines in a given rectangle, 
in rows with no foreground pixels.
 */
void hori_lines(SDL_Surface *image_surface, int *vertHisto, coord rect)
{
  int topLh = rect.topL.h;
  int topLw = rect.topL.w;
  int botRw = rect.botR.w;
  int botRh = rect.botR.h;
  int len = botRh - topLh;
  for(int i = 0 ; i < len ; i++)
    {
      if (*(vertHisto + i) == 0)
	  trace_hori_red_line(image_surface, topLh + i, topLw, topLh + i, botRw);
    }
}


/*
Function that draws vertical lines in a given rectangle, 
in columns with no foreground pixels.
*/
void vert_lines(SDL_Surface *image_surface, int *hori_histo, coord rect)
{
  int topLh = rect.topL.h;
  int topLw = rect.topL.w;
  int botRw = rect.botR.w;
  int botRh = rect.botR.h;
  int len = botRw - topLw;
  for(int i = 0 ; i < len ; i++)
    {
      if (*(hori_histo + i) == 0)
	trace_vert_red_line(image_surface, topLh, topLw + i, botRh, topLw + i);
    }
}


//This function calculates the average number of black pixels
//in the histogram.
int average_black_pixels(int len, long *histo)
{
  int a = 0;
  for(int i = 0 ; i < len ; i++)
    {
      a += *(histo + i);
    }
  return a / len;
}


//function that smoothes the projection profile histogram
void moving_average(int len, long *histo, int window)
{
  int sum;
  int average;
  for(int i = 0 ; i < len ; i++)
    {
      //reseting the sum value
      sum = 0;
      if(i + window < len)
	{
	  //adding the values of the histo
	  for(int j = i ; j < i + window ; j++)
	    {
	      sum += *(histo + j);
	    }
	  average = sum / window; 
	  //changing the values of the histo
	  for(int j = i ; j < i + window ; j++)
	    {
	      *(histo + j) = average;
	    }
	}
    }
}


/*
Function that counts the number of peaks in a given 
histogram.
*/
int count_peaks(int average, int *histo, int lenH)
{
  int isGoingDown = 1; //false, 0 if true
  int before = *histo;
  int res = 0;
  for(int i = 0 ; i < lenH ; i++)
    {
      if(before > *(histo + i) && isGoingDown == 1)
	{
	  isGoingDown = 0;
	  if (*(histo + i) > average)
	    res++;
	}
      else if(before <= *(histo + i) && isGoingDown == 0)
	isGoingDown = 1;
      before = *(histo+i);
    }
  return res;
}


/*
Function that frees a doc
*/
void free_doc(doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      free(image.allLines[i].letters);
    }
  free(image.allLines);
}


/*
Function that initialises a lineZone variable
*/
lineZones init_lineZones(int nbLines)
{
  lineZones res;
  res.nbZones = nbLines;
  res.zones = calloc(nbLines, sizeof(coord));
  for(int i = 0 ; i < nbLines ; i++)
    {
      res.zones[i].topL.w = 0;
      res.zones[i].topL.h = 0;
      res.zones[i].botR.w = 0;
      res.zones[i].botR.h = 0;
    }
  return res;
}


/*
Function that initialises a structure doc
*/
doc init_doc(int nbLines)
{
  doc res;
  res.nbLines = nbLines;
  res.allLines = calloc(nbLines, sizeof(line));
  return res;
}


/*
Function that initialises a structure line
*/
line init_line(int nbLetters)
{
  line res;
  res.nbLetters = nbLetters;
  res.nbSpaces = 0;
  //if (nbLetters != 0)
  res.letters = calloc(nbLetters, sizeof(coord));
  return res;
}


/*
Function that calculates the average width of a letter on a line
*/
int avLenLetter(line l)
{
  int sum = 0;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      sum += (l.letters[i].botR.w - l.letters[i].topL.w);
    }
  if (l.nbLetters != 0)
    return sum / l.nbLetters;
  return sum;
}

/*
Function that calculates the average height of a letter on a line
*/
double avHeiLetter(line l)
{
  double sum = 0;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      sum += (l.letters[i].botR.h - l.letters[i].topL.h);
    }
  printf("sum = %f\n, nb = %d\n", sum, l.nbLetters);
  if (l.nbLetters != 0)
    return sum / l.nbLetters;
  return sum;
}



/*
Function that separates letters of a same line if they are still in the
same rectangle of letter, even after segmentation
*/
void sepLetters(SDL_Surface *image, line l)
{
  int av = avLenLetter(l);
  double av2 = avHeiLetter(l);
  int len;
  double hei;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      len = l.letters[i].botR.w - l.letters[i].topL.w;
      hei = l.letters[i].botR.h - l.letters[i].topL.h;
      if (len > 1.75*av && hei < 1.2*av2)
	{
	  printf("hei = %f\nav2 = %f\n", hei, av2);
	  int w = l.letters[i].topL.w + (len/2.4);
	  trace_vert_red_line(image, l.letters[i].topL.h, w, l.letters[i].botR.h, w);
	}
    }
}

/*
Function that calculates the average space between two letters
*/
int avSpaceLetter(line *l)
{
  int sum = 0;
  for(int i = 0 ; i < l->nbLetters - 1 ; i++)
    {
      sum += (l->letters[i+1].topL.w - l->letters[i].botR.w);
    }
  if (l->nbLetters != 0)
    return sum / l->nbLetters;
  return sum;
}


/*
Function that detects the spaces between two letters on a line
*/
void detectSpace(line *l)
{
  int av = avSpaceLetter(l);
  int len;
  for(int i = 0 ; i < l->nbLetters - 1 ; i++)
    {
      len = l->letters[i+1].topL.w - l->letters[i].botR.w;
      if (len > av)
	{
	  l->nbSpaces +=1;
	  l->letters[i].folBySpace = 1;
	}
    }
}

void detectSpaceDoc(doc *image)
{
  for(int i = 0 ; i < image->nbLines ; i++)
    {
      detectSpace(&image->allLines[i]);
    }
}

/*
Function to test if the detection of spaces works.
Prints random text with the spaces of the text from the image.
*/
void print_line(line *l)
{
  int sum = 0;
  for(int i = 0 ; i < l->nbLetters ; i++)
    {
      printf("A");
      sum += 1;
      if (l->letters[i].folBySpace == 1)
	printf(" ");
    }
  printf("\n %d \n", sum);
}

/*
Function to test if the detection of spaces works.
Prints the text of the whole doc
*/
void print_doc(doc *image)
{
  for(int i = 0 ; i < image->nbLines ; i++)
    {

      print_line(&image->allLines[i]);
    }
}


/*
Function that goes to the summit of a rectangle,
at the right of a given point.
Returns the width of this given point
*/
int r_sum(SDL_Surface *image_surface, int wMin, int wMax, int h)
{
  int red = 1;
  int res = wMin;
  while (red == 1 && res < wMax)
    {
      red = is_red(image_surface, res, h);
      res++;
    }
  return res;
}


/*
Function that goes to the summit of a rectangle,
at the bottom of a given point.
Returns the height of this given point
*/
int bot_sum(SDL_Surface *image_surface, int hMin, int hMax, int w)
{
  int red = 1;
  int res = hMin;
  while (red == 1 && res < hMax)
    {
      red = is_red(image_surface, w, res);
      res++;
    }
  return res;
}


/*
Function that goes through the image to count the zones 
not touched by the lines drawn
*/
int count_get_lines(SDL_Surface *image_surface, coord rect)
{
  int res = 0;
  int i = rect.topL.h;
  while (i < rect.botR.h)
    {
      int j = rect.topL.w;
      while(j < rect.botR.w)
	{
	  int red = is_red(image_surface, j, i);
	  if(red == 1) //first encounter with a pixel not red
	    {
	      int tmp =  bot_sum(image_surface, i, rect.botR.h, j);
	      res++;
	      j = rect.botR.w;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
  return res;
}


/*
Function that goes through the image to get the zones 
not touched by the lines drawn.
Gets 2 coordinates to define a rectangle
*/
void get_lines(SDL_Surface *image_surface, coord rect, lineZones all)
{
  int ind = 0;
  int i = rect.topL.h;
  while (i < rect.botR.h)
    {
      int j = rect.topL.w;
      while(j < rect.botR.w)
	{
	  int red = is_red(image_surface, j, i);
	  if(red == 1) //first encounter with a pixel not red
	    {
	      all.zones[ind].topL.w = j;
	      all.zones[ind].topL.h = i;
	      all.zones[ind].botR.w = r_sum(image_surface, j, rect.botR.w, i);
	      int tmp =  bot_sum(image_surface, i, rect.botR.h, j);
	      all.zones[ind].botR.h = tmp;
	      ind++;
	      j = rect.botR.w;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
}

/*
Function that goes through a given rectangle to count the zones 
were there are letters
*/
int count_get_letters(SDL_Surface *image_surface, coord rect)
{
  int res = 0;
  int i = rect.topL.w;
  while(i < rect.botR.w)
    {
      int j = rect.topL.h;
      while(j < rect.botR.h)
	{
	  int red = is_red(image_surface, i, j);
	  if (red == 1)//first encounter of a pixel not red
	    {
	      int tmp = r_sum(image_surface, i, rect.botR.w, j);
	      res++;
	      j = rect.botR.h;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
  return res;
}


/*
Function that goes through a given rectangle to get the zones not touched by the red lines and where there are the letters.
*/
void get_letters(SDL_Surface *image_surface, coord rect, line l)
{
  int ind = 0;
  int i = rect.topL.w;
  while(i < rect.botR.w)
    {
      int j = rect.topL.h;
      while(j < rect.botR.h)
	{
	  int red = is_red(image_surface, i, j);
	  if (red == 1)//first encounter of a pixel not red
	    {
	      l.letters[ind].folBySpace = 0;
	      l.letters[ind].topL.w = i;
	      l.letters[ind].topL.h = j;
	      l.letters[ind].botR.h = bot_sum(image_surface, j, rect.botR.h, i);
	      int tmp = r_sum(image_surface, i, rect.botR.w, j);
	      l.letters[ind].botR.w = tmp;
	      ind++;
	      j = rect.botR.h;
	      i = tmp;
	    }
	  j++;
	}
      i++;
    }
}


/*
Function that does the vertical histogram and
draws the lines to show the lines of the text.
*/
void marking_lines(SDL_Surface *image_surface, int height, int width)
{
  int *histo;
  histo = calloc(height, sizeof(int));
  coord rect;
  rect.topL.h = 0;
  rect.topL.w = 0;
  rect.botR.w = width-1;
  rect.botR.h = height;
  verti_histo(image_surface, histo, rect);
  hori_lines(image_surface, histo, rect);
  free(histo);
}


/*
Function that gets the nb of lines and for each line, 
makes the horizontal histogram to separate the
letters, and draws the lines.
*/
lineZones marking_letters(SDL_Surface *image_surface, int w, int h)
{
  coord rect;
  rect.topL.h = 0;
  rect.topL.w = 0;
  rect.botR.w = w;
  rect.botR.h = h;
  int nbLines = count_get_lines(image_surface, rect);
  lineZones all = init_lineZones(nbLines);
  get_lines(image_surface, rect, all);
  for(int i = 0 ; i < nbLines ; i++)
    {
      int topLw = all.zones[i].topL.w;
      int botRw = all.zones[i].botR.w;
      int *histo2 = calloc(botRw - topLw, sizeof(int));
      hori_histo(image_surface, histo2, all.zones[i]);
      vert_lines(image_surface, histo2, all.zones[i]);
      free(histo2);
    }
  return all;
}


/*
Function that takes all the coordinates of the rectangles
that contains each letter.
*/
doc keep_letters(SDL_Surface *image_surface, lineZones all)
{
  doc image = init_doc(all.nbZones);
  for(int i = 0 ; i < all.nbZones ; i++)
    {
      int nbLetters = count_get_letters(image_surface, all.zones[i]);
      image.allLines[i] = init_line(nbLetters);
      get_letters(image_surface, all.zones[i], image.allLines[i]);
    }
  return image;
}

/*
Function that resizes the rectangles arround the letters.

*//*
void resize_letter(SDL_Surface *image_surface, doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      sepLetters(image_surface, image.allLines[i]);
      
      for(int j = 0 ; j < image.allLines[i].nbLetters ; j++)
	{
	  int botRh = image.allLines[i].letters[j].botR.h;
	  int topLh = image.allLines[i].letters[j].topL.h;
	  int *histo3 = calloc(botRh - topLh, sizeof(int));
	  verti_histo(image_surface, histo3, image.allLines[i].letters[j]);
	  hori_lines(image_surface, histo3, image.allLines[i].letters[j]);
	  free(histo3);
	}
    }
  detectSpaceDoc(&image);
}
  */

/*
Function that prints the content of a matrix
*/
void print_matrix(matrix m)
{
  for(int i = 0 ; i < m.height ; i++)
    {
      for(int j = 0 ; j < m.width ; j++)
	{
	  printf("%.0lf", m.mat[i*m.width+j]);
	  if((j+1) % m.width == 0)
	    printf("\n");
	}
    }
}

/*
Function that builds a matrix for a letter
*/
matrix buildMatrix(SDL_Surface *image, coord let)
{
  matrix m;
  m.width = let.botR.w - let.topL.w;
  m.height = let.botR.h - let.topL.h;
  m.mat = calloc(m.width*m.height, sizeof(double));
  for(int i = 0 ; i < m.height; i++)
    {
      for(int j = 0 ; j < m.width  ; j++)
	{
	  if(is_black(image, j+let.topL.w, i+let.topL.h) == 0)
	    m.mat[i*m.width + j] = 1;
	  else
	    m.mat[i*m.width + j] = 0;
	}
    }
  //print_matrix(m);
  //printf("\n\n");
  return m;
}

/*
Function that builds a lineMat
*/
lineMat buildLineMat(SDL_Surface *image, line l)
{
  lineMat lm;
  lm.letterMat = malloc(l.nbLetters*sizeof(matrix));
  lm.nbLetters = l.nbLetters;
  for(int i = 0 ; i < l.nbLetters ; i++)
    {
      lm.letterMat[i] = buildMatrix(image, l.letters[i]);
    }
  return lm;
}

/*
Function that build docMat
*/
docMat buildDocMat(SDL_Surface *image, doc i)
{
  docMat r;
  r.nbLines = i.nbLines;
  r.lines = malloc(i.nbLines*sizeof(lineMat));
  for(int j = 0 ; j < i.nbLines ; j++)
    {
      r.lines[j] = buildLineMat(image, i.allLines[j]);
    }
  return r;
}

/*
Function that frees docMat
*/
void free_docMat(docMat m)
{
  for(int i = 0 ; i < m.nbLines ; i++)
    {
      for(int j = 0 ; j < m.lines[i].nbLetters ; j++)
	{
	  free(m.lines[i].letterMat[j].mat);
	}
      free(m.lines[i].letterMat);
    }
  free(m.lines);
}


//--------------------------------------------------------------------------

//to keep point on the i and take off accents

/*
Calculates the ratio w/h of a rectangle
*/
double rect_ratio(coord r)
{
  double w = r.botR.w - r.topL.w;
  double h = r.botR.h - r.topL.h;
  printf("ratio\nw = %f\nh = %f\n", w, h);
  if (h == 0)
    return 0;
  return w/h;
}

/*
Function that counts the number of groups of black pixels in a given 
histogram.
*/
int count_groups(int *histo, int lenH)
{
  int inZone = 0;
  int res = 0;
  for(int i = 0 ; i < lenH ; i++)
    {
      if (inZone == 0 && *(histo + i) != 0)
	{
	  inZone = 1;
	  ++res;
	}
      if(inZone == 1 && *(histo + i) == 0)
	{
	  inZone = 0;
	}
    }
  return res;
}

/*
Function that returns the rank of the black pixels groups
*/
tuples generate_groups(int *histo, int lenH)
{
  tuples t;
  int nbgp = count_groups(histo, lenH);
  t.nb = nbgp;
  t.list = calloc(nbgp, sizeof(couple));
  int inZone = 0;
  int j = 0;
  for(int i = 0 ; i < lenH ; i++)
    {
      if (inZone == 0 && *(histo + i) != 0)
	{
	  inZone = 1;
	  t.list[j].h = i; 
	}
      if(inZone == 1 && *(histo + i) == 0)
	{
	  inZone = 0;
	  t.list[j].w = i;
	  ++j;
	}
      if(inZone == 1 && i+1 == lenH)
	t.list[j].w = i;
    }
  return t;
}

/*
Function that returns the index of the largest group of black pixels
*/
int max_tuples(tuples t)
{
  if (t.nb > 0)
    {
      int index = 0;
      int max = t.list[0].h - t.list[0].w;
      int tmp;
      for(int i = 0 ; i < t.nb ; i++)
	{
	  tmp = t.list[i].h - t.list[i].w;
	  if(max < tmp)
	    {
	      index = i;
	      max = tmp;
	    }
	}
      return index;
    }
  else
    return -1;
}

/*
Function that draws vertical lines in a given rectangle, 
in rows with no foreground pixels and in attributed columns.
*/
void hori_lines_define(SDL_Surface *image, int *vertHisto, coord rect, tuples t)
{
  //int len = rect.botR.h - rect.topL.h;
  double r = rect_ratio(rect);
  int gps = t.nb;

  printf("\ngps = %d\n\nr = %f\n", gps, r);

  if (gps == 1)
    {
      //printf("\nrect.topL.h = %d\n",rect.topL.h);
      //printf("rect.topL.w = %d\n\n\n", rect.topL.w);
      hori_lines(image, vertHisto, rect);
    }
  else
    {
      //if the ratio is like a i
      if (r < 0.33 && gps == 2)
	{
	  int highr = t.list[0].h;
	  int lowr = t.list[1].w;
	  printf("hereeeee\n");
	  printf("highr = %d \nlowr = %d\n", highr, lowr);
	  printf("\nrect.topL.h = %d\nlen = %d\n",rect.topL.h, rect.topL.h + highr);
	  printf("rect.topL.w = %d\n\n", rect.topL.w);
	  for(int i = rect.topL.h ; i < rect.topL.h + highr ; i++)
	    {
	      printf("hi\n");
	      trace_hori_red_line(image, i,
		rect.topL.w, i, rect.botR.w);
	    }
	  for(int i = rect.topL.h + lowr ; i < rect.botR.h ; i++)
	    {
	      trace_hori_red_line(image, i,
			      rect.topL.w, i, rect.botR.w);
	    }
	}
      else
	{
	  printf("hi22222\n");
	  int indMax = max_tuples(t);
	  int highr = t.list[indMax+1].h;
	  int lowr = t.list[indMax+1].w;
	  printf("indMax = %d\n", indMax);
	  printf("else\nhighr = %d \nlowr = %d\n", highr, lowr);
	  printf("\nrect.topL.h = %d\nlen = %d\n\n",rect.topL.h, rect.topL.h + highr);
	  printf("rect.topL.w = %d\n\n", rect.topL.w);
	  for(int i = rect.topL.h ; i < rect.topL.h + highr ; i++)
	    {
	      trace_hori_red_line(image, i,
			      rect.topL.w, i, rect.botR.w);
	    }
	  for(int i = rect.topL.h + lowr ; i < rect.botR.h ; i++)
	    {
	      trace_hori_red_line(image, i,
			      rect.topL.w, i, rect.botR.w);
	    }
	}
    }
}


/*
Function that resizes the rectangle arround a letter but 
takes into account the accents and the points on the "i"
*/
void resize(SDL_Surface *image, coord letter)
{
  int len =letter.botR.h - letter.topL.h;
  int *histo = calloc(len, sizeof(int));
  verti_histo(image, histo, letter);
  tuples t = generate_groups(histo, len);
  hori_lines_define(image, histo, letter, t);
  free(t.list);
  free(histo);
}


/*
Function that resizes the rectangles arround the letters.
*/
void resize_letter(SDL_Surface *image_surface, doc image)
{
  for(int i = 0 ; i < image.nbLines ; i++)
    {
      sepLetters(image_surface, image.allLines[i]);
      
      for(int j = 0 ; j < image.allLines[i].nbLetters ; j++)
	{
	  resize(image_surface, image.allLines[i].letters[j]);
	}
    }
  detectSpaceDoc(&image);
}
