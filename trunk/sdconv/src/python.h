// python.h

#include <glib.h>

bool init_python(const char *file);
void fini_python();
void convert_with_python(FILE *fp, gchar *source);

