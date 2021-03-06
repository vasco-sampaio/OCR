# include <gtk/gtk.h>
# include <string.h>

# include "../types.h"

# define GLD_NAME "UI/UI.glade"



void on_save_clicked(GtkButton *btn, gpointer user_data)
{
	g_print("Pressed save button\n");
	btn = btn;

	save_text_to_file(user_data);
}



void on_load_image_click(GtkButton *btn, gpointer user_data)
{
	g_print("Pressed load_image button\n");
	btn = btn; // remove unused warning

	data_t *app_data = user_data;
	char *filename = ask_file_path(app_data->ui.main_window, GTK_FILE_CHOOSER_ACTION_OPEN);
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
	btn = btn; // remove unused warning

	data_t *app_data = user_data;
	char *filename = ask_file_path(app_data->ui.main_window, GTK_FILE_CHOOSER_ACTION_OPEN);
	if(filename)
	{
		app_data->nn_path = filename;
	}
}


void on_run_OCR_click(GtkButton *btn, gpointer user_data)
{
	g_print("Pressed run button\n");
	btn = btn; // remove unused warning

	data_t *app_data = user_data;
	char *s = ocr(app_data->img_path, app_data->nn_path);
	int size = strlen(s);
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(app_data->ui.txt_buffer), s, size);
}


void on_window_close_click(GtkButton *btn, gpointer user_data)
{
	g_print("Pressed close\n");
	btn = btn;

	data_t *app_data = user_data;
	
	free(app_data->img_path);
	free(app_data->nn_path);
	

	//gtk_window_close(app_data->ui.main_window);
	gtk_main_quit();
}


void launch_GUI()
{
	gtk_init(NULL, NULL);

	GtkBuilder *builder = gtk_builder_new();

	GError *error = NULL;
	if(gtk_builder_add_from_file(builder, GLD_NAME, &error) == 0)
	{
		g_printerr("Error loading file : %s\n", error->message);
		g_clear_error(&error);
		return;
	}

	GtkWindow *main_window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.ocr"));
	GtkButton *load_img_button = GTK_BUTTON(gtk_builder_get_object(builder, "load_img_button"));
	GtkButton *load_nn_button = GTK_BUTTON(gtk_builder_get_object(builder, "load_nn_button"));
	GtkButton *run_OCR_button = GTK_BUTTON(gtk_builder_get_object(builder, "run_OCR_button"));
	GtkButton *close_button = GTK_BUTTON(gtk_builder_get_object(builder, "close_button"));
	GtkButton *save_button = GTK_BUTTON(gtk_builder_get_object(builder, "save_text_button"));

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
			.img_pix_buf = NULL,
			.txt_display = txt_display,
			.txt_buffer = txt_buffer
		}
	};

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(load_img_button, "clicked", G_CALLBACK(on_load_image_click), &app_data);
	g_signal_connect(load_nn_button, "clicked", G_CALLBACK(on_load_neural_net_click), &app_data);
	g_signal_connect(run_OCR_button, "clicked", G_CALLBACK(on_run_OCR_click), &app_data);
	g_signal_connect(close_button, "clicked", G_CALLBACK(on_window_close_click), &app_data);
	g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_clicked), &app_data);

	g_print("Running GUI\n");
	g_object_unref(builder);
	gtk_main();
	return;
}
