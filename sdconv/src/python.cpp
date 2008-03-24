// python.cpp

#include <libgen.h>
#include <python2.5/Python.h>
#include "python.h"

PyObject *py_transform_func;

static char *copy_module_name_from_file(const char *file)
{
    char *module = strdup(basename((char *) file));
    if (! module)
        return NULL;

    int len = strlen(module);
    int i;

    for (i = len - 1; i >= 0; i--)
        if (module[i] == '.')
            module[i] = '\0';

    return module;
}

void add_directory_of_file_to_path(const char *file)
{
    PyRun_SimpleString("import sys");

    char buf[256];
    snprintf(buf, sizeof(buf), "sys.path.insert(0, '%s')", 
             dirname((char *) file));

    PyRun_SimpleString(buf);
}

bool init_python(const char *file)
{
    char *module_name = copy_module_name_from_file(file);
    if (! module_name)
        goto failed2;

    Py_Initialize();
    add_directory_of_file_to_path(file);
    
    fprintf(stderr, "loading module %s...\n", module_name);
    PyObject *module = PyImport_ImportModule(module_name);
    if (! module)
        goto failed1;

    fprintf(stderr, "locating function transform...\n");
    /* locate py_module.transform() */
    py_transform_func = PyObject_GetAttrString(module, 
                                               "transform");
    Py_DECREF(module);
    if (! py_transform_func)
        goto failed1;

    return true;

failed1:
    free(module_name);

failed2:
    return false;
}

void fini_python()
{
    Py_DECREF(py_transform_func);
    Py_Finalize();
}

void convert_with_python(FILE *fp, gchar *source)
{
    PyObject *pargs = Py_BuildValue("(s)", source);
    PyObject *pstr  = PyEval_CallObject(py_transform_func, 
                                        pargs);

    char *cstr;
    PyArg_Parse(pstr, "s", &cstr);

    fprintf(fp, "%s", cstr);

    Py_DECREF(pstr);
    Py_DECREF(pargs);
}

