// sdconv.cpp

#include <stdio.h>
#include <sys/stat.h>
#include <mdk.h>

#define DEBUG_OUT_ENTRIES   3

void show_usage()
{
    fprintf(stderr, "usage: sdconv [options] [script.py/rb/lua] <dict.ifo> <output.xml>\n\n"
                    "Available options:\n"
                    "   -m <module>: select convert module, available: python\n"
                    "   -d         : debug mode\n"
                    "   -h         : show this help page\n\n"
                    "For additional information, see http://mac-dictionary-kit.googlecode.com/\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    int i = 1, debug_mode = 0;
    FILE *fp;
    char *outfile, *module_name = "default", *module_file = NULL;

    if (argc < 3)
        show_usage();

    // process options in arguments
    while (i < argc)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            debug_mode = 1;
            i++;
        }

        else
        if (strcmp(argv[i], "-m") == 0)
        {
            i++;

            if (i >= argc)
            {
                fprintf(stderr, "-m options must come with a module name.\n");
                return 1;
            }

            module_name = argv[i++];
        }

        else
        if (strcmp(argv[i], "-h") == 0)
            show_usage();

        else
            break;
    }

    struct convert_module *mod = mdk_get_convert_module(module_name);
    if (! mod)
    {
        fprintf(stderr, "module '%s' not found.\n", module_name);
        return 1;
    }

    if (mod->req_file)
    {
        struct stat st;

        if (argc - i < 1)
            show_usage();

        module_file = argv[i++];
        if (stat(module_file, &st) != 0)
        {
            fprintf(stderr, "specified module script '%s' not found.\n", 
                    module_file);
            return 1;
        }
    }

    if (mod->init)
    {
        bool ret = mod->init(module_file);
        if (ret != true)
        {
            fprintf(stderr, "%s: initialize module %s failed.\n", 
                    argv[0], module_file);
            return 1;
        }
    }

    if (debug_mode && argc - i < 1 || 
        ! debug_mode && argc - i < 2)
        show_usage();

    const char *path = argv[i++];
    const std::string url(path);
    int count;
    mdk_dict *dict = new mdk_dict;

    setlocale(LC_ALL, "");

    if (dict->load(url) != true)
    {
        fprintf(stderr, "%s: load dictionary file '%s' failed.\n", argv[0], path);
        return 1;
    }

    count = dict->get_entry_count();
    printf("%s %d\n", dict->dict_name().c_str(), count);

    if (debug_mode)
        count = DEBUG_OUT_ENTRIES;
    
    outfile = argv[i];
    fp = fopen(outfile, "w");
    if (! fp)
    {
        fprintf(stderr, "%s: write to output file '%s' failed.\n", argv[0], outfile);
        return 1;
    }

    GString *dest = mdk_start_convert(mod);

    for (i = 0; i < count; i++)
        mdk_convert_index_with_module(mod, dict, i, dest);

    mdk_finish_convert(mod, dest);
    fprintf(fp, "%s", dest->str);
    g_string_free(dest, TRUE);
    fclose(fp);

    if (mod->fini)
        mod->fini();
    
    return 0;
}

