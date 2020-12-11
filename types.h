#ifndef TYPES_H_
#define TYPES_H_

# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include <gtk/gtk.h>
# include <gdk/gdkpixbuf.h>



# define LEARNING_RATE 0.1

# define IMG_SIDE 30
# define NN_INPUTS IMG_SIDE*IMG_SIDE
# define NN_HIDDENS 22
# define NN_OUTPUTS 62

//------------------------------------------------------------------------------

//STRUCTURES

/*
  Structure from preprocessing to take in
  charge tuples for binarisation.
*/
typedef struct
{
  double b;
  double f;
}duo;


/*
  Used for normalization
  Gives the dimension of the normalized matrix and the aspect ratio used
*/
struct ratio{
  float w;
  float h;
  float r;
};

/*
  To keep the coordinates of a pixel in the image.
*/
typedef struct
{
  int w;
  int h;
}couple;


/*
  Are used to detect the i and accents.
  To know the number of groups of black pixels in a row / column.
*/
typedef struct
{
  couple *list;
  int nb;
}tuples;


/*
  Regroups 2 coordinates of a rectangles in an image.
  The first one is the point in the top left corner.
  The other one is the point in the bottom right corner.
  They are enough to define a rectangle.
*/
typedef struct
{
  couple topL;
  couple botR;
  int folBySpace; //if a letter is followed by a space equals 1, else 0
}coord;


/*
  Contains all the rectangles around all the letters of a line.
  To sum up : regroups all the letters of a line.
  Keeps the number of letters and the number of spaces (useful for the UI)
*/
typedef struct
{
  int nbLetters;
  int nbSpaces;
  coord *letters;
}line;


/*
  Contains all the rectangles that define a line in the image.
  To sum up : regroups all the lines of a document.
*/
typedef struct
{
  int nbZones;
  coord *zones;
}lineZones;


/*
  Regroups all the parts of a document : letters, lines, lineZones
*/
typedef struct
{
  int nbLines;
  line *allLines;
  lineZones docZone;
}doc;

/*
  To keep the letters in matrices.
*/
typedef struct
{
  int width;
  int height;
  double *mat;
}matrix;


typedef struct
{
  int nbLetters;
  matrix *letterMat;
}lineMat;


typedef struct
{
  int nbLines;
  lineMat *lines;
}docMat;


/*
  Structure for the neural network
*/
typedef struct nn
{

	// index out the highest output neuron
	// in the output layer
	int highest_output;

	// Inputs array
	double inputs[NN_INPUTS];
	double exp_outputs[NN_OUTPUTS];

	// Biases arrays
	double biases_h[NN_HIDDENS];
	double biases_o[NN_OUTPUTS];

	// Weights arrays
	double weights_i_h[NN_INPUTS][NN_HIDDENS];
	double weights_h_o[NN_HIDDENS][NN_OUTPUTS];

	// Activations arrays
	double act_h[NN_HIDDENS];
	double act_o[NN_OUTPUTS];
	

	// STORRED ERRORS
	
	// Delta Weights arrays
	double d_weights_i_h[NN_INPUTS][NN_HIDDENS];
	double d_weights_h_o[NN_HIDDENS][NN_OUTPUTS];

	// Delta Output layer
	double d_output[NN_OUTPUTS];

	// Delta Hidden layer
	double d_hidden[NN_HIDDENS];


	double gen_error;
	double global_error;
	double max_glb_err;


} neunet_t;

//------------------------------------------------------------------------------
//Functions from preprocessing.c

void toGrayscale(SDL_Surface *image_surface, int w, int h);
void histogram(SDL_Surface *image_surface, int w, int h, long *histo);

void rotate(SDL_Surface *image, int w, int h, double angle);

int threshold(SDL_Surface *image_surface, int w, int h, long *histo);
void binarize(SDL_Surface *image_surface, int w, int h, long *histo);

void reduce_noise(SDL_Surface *is, int w, int h);
void contrast(SDL_Surface *im, int lvlc, int w, int h);

void preprocessing(char *path);
SDL_Surface* preprocessing_SDL(char *path);
SDL_Surface* preprocessing_dataset(char *path);

//------------------------------------------------------------------------------
//Functions from structures.c

//To initialize
lineZones init_lineZones(int nbLines);
line init_line(int nbLetters);
doc init_doc(int nbLines);

//To free
void free_doc(doc image);
void free_docMat(docMat m);

//To test
void print_line(line *l);

char* line_string(SDL_Surface *surface, line *l, neunet_t *nn);
char* doc_string(SDL_Surface *surface, doc *image, neunet_t *nn);

void print_doc(doc *image);
void print_matrix(matrix m);


//------------------------------------------------------------------------------
//Functions from utility.c

void verti_histo(SDL_Surface *image_surface, int *histo, coord rect);
void hori_histo(SDL_Surface *image_surface, int *histo, coord rect);

