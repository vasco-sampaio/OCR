# include "load_set.h"

# include <stdio.h>
# include <dirent.h>
# include <string.h>




void load_answer(char letter, double *arr)
{
	int shift = 0;
	if(letter >= '0' && letter <= '9')
		*(arr + letter - '0') = 1;
	if(letter >= 'a' && letter <= 'z')
		*(arr + letter - 'a' + 10) = 1;
	if(letter >= 'A' && letter <= 'Z')
		*(arr + letter - 'A' + 36) = 1;


	arr += NN_OUT_SIZE;
}


/*
 * load the files in the *path* folder and send
 * back an array of inputs. and save the output in outputs
 *
 * outputs doesn't need to be initialized
 */
void load_dataset(char *path, size_t len, double *inputs, double *outputs)
{
	DIR *main_dir = opendir(path);	
	struct dirent *ent;

	char letter;

	inputs = calloc(IMAGE_SIZE * len, sizeof(double));
	outputs = calloc(NN_OUT_SIZE * len, sizeof(double));

	double *ii = inputs;
	double *io = outputs;
	
	while((ent = readdir(main_dir)) != NULL)
	{
		if(strstr(ent->d_name, ".bmp") != NULL)
		{
			// the 1st letter is the actual symbol of the file
			letter = ent->d_name;

			// Function that load the image in the path in the array pointed by
			// ii

			// TODO
			load_image(path, name, ii);
			
			// Function that fill the output part from the io pointer
			load_answer(letter, io);

		}
	}

}



int main()
{
	double *ins;
	double *outs;
	size_t x = x;
	load_dataset(".", x, ins, outs);
	//*x = 12;
	printf("%lu\n", x);
}
