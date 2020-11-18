# ifndef __LOAD_SET_H__
# define __LOAD_SET_H__

# include <stdlib.h>

# define NN_OUT_SIZE 62
# define IMAGE_SIZE 41*41

void load_dataset(char *path, size_t len, double *inputs, double *outputs);



# endif
