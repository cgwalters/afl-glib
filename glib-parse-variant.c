#include <glib.h>
#include <string.h>

int
main (int argc, char **argv)
{
  const char *fname = argv[1];
  const char *fmt;
  int r;
  char *buf;
  char *aligned_buf;
  gsize len, vlen;
  GError *error = NULL;
  GVariant *container;
  GVariant *vay;
  GVariant *v;

  if (!g_file_get_contents (fname, &buf, &len, &error))
    goto out;

  container = g_variant_new_from_data (G_VARIANT_TYPE ("(say)"), (guint8*)buf, len,
				       FALSE, NULL, NULL);
  g_variant_get (container, "(&s@ay)", &fmt, &vay);

  if (!g_variant_type_string_is_valid (fmt))
    {
      g_printerr ("Invalid format string: %s\n", fmt);
      return 1;
    }
  if (!g_variant_type_is_definite ((GVariantType*)fmt))
    {
      g_printerr ("Indefinite format string: %s\n", fmt);
      return 1;
    }

  g_print ("Loading: %s %p\n", fmt, (gpointer)vay);
  vlen = g_variant_get_size (vay);
  r = posix_memalign (&aligned_buf, sizeof(void*), vlen);
  g_assert (r == 0);
  memcpy (aligned_buf, g_variant_get_data (vay), vlen);
  v = g_variant_new_from_data (G_VARIANT_TYPE (fmt),
			       aligned_buf,
			       vlen,
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
