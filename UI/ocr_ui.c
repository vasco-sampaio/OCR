# include <gtk/gtk.h>

# include "ui_utils.h"

# define GLD_NAME "UI.glade"








void on_load_image_click(GtkButton *btn, gpointer user_data)
{
	g_print("Pressed load_image button\n");

	data_t *app_data = user_data;
	char *filename = ask_file_path(app_data->ui.main_window);
	if(filename)
	{
		app_data->img_path = filename;
		load_img_pixbuf(user_data);
		if(app_data->ui.img_pix_buf != NULL)
			display_image(user_data);
	}
}


int main(int argc, char **argv)
{
	gtk_init(NULL, NULL);

	GtkBuilder *builder = gtk_builder_new();

	GError *error = NULL;
	if(gtk_builder_add_from_file(builder, GLD_NAME, &error) == 0)
	{
		g_printerr("Error loading file : %s\n", error->message);
		g_clear_error(&error);
		return 1;
	}

	GtkWindow *main_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.ocr"));
	GtkButton *load_img_button = GTK_BUTTON(gtk_builder_get_object(builder, "load_img_button"));
	GtkButton *load_nn_button = GTK_BUTTON(gtk_builder_get_object(builder, "load_nn_button"));
	GtkButton *run_OCR_button = GTK_BUTTON(gtk_builder_get_object(builder, "run_OCR_button"));

	GtkViewport *img_viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "img_viewport"));
	GtkImage *img_display = GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	GtkLabel *txt_display = GTK_LABEL(gtk_builder_get_object(builder, "text_display"));

	data_t app_data =
	{
		.img_path = NULL,
		.nn_path = NULL,
		.ui =
		{
			.main_window = main_window,
			.img_display = img_display,
			.img_viewport = img_viewport,
			.img_pix_buf = NULL
		}
	};

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(load_img_button, "clicked", G_CALLBACK(on_load_image_click), &app_data);

	g_print("TEST");
	gtk_main();
	return 0;
}
