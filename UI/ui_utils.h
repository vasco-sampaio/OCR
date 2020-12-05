# ifndef _UI_UTILS_H
# define _UI_UTILS_H

# include <gtk/gtk.h>
# include <gdk/gdkpixbuf.h>

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


# endif	//_UI_UTILS_H
