# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <err.h>
# include <stdlib.h>
# include <unistd.h>

# include "image_segmentation/histo_segmentation/segmentation.h"
# include "preprocessing/preprocessing.h"
# include "neunet_HD/neural_main.h"

// Where to find a default neural net file 
# define NN_DEFAULT "./neunet_HD/test.nn"

// Just print the help text
void help(void)
{
	printf("Usage:\n");
	printf("- No arguments : launch GUI\n");
	printf("- Options:\n"
			"	-h : Display help\n"
			"	-a BMP_FILE [NN_FILE] : Compute OCR on the BMP_FILE, with NN_FILE, if given."
					"otherwise, process it with the default one.\n"
			"	-t NN-PATH DATA-SET-PATH [NB-GEN]: if nn-path exists, load it, train it with the given dataset, otherwise, create one, train it and saves it.\n");
}




int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("Launching GUI\n");
		//launch_GUI();
	}
	else
	{
		// Please refer to the help text to learn about the options
		char *nn_path = NN_DEFAULT;
		int option = 0;	
		if((option = getopt(argc, argv, ":hat")) != -1)
		{
			switch(option)
			{
				case 'a':
					if(optind == argc)
					{
						help();
						break;
					}
					char *img_path = argv[optind];
					if(optind + 1 < argc)
						nn_path = argv[optind + 1];

					//process_OCR(nn_path, img_path);
					break;

				case 't':
					if(optind + 2 >= argc)
					{
						printf("-t : bad usage.\n");
						help();
						break;
					}
					int gens = 500;
					if(optind + 3 < argc)
						gens = atoi(argv[optind + 2]);

					nn_path = argv[optind];
					char *ds_path = argv[optind + 1];
					int set_size = atoi(argv[optind + 2]);

					neural_net_run_training(nn_path, ds_path, set_size, gens);
					break;

				case 'h':
				case '?':
				default:
					help();
					break;
			}
		}	
		else
		{
			printf("Unknown argument.\n");
			help();
		}
	}
	return 0;
}


/*
int _main(int argc, char** argv)
{
	if(argc != 2)
		errx(1, "the number of arguments is not valid : you should have 2 arguments");

	SDL_Surface *image = preprocessing_SDL(argv[1]);
	char *text = segmentation_SDL(image);
	printf("text = \n%s\n", text);

	free(text);

	return 0;
}
*/
