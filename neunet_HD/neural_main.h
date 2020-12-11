# ifndef __NEURAL_MAIN_H
# define __NEURAL_MAIN_H

# include "neunet.h"
# include "neuralIO.h"
# include "load_set.h"
# include "../types.h"

# define LEARNING_RATE 0.1


void neural_net_run_training(char *nn_path, char *data_set, int set_size, int gens);

# endif // __NEURAL_MAIN_H
