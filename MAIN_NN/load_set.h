# ifndef __LOAD_SET_H__
# define __LOAD_SET_H__

# include <stdlib.h>

# define NN_OUT_SIZE 62
# define IMG_SIDE 30
# define IMG_SIZE 900

void print_output(double *out);

/*	Load at max max_len images from the path directory
 *	and fill the outputs and inputs
 *
 *	return the number of elements loaded
 *
 * /!\ Inputs and outputs must be large enough
 */
size_t load_dataset(char *path, size_t max_len, double *inputs, double *outputs);

// For the nn output, we'll put it in this order : [0-9][a-z][A-Z]

# endif
