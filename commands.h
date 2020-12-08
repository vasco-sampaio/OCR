# ifndef _COMMANDS_H
# define _COMMANDS_H

void launch_GUI(void);

void process_OCR(char *nn_path, char *img_path);

void train_neural_net(char *nn_path, char *dataset_path, int nb_gens);

# endif // _COMMANDS_H
