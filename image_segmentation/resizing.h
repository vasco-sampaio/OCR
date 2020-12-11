#ifndef RESIZING_H_
#define RESIZING_H_

# include "../types.h"

struct ratio* normalized_ratio(int width, int height, float n_ratio);
matrix interpolation(matrix *m, int n_width, int n_height);
void  m_fill(matrix* m, int new_size);
matrix normalization(matrix* m);
void print_m(matrix m);

#endif
