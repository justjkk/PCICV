#include<gtk/gtk.h>

#include "sigextract.h"
#include "video_decode.h"

GtkBuilder      *builder;
GtkWidget       *window;
GtkFileChooserButton *fcButton;
GtkLabel *lblResults;

void on_window_destroy(GtkObject *object, gpointer userdata)
{
  gtk_main_quit();
}

void processVideo(void *filename)
{
  T[SIMPLE] = T[MODERATE] = T[COMPLEX] = 0;
  gchar buff[1024];
  ticm = allocTICM();
  dct = allocDCT();
  decodeVideo((char *)filename, &processFrame);
  sprintf(buff, "Signature = (%d, %d, %d)\n", T[SIMPLE], T[MODERATE], T[COMPLEX]);
  gtk_label_set_text (lblResults, (const gchar *)buff);
  deallocTICM(ticm);
  deallocDCT(dct);
}

void on_btnSigExtract_clicked(GtkObject *object, gpointer userdata)
{
  GError *err = NULL;
  const char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (fcButton));
  if(filename == NULL)
    return;

  gtk_label_set_text(lblResults, "Processing... Please wait.");
  GThread *thread = g_thread_create((GThreadFunc)processVideo, (void *)filename, TRUE, &err);
  if(thread == NULL)
  {
    printf("Thread create failed: %s!!\n", err->message);
    g_error_free(err);
  }
  else
  {
    //g_thread_join(thread);
  }
  
}

int main(int argc, char **argv)
{

  g_thread_init(NULL);
  gdk_threads_init();
  gtk_init (&argc, &argv);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "layout.glade", NULL);
  window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
  fcButton = GTK_FILE_CHOOSER_BUTTON (gtk_builder_get_object (builder, "fcVideoFile"));
  lblResults = GTK_LABEL (gtk_builder_get_object (builder, "lblResults"));
  gtk_builder_connect_signals (builder, NULL);

  g_object_unref (G_OBJECT (builder));

  gtk_widget_show (window);
  gtk_main ();

  return 0;
}
