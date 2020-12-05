#ifndef PREPROCESSING_H_
#define PREPROCESSING_H_

typedef struct
{
  double b;
  double f;
}duo;

void toGrayscale(SDL_Surface *image_surface, int w, int h);
void histogram(SDL_Surface *image_surface, int w, int h, long *histo);

long calcul_sum_gray(long *histo);
duo init_couple();

void rotate(SDL_Surface *image, int w, int h, double angle);

duo cal_weight(duo w, int t, int total, long *histo);
duo cal_sum(duo s, int t, long *histo, long sum_gray);
duo cal_mean(duo m, duo s, duo w);

int threshold(SDL_Surface *image_surface, int w, int h, long *histo);
void binarize(SDL_Surface *image_surface, int w, int h, long *histo);

void reduce_noise(SDL_Surface *is, int w, int h);
void contrast(SDL_Surface *im, int lvlc, int w, int h);

void preprocessing(char *path);
SDL_Surface* preprocessing_SDL(char *path);

#endif
