#ifndef MAPFILE_H
#define MAPFILE_H

#define HAVE_MMAP 1

#ifdef HAVE_MMAP
#  include <sys/types.h>
#  include <fcntl.h>
#  include <sys/mman.h>
#  include <unistd.h>
#endif
#include <glib.h>

class MapFile {
public:
    MapFile(void) : 
#ifdef HAVE_MMAP
		mmap_fd(-1),
#endif
		data(NULL)
	{
	}
  
    ~MapFile();
    inline bool open(const char *file_name, unsigned long file_size);
    inline void close();
    inline gchar *begin(void) { return data; }
private:
#ifdef HAVE_MMAP
    int mmap_fd;
#endif
    char *data;
    unsigned long size;
};

inline bool MapFile::open(const char *file_name, unsigned long file_size)
{
  size=file_size;
#ifdef HAVE_MMAP
  if ((mmap_fd = ::open(file_name, O_RDONLY)) < 0) {
    //g_print("Open file %s failed!\n",fullfilename);
    return false;
  }
  fprintf(stderr, "file_size = %lu\n", file_size);
  data = (gchar *)mmap(NULL, file_size, PROT_READ, MAP_SHARED, mmap_fd, 0);
  if ((void *)data == (void *)(-1)) {
    //g_print("mmap file %s failed!\n",idxfilename);
    ::close(mmap_fd);
    data=NULL;
    return false;
  }
#else
  gsize read_len;
  if (!g_file_get_contents(file_name, &data, &read_len, NULL))
    return false;

  if (read_len!=file_size)
    return false;		
#endif

  return true;
}

inline void MapFile::close()
{
    if (!data)
        return;
#ifdef HAVE_MMAP
    munmap(data, size);
    ::close(mmap_fd);
#else
    g_free(data);
#endif

  data=NULL;
}

inline MapFile::~MapFile()
{
  close();
}

#endif

