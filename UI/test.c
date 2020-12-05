
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

/* Event callbacks */
static gboolean keyPress(GtkWidget *widget, gpointer data);
static gboolean sizeChanged(GtkWidget *widget, GtkAllocation
*allocation, gpointer data);

static    GtkWidget *image;		/* As displayed on the screen */

int
main(int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *viewport;
    GdkPixbuf *sourcePixbuf = NULL;	/* As read from a file */
    char *filename =  (argc > 1) ? argv[1] : "/home/renji/Downloads/coding.jpg";

    gtk_init(&argc, &argv);

    /* Make pixbuf, then make image from pixbuf because
     * gtk_image_new_from_file() doesn't flag errors */
    {
	GError *error = NULL;
	sourcePixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (sourcePixbuf == NULL) {
	    g_message("%s", error->message);
	    return 1; /* exit() */
	}
    }

    /* On expose/resize, the image's pixbuf will be overwritten
     * but we will still need the original image so take a copy of it */
    image = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(sourcePixbuf));

    viewport = gtk_scrolled_window_new(NULL, NULL);
    /* Saying "1x1" reduces the window's minumum size from 55x55 to 42x42. */
    gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(viewport), 1);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(viewport),
1);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "image1-gtk3");

    /* Quit if they ask the window manager to close the window */
    g_signal_connect(G_OBJECT(window), "destroy",
		     G_CALLBACK(gtk_main_quit), NULL);
    /* Quit on control-Q. */
    g_signal_connect(window, "key-press-event", G_CALLBACK(keyPress), NULL);

    /* When the window is resized, scale the image to fit */
    g_signal_connect(viewport, "size-allocate",
G_CALLBACK(sizeChanged), sourcePixbuf);

    /* The image is in a scrolled window container so that the main window
     * can be resized smaller than the current image. */
    gtk_container_add(GTK_CONTAINER(viewport), image);
    gtk_container_add(GTK_CONTAINER(window), viewport);

    //gtk_window_set_resizable(GTK_WINDOW(window), 1);
    /* Open the window the same size as the image */
    gtk_window_set_default_size(GTK_WINDOW(window),
	gdk_pixbuf_get_width(sourcePixbuf),
	gdk_pixbuf_get_height(sourcePixbuf));

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

/* Callback functions */

/* Check for Control-Q and quit if it was pressed */
static gboolean
keyPress(GtkWidget *widget, gpointer data)
{
    GdkEventKey *event = (GdkEventKey *) data;

    if (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK)) {
	gtk_main_quit();
	return FALSE;
    } else
	return TRUE;
}

/* If the window has been resized, that resizes the scrolledwindow,
 * and we scale the image to the dimensions of the scrolledwindow so that
 * the scrollbars disappear again. Yuk! */
static gboolean
sizeChanged(GtkWidget *widget, GtkAllocation *allocation, gpointer data)
{
    GdkPixbuf *sourcePixbuf = data;	/* As read from a file */
    GdkPixbuf *imagePixbuf;	/* pixbuf of the on-screen image */

    imagePixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
    if (imagePixbuf == NULL) {
	g_message("Can't get on-screen pixbuf");
	return TRUE;
    }
    /* Recreate the displayed image if the image size has changed. */
    if (allocation->width != gdk_pixbuf_get_width(imagePixbuf) ||
        allocation->height != gdk_pixbuf_get_height(imagePixbuf)) {

	gtk_image_set_from_pixbuf(
	    GTK_IMAGE(image),
	    gdk_pixbuf_scale_simple(sourcePixbuf,
				    allocation->width,
				    allocation->height,
				    GDK_INTERP_BILINEAR)
	);
        g_object_unref(imagePixbuf); /* Free the old one */
    }

    return FALSE;
}
