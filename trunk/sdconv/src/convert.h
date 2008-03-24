// convert.h

#ifndef MDK_CONVERT_H
#define MDK_CONVERT_H

#include <stdio.h>

struct convert_module {
    const char *name;
    int req_file;                               /* need to specify another file */
    bool (*init)(const char *file);
    void (*convert)(FILE *fp, gchar *data);
    void (*fini)();
};

struct convert_module *mdk_get_convert_module(const char *name);
void mdk_convert_with_module(struct convert_module *mod, 
                             FILE *fp, gchar *data);

#endif

