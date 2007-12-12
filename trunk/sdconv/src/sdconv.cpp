// sdconv.cpp

#include <stdio.h>
#include "dict.h"

#define get_uint32(x) *reinterpret_cast<const guint32 *>(x)
void output_stardict_data(FILE *fp, gchar *data);

int main(int argc, char *argv[])
{
    sd_dict *dict = new sd_dict;
    FILE *fp;

    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <dict.ifo> <output.xml>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    const std::string url(path);
    sd_entry entry;
    guint32 count, i;

    setlocale(LC_ALL, "");

    if (dict->load(url) != true)
    {
        fprintf(stderr, "%s: load %s failed\n", argv[0], argv[1]);
        return 1;
    }

    count = dict->get_entry_count();
    printf("%s %d\n", dict->dict_name().c_str(), count);
    fp = fopen(argv[2], "w");
    if (! fp)
    {
        fprintf(stderr, "%s: write to %s failed\n", argv[0], argv[2]);
        return 1;
    }

    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<d:dictionary xmlns=\"http://www.w3.org/1999/xhtml\" "
            "xmlns:d=\"http://www.apple.com/DTDs/DictionaryService-1.0.rng\">\n\n");

    for (i = 0; i < count; i++)
    {
        dict->get_entry_by_index(i, &entry);
		
        gchar *m_str = g_markup_escape_text(entry.key, strlen(entry.key));
 
        fprintf(fp, "<d:entry id=\"%d\" d:title=\"%s\">\n"
                "<d:index d:value=\"%s\"/>\n"
                "<h1>%s</h1>\n", i, m_str, m_str, m_str);
        
        g_free(m_str);

        gchar *data = dict->get_entry_data(&entry);
        output_stardict_data(fp, data);
        g_free(data);

        fprintf(fp, "</d:entry>\n\n");
    }

    fprintf(fp, "</d:dictionary>\n");
    fclose(fp);
    
    return 0;
}

void output_stardict_data(FILE *fp, gchar *data)
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
				if (sec_size) {
					mark += "<pre>\n";
					gchar *m_str = g_markup_escape_text(p, sec_size);
					mark += m_str;
					g_free(m_str);
					mark += "\n</pre>";
				}
				sec_size++;
				break;

			case 'g':
				p++;
				sec_size = strlen(p);
				if (sec_size)
                {
					mark += "<pre>\n";
					mark += p;
					mark += "\n</pre>";
                }
				sec_size++;
				break;

			case 'x':
				p++;
				sec_size = strlen(p) + 1;
				mark+= "<p class=\"error\">XDXF data parsing plug-in is not found!</p>";
				break;

			case 'k':
				p++;
				sec_size = strlen(p) + 1;
				mark+= "<p class=\"error\">PowerWord data parsing plug-in is not found!</p>";
				break;

			case 'w':
				p++;
				sec_size = strlen(p) + 1;
				mark+= "<p class=\"error\">Wiki data parsing plug-in is not found!</p>";
				break;

			case 'h':
				p++;
				sec_size = strlen(p) + 1;
				mark+= "<p class=\"error\">HTML data parsing plug-in is not found!</p>";
				break;

			case 't':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					mark += "<div class=\"t\">";
					gchar *m_str = g_markup_escape_text(p, sec_size);
					mark += m_str;
					g_free(m_str);
					mark += "</div>";
				}
				sec_size++;
				break;

			case 'y':
				p++;
				sec_size = strlen(p);
				if (sec_size) {
					mark += "<div class=\"y\">";
					gchar *m_str = g_markup_escape_text(p, sec_size);
					mark += m_str;
					g_free(m_str);
					mark += "</div>";
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
					mark += "<span foreground=\"red\">[Missing Image]</span>";
				} else {
					mark += "<span foreground=\"red\">[Missing Image]</span>";
				}
				
                sec_size += sizeof(guint32);
			    break;

			default:
				if (g_ascii_isupper(*p)) {
					p++;
					sec_size = ntohl(get_uint32(p));
					sec_size += sizeof(guint32);
				} else {
					p++;
					sec_size = strlen(p) + 1;
				}

				mark += "<p class=\"error\">Unknown data type.</p>";
				break;
		}
		p += sec_size;
    }

    fprintf(fp, "%s\n", mark.c_str());
}

