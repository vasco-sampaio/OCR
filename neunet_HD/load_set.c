# include "load_set.h"
# include "pixel_functions.h"

# include <stdio.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <err.h>
# include <SDL.h>
# include <SDL/SDL_image.h>


double is_pixel_true(SDL_Surface *img, int x, int y)
{
	Uint32 pix = get_pixel(img, x, y);
	return (pix) ? 0.0 : 1.0;
}

//place the image in arr
void load_image(char *path, char *name, double *arr)
{
	char *full_path = calloc(strlen(path) + strlen(name)+ 2, sizeof(char));
	char *slash = "/";
	strcpy(full_path, path);
	strcat(full_path, slash);
	strcat(full_path, name);
	//we can get the image with full_path
	//printf("%s\n",full_path);
	SDL_Surface *img = IMG_Load(full_path);

	if(img != NULL)
	{
		for(int i = 0; i < img->w; ++i)
		{
			for(int j = 0; j < img->h; ++j)
			{
				arr[i * img->w + j] = is_pixel_true(img, j, i);
			}
		}
	}

	free(img);
	free(full_path);
}


void load_answer(char letter, double *arr)
{
	//int shift = 0;
	if(letter >= '0' && letter <= '9')
		*(arr + letter - '0') = 1;
	if(letter >= 'a' && letter <= 'z')
		*(arr + letter - 'a' + 10) = 1;
	if(letter >= 'A' && letter <= 'Z')
		*(arr + letter - 'A' + 36) = 1;
}


/*
 * load the files in the *path* folder and send
 * back an array of inputs. and save the output in outputs
 *
 * outputs doesn't need to be initialized
 */
size_t load_dataset(char *path, size_t len, double *inputs, double *outputs)
{
	DIR *main_dir = opendir(path);	
	if(errno)
		errx(errno, "Error: no such directory: %s", path);

	struct dirent *ent;

	char letter;

	size_t count = 0;

	while((ent = readdir(main_dir)) != NULL && count < len)
	{
		if(strstr(ent->d_name, ".bmp") != NULL)
		{
			// the 1st letter is the actual symbol of the file
			letter = ent->d_name[0];

			// Function that load the image in the path in the array pointed by
			// inputs

			// TODO
			load_image(path, ent->d_name, inputs + (IMG_SIZE * count));

			// Function that fill the output part from the output spointer
			load_answer(letter, outputs + (NN_OUT_SIZE * count));

			count++;

		}
	}

	return count;
}





void print_output(double *out)
{
	int t = 0;

	for(; t < 10; ++t)
	{
		if(out[t])
			printf("\033[1;33m%c \033[0m",t + '0');
		else
			printf("%c ",t + '0');

	}
	for(; t < 36; ++t)
	{
		if(out[t])
			printf("\033[1;33m%c \033[0m",t + 'a' - 10);
		else
			printf("%c ",t + 'a' - 10);
			
	}
	for(; t < 62; ++t)
	{
		if(out[t])
			printf("\033[1;33m%c \033[0m",t + 'A' - 36);
		else
			printf("%c ",t + 'A' - 36);
	}
	printf("\n");
}


void print_input(double *in)
{
	for(int i = 0; i < IMG_SIDE; ++i)
	{
		for(int j = 0; j < IMG_SIDE; ++j)
		{
			printf("%c ",((int) *(in + i * IMG_SIDE + j)) ? '#': ' ');
		}
		printf("\n");
	}
}

