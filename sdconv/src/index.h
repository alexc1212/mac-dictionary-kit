// index.h

#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <string>
#include <glib.h>
#include "getuint32.h"

typedef struct sd_entry {
    const gchar *key;
    guint32 offset;
    guint32 size;
} sd_entry;

class sd_index {
public:
    sd_index();
	~sd_index();

	bool load(const gchar *file, 
              guint32 entry_count, 
              guint32 fsize);
    bool get_entry(guint32 index, sd_entry *entry);

    guint32 entry_count();

private:
	gchar               *entry_buffer;
	std::vector<gchar *> entry_list;
};

#endif

