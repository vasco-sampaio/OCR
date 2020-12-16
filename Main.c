# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <err.h>
# include <stdlib.h>
# include <unistd.h>

//# include "image_segmentation/segmentation.h"
//# include "preprocessing/preprocessing.h"
//# include "neunet_HD/neural_main.h"
//# include "UI/ocr_ui.h"
# include "types.h"

// Where to find a default neural net file 
# define NN_DEFAULT "./test.nn"

// Just print the help text
void help(void)
{
	printf("Usage:\n");
	printf("- No arguments : launch GUI\n");
	printf("- Options:\n"
			"	-h : Display help\n"
			"	-a BMP_FILE [NN_FILE] : Compute OCR on the BMP_FILE, with NN_FILE, if given."
					"otherwise, process it with the default one.\n"
			"	-t NN-PATH DATA-SET-PATH DATA-SET-LEN [NB-GEN]: if nn-path exists, load it, train it with the given dataset, otherwise, create one, train it and saves it.\n");
}




int main(int argc, char **argv)
{
	printf("Neural net actual size: %i, %i, %i\n", NN_INPUTS, NN_HIDDENS, NN_OUTPUTS);
	if(argc == 1)
	{
		printf("Launching GUI\n");
		launch_GUI();
	}
	else
	{
		// Please refer to the help text to learn about the options
		char *nn_path = NN_DEFAULT;
		int option = 0;	
		if((option = getopt(argc, argv, ":hatv")) != -1)
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

					SDL_Surface *image_surface = IMG_Load(img_path);
				        /*double a = find_angle(image_surface);
					printf("a = %f\n", a);
					SDL_SaveBMP(rotate2(image_surface, image_surface->w, image_surface->h, a), "rotation.bmp");*/
					double a = hough(image_surface);
					SDL_Surface *test =rotate2(image_surface, image_surface->w, image_surface->h, a);
					SDL_SaveBMP(test, "rotation.bmp");
					SDL_FreeSurface(test);
					SDL_FreeSurface(image_surface);
					/*neunet_t *nn = fileToNeuralNet(nn_path);
					segmentation(img_path, nn);
					char *txt = ocr(img_path, nn_path);
					printf("text :\n%s\n", txt);
					free(txt);*/
					/*int c[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
					SDL_Surface *t = convolution(image_surface, c , 3);
					SDL_SaveBMP(t, "conv.bmp");*/
					//dataset(img_path);
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
						gens = atoi(argv[optind + 3]);

					nn_path = argv[optind];
					char *ds_path = argv[optind + 1];
					int set_size = atoi(argv[optind + 2]);

					neural_net_run_training(nn_path, ds_path, set_size, gens);
					break;

				case 'v':
					if(optind + 2 >= argc)
					{
						printf("-v : bad usage.\n");
						help();
						break;
					}
					nn_path = argv[optind];
					char *ds_path1 = argv[optind + 1];
					int set_size1 = atoi(argv[optind + 2]);

					neural_net_validation(nn_path, ds_path1, set_size1);
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
}

