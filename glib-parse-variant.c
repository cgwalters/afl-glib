#include <glib.h>

int
main (int argc, char **argv)
{
  const char *fname = argv[1];
  const char *fmt;
  char *buf;
  gsize len;
  GError *error = NULL;
  GVariant *container;
  GVariant *vay;
  GVariant *v;

  if (!g_file_get_contents (fname, &buf, &len, &error))
    goto out;

  container = g_variant_new_from_data (G_VARIANT_TYPE ("(say)"), (guint8*)buf, len,
				       FALSE, NULL, NULL);
  g_variant_get (container, "(&s@ay)", &fmt, &vay);
  g_print ("Loading: %s\n", fmt);
  v = g_variant_new_from_data (G_VARIANT_TYPE (fmt),
			       g_variant_get_data (vay),
			       g_variant_get_size (vay),
			       FALSE, NULL, NULL);

  (void) g_variant_print (v, TRUE);
  (void) g_variant_print (v, FALSE);
  
 out:
  if (error)
    { 
      g_printerr ("%s\n", error->message);
      return 1;
    }
  return 0;
}
