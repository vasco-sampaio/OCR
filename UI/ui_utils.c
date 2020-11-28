# include <gtk/gtk.h>






char *ask_file_path(GtkWindow *parent_window)
{

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    GError *error;
    gboolean read_file_status;
	char *filename = NULL;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          parent_window,
                                          action,
                                          ("_Cancel"),
                                          GTK_RESPONSE_CANCEL,
                                          ("_Open"),
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        g_print("Success\n");
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("%s\n",filename);
		char *contents;
        read_file_status = g_file_get_contents(filename,&contents,NULL, &error);
        g_print("%d", read_file_status);
        
        if (read_file_status == FALSE) {
            g_error("error opening file: %s\n",error && error->message ? error->message : "No Detail");
            return NULL;
        }

    }
    gtk_widget_destroy (dialog);

	return filename;
}
