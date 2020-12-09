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


void on_load_neural_net_click(GtkButton *btn, gpointer user_data)
{
	g_print("Pressed load_nn button\n");

	data_t *app_data = user_data;
	char *filename = ask_file_path(app_data->ui.main_window);
	if(filename)
	{
		app_data->nn_path = filename;
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

	GtkImage *img_display = GTK_IMAGE(gtk_builder_get_object(builder, "image_display"));
	GtkTextView *txt_display = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "text_display"));
	GtkTextBuffer *txt_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "ocr_text_buffer"));

	gtk_text_view_set_buffer(txt_display, txt_buffer);

	data_t app_data =
	{
		.img_path = NULL,
		.nn_path = NULL,
		.ui =
		{
			.main_window = main_window,
			.img_display = img_display,
			.img_pix_buf = NULL
		}
	};

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(load_img_button, "clicked", G_CALLBACK(on_load_image_click), &app_data);
	g_signal_connect(load_nn_button, "clicked", G_CALLBACK(on_load_neural_net_click), &app_data);

	g_print("TEST");
	gtk_main();
	return 0;
}
