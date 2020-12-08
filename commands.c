# include <SDL/SDL.h>
# include <SDL_image.h>
# include <err.h>
# include <stdlib.h>

# include "image_segmentation/histo_segmentation/segmentation.h"
# include "preprocessing/preprocessing.h"
# include "neunet_HD/neunet.h"
# include "neunet_HD/load_set.h"
# include "neunet_HD/neuralIO.h"
# include "UI/ocr_ui.h"


void launch_GUI()
{
	// TODO
}


void process_OCR(char *nn_path, char *image_path)
{
	neunet_t *nn = fileToNeuralNet(nn_path);
	SDL_Surface *img = preprocessing_SDL(img_path);	
}


void train_neural_net(char *nn_path, char *dataset_path, int nb_gens)
{
	neunet_t *nn;
	if(access(nn_path, F_OK) == 0)
		nn = fileToNeuralNet(nn_path);
	else
		nn = init_neunet();

	for(int g = 0; g < nb_gens; ++g)
	{

	}

}
