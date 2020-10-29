#ifndef PREPROCESSING_H_
#define PREPROCESSING_H_

typedef struct
{
  double b;
  double f;
}couple;

void toGrayscale(SDL_Surface *image_surface, int w, int h);
void histogram(SDL_Surface *image_surface, int w, int h, long *histo);

long calcul_sum_gray(long *histo);
couple init_couple();

couple cal_weight(couple w, int t, int total, long *histo);
couple cal_sum(couple s, int t, long *histo, long sum_gray);
couple cal_mean(couple m, couple s, couple w);

int threshold(SDL_Surface *image_surface, int w, int h, long *histo);
void binarize(SDL_Surface *image_surface, int w, int h, long *histo);

void reduce_noise(SDL_Surface *is, int w, int h);
void contrast(SDL_Surface *im, int lvlc, int w, int h);

#endif
