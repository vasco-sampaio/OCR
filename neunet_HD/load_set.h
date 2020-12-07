# ifndef __LOAD_SET_H__
# define __LOAD_SET_H__

# include <stdlib.h>

# include "neunet.h"

void print_output(double *out);
void print_input(double *in);

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
