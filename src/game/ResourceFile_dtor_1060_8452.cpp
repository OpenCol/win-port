/* ResourceFile::~ResourceFile() -- shipped at 1060:8452.
 *
 * Ported by tools/port.py from matched/game/ResourceFile_dtor_1060_8452.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class ResourceFile {
public:
    col_i16 h;
    ResourceFile(char *name);
    ~ResourceFile();
};
extern "C" col_i16 open_resource_dll(char *name);
extern "C" void close_resource_dll(col_i16 h);

ResourceFile::~ResourceFile()
{
    close_resource_dll(h);
}
