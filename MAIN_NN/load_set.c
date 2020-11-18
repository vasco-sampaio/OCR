# include "load_set.h"

# include <stdio.h>
# include <dirent.h>
# include <string.h>



/*
 * load the files in the *path* folder and send
 * back an array of inputs. and save the output in outputs
 *
 * outputs doesn't need to be initialized
 */
void load_dataset(char *path, size_t *len, double *inputs, double *outputs)
{
	DIR *main_dir = opendir(path);	
	struct dirent *ent;

	while((ent = readdir(main_dir)) != NULL)
	{
		if(strstr(ent->d_name, ".bmp") != NULL)
		{
			printf("%s\n", ent->d_name);
			*len += 1;
		}
	}

}



int main()
{
	double *ins;
	double *outs;
	size_t x = 0;
	load_dataset(".", &x, ins, outs);
	//*x = 12;
	printf("%lu\n", x);
}
