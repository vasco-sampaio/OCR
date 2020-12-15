# include "../types.h"



// Open a file choosing window and return the path of the choosen file
char *ask_file_path(GtkWindow *parent_window, GtkFileChooserAction action)
{
	GtkWidget *dialog;
	//GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;
	char *filename = NULL;

	dialog = gtk_file_chooser_dialog_new ("Open File",
			parent_window,
			action,
			("_Cancel"),
			GTK_RESPONSE_CANCEL,
			("_Open"),
			GTK_RESPONSE_ACCEPT,
			NULL);

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),TRUE);

	res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		g_print("Success\n");
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		g_print("%s\n",filename);
	}
	gtk_widget_destroy (dialog);

	return filename;
}




// Load the image in img_path app into the pixbuf
void load_img_pixbuf(gpointer user_data)
{
	data_t *app = user_data;

	GError *error = NULL;

	app->ui.img_pix_buf = gdk_pixbuf_new_from_file(app->img_path, &error);
	if(app->ui.img_pix_buf == NULL)
	{
		g_message("%s", error->message);
		return;	
	}
}


// set the image shown with the one present in the image buffer
void display_image(gpointer user_data)
{
	data_t *app = user_data;

	if(app->ui.img_pix_buf != NULL)
	{
		int w_max = gtk_widget_get_allocated_width(GTK_WIDGET(app->ui.img_display));

		int h_max = gtk_widget_get_allocated_height(GTK_WIDGET(app->ui.img_display));

		int w = gdk_pixbuf_get_width(app->ui.img_pix_buf);
		int h = gdk_pixbuf_get_height(app->ui.img_pix_buf);

		int new_w = CLAMP(w, 0, w_max);
		int new_h = h * new_w / w;

		new_h = CLAMP(new_h, 100, h_max);
		new_w = w *new_h / h;

		g_print("%i,%i\n", w_max, h_max);
		g_print("%i,%i\n", w, h);
		g_print("%i,%i\n", new_w, new_h);

		app->ui.img_pix_buf = gdk_pixbuf_scale_simple(app->ui.img_pix_buf, 
				new_w, new_h, GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf(GTK_IMAGE(app->ui.img_display),
				app->ui.img_pix_buf);
				
	}
}


void save_text_to_file(gpointer user_data)
{
	data_t *app = user_data;
	
	char *file_path = ask_file_path(app->ui.main_window, GTK_FILE_CHOOSER_ACTION_SAVE);
	if(file_path)
	{
		FILE *fp = fopen(file_path, "w");

		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_bounds(app->ui.txt_buffer, &start, &end);

		char *text = gtk_text_buffer_get_text(app->ui.txt_buffer, &start, &end,  FALSE);
		fprintf(fp, text);

		fclose(fp);
	}

}