int avLenLetter(line l);
double avHeiLetter(line l);
void sepLetters(SDL_Surface *image, line l);

int avSpaceLetter(line *l);
void detectSpace(line *l);
void detectSpaceDoc(doc *image);

int r_sum_line(SDL_Surface *image_surface, int wMin, int wMax, int h);
int bot_sum_line(SDL_Surface *image_surface, int hMin, int hMax, int w);

int r_sum_letter(SDL_Surface *image_surface, int wMin, int wMax, int h);
int bot_sum_letter(SDL_Surface *image_surface, int hMin, int hMax, int w);

int count_get_lines(SDL_Surface *image_surface, coord rect);
void get_lines(SDL_Surface *image_surface, coord rect, lineZones all);

int count_get_letters(SDL_Surface *image_surface, coord rect);
void get_letters(SDL_Surface *image_surface, coord rect, line l);

double rect_ratio(coord r);
int count_groups(int *histo, int lenH);
tuples generate_groups(int *histo, int lenH);
int max_tuples(tuples t);
void hori_lines_define(SDL_Surface *image, int *vertHisto, coord rect, tuples t);


//------------------------------------------------------------------------------
//Functions from pixel_functions.c

Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void trace_hori_red_line(SDL_Surface *surface, int startH, int startW, int endH, int endW);
void trace_vert_red_line(SDL_Surface *surface, int startH, int startW, int endH, int endW);

void hori_lines(SDL_Surface *image_surface, int *vertHisto, coord rect);
void vert_lines(SDL_Surface *image_surface, int *hori_histo, coord rect);

int is_black(SDL_Surface *image_surface, int w, int h);
int is_red(SDL_Surface *image_surface, int w, int h);

//------------------------------------------------------------------------------
//Functions from matrix_letters.c

matrix buildMatrix(SDL_Surface *image, coord let);

lineMat buildLineMat(SDL_Surface *image, line l);

docMat buildDocMat(SDL_Surface *image, doc i);

//------------------------------------------------------------------------------
//Functions from resizing.c

struct ratio* normalized_ratio(int width, int height, float n_ratio);
matrix interpolation(matrix *m, int n_width, int n_height);
void  m_fill(matrix* m, int new_size);
matrix normalization(matrix* m);
void print_m(matrix m);

//------------------------------------------------------------------------------
//Functions from segmentation.c

void segmentation(char *path, neunet_t *nn);
char* segmentation_SDL(SDL_Surface * image_surface, neunet_t *nn);

char* ocr(char * image_path, char* neunet_path);
double* dataset(char *path);

//------------------------------------------------------------------------------
//Functions from neunet.c

// Neural net initialisation function
neunet_t *init_neunet();

// Main neural net function
char neural_net_ask(neunet_t *nn, double *inputs);

// Training functions
void neunet_train(neunet_t *nn, double *in, double *out, double lr);
//Give the expected character
char expected_output(size_t size,double *expOut);

// Get error of the neural net
double neunet_get_error(neunet_t *nn);


//------------------------------------------------------------------------------
//Functions from neural_main.c

void neural_net_run_training(char *nn_path, char *data_set, int set_size, int gens);

//------------------------------------------------------------------------------
//Function from ocr_ui.c

void launch_GUI();

//------------------------------------------------------------------------------
//Functions from neuralIO.c
void neuralNetToFile(neunet_t *nn, char *path);
neunet_t *fileToNeuralNet(char *path);

//------------------------------------------------------------------------------
// Functions from neunet_HG/utils.c
double sigmoid(double x);
double dSigmoid(double x);

// Return a random value in 0-1
double uniform(); 

double xavier_init(int n);


double dot_product(double *a, double *b, int len);

void apply_fct(double *a, double (*fvt)(double), int n);


void swap(int *a, int *b);

void shuffledList(int size, int *dst);

//------------------------------------------------------------------------------
// Functions from load_set.c

void print_output(double *out);
void print_input(double *in);
size_t load_dataset(char *path, size_t max_len, double *inputs, double *outputs);


//#############################################################################
//#############################################################################
//							USER INTERFACE
//#############################################################################
//#############################################################################



// Funcions from ocr_ui.c
void launch_GUI();



//------------------------------------------------------------------------------
// Functions and structures from ui_utils.c

typedef struct
{
	GtkWindow *main_window;

	GtkImage *img_display;
	GdkPixbuf *img_pix_buf;

	GtkTextView *txt_display;
	GtkTextBuffer *txt_buffer;
} UI_t;

typedef struct
{
	char* img_path;
	char* nn_path;
	UI_t ui;
} data_t;



char *ask_file_path(GtkWindow *parent_window);


void load_img_pixbuf(gpointer userdata);
void display_image(gpointer userdata);









#endif
