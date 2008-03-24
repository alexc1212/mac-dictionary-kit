// convert.cpp: convert modules

#include "dict.h"
#include "convert.h"
#include "python.h"
#include "index.h"

inline void convert_with_glib(FILE *fp, gchar *source)
{
    fprintf(fp, "%s", source);
}

struct convert_module convert_module_list[] = {
    { "default", 0, NULL,        convert_with_glib,   NULL        }, 
    { "python",  1, init_python, convert_with_python, fini_python }, 
    { NULL,      0, NULL,        NULL,                NULL        },
};

struct convert_module *mdk_get_convert_module(const char *name)
{
    int i;

    for (i = 0; convert_module_list[i].name != NULL; i++)
        if (strcmp(convert_module_list[i].name, name) == 0)
            return &convert_module_list[i];

    return NULL;
}

void mdk_convert_with_module(struct convert_module *mod, 
                             FILE *fp, gchar *data)
{
    guint32 data_size, sec_size;
    int first = 1;
    std::string mark;    

    data_size = get_uint32(data);
    data += sizeof(guint32);

    const gchar *p = data;

    while (guint32(p - data) < data_size)
    {
        if (first)
            first = 0;
        else
            mark += "\n";

        switch (*p)
        {
			case 'm':
			case 'l':
				p++;
				sec_size = strlen(p);

				if (sec_size)
                {
					fprintf(fp, "<pre>\n");
					gchar *m_str = g_markup_escape_text(p, sec_size);
                    mod->convert(fp, m_str);
					g_free(m_str);
					fprintf(fp, "\n</pre>\n");
				}

				sec_size++;
				break;

			case 'g':
				p++;
				sec_size = strlen(p);
				if (sec_size)
                {
					fprintf(fp, "<pre>\n");
					fprintf(fp, "%s", p);
					fprintf(fp, "\n</pre>");
                }
				sec_size++;
				break;

			case 'x':
			case 'k':
			case 'w':
			case 'h':
				p++;
				sec_size = strlen(p) + 1;

                fprintf(fp, "<p class=\"error\">Format not supported.</p>");
				break;

			case 't':
				p++;
				sec_size = strlen(p);
				if (sec_size)
                {
					fprintf(fp, "<div class=\"t\">");
					gchar *m_str = g_markup_escape_text(p, sec_size);
					fprintf(fp, "%s", m_str);
					g_free(m_str);
					fprintf(fp, "</div>");
				}
				sec_size++;
				break;

			case 'y':
				p++;
				sec_size = strlen(p);
				if (sec_size)
                {
					fprintf(fp, "<div class=\"y\">");
					gchar *m_str = g_markup_escape_text(p, sec_size);
					fprintf(fp, "%s", m_str);
					g_free(m_str);
					fprintf(fp, "</div>");
				}
				sec_size++;
				break;

			case 'W':
				p++;
				sec_size = ntohl(get_uint32(p));
				// enable sound button.
				sec_size += sizeof(guint32);
				break;

			case 'P':
				p++;
				sec_size = ntohl(get_uint32(p));
				if (sec_size) {
					// TODO: extract images from here
					fprintf(fp, "<span foreground=\"red\">[Missing Image]</span>");
				} else {
					fprintf(fp, "<span foreground=\"red\">[Missing Image]</span>");
				}

                sec_size += sizeof(guint32);
			    break;

			default:
				if (g_ascii_isupper(*p))
                {
					p++;
					sec_size = ntohl(get_uint32(p));
					sec_size += sizeof(guint32);
				} else {
					p++;
					sec_size = strlen(p) + 1;
				}

				fprintf(fp, "<p class=\"error\">Unknown data type.</p>");
				break;
		}

		p += sec_size;
    }
}

