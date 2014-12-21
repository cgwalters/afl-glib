#include <glib.h>

int
main (int argc, char **argv)
{
  const char *fname = argv[1];
  GKeyFile *keyfile = g_key_file_new ();
  GError *error = NULL;

  if (!g_key_file_load_from_file (keyfile, fname, 0, &error))
    goto out;
  
 out:
  if (error)
    { 
      g_printerr ("%s\n", error->message);
      return 1;
    }
  return 0;
}
