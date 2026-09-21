/* DiskFile::DiskFile() -- shipped at 1068:9285.
 *
 * Ported by tools/port.py from matched/gfx/DiskFile_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class DiskFile {
public:
    char body[0x8c];
    DiskFile();
    ~DiskFile();
};
extern "C" void * memset_rtl(void *p, col_i16 c, col_u16 n);

DiskFile::DiskFile()
{
    memset_rtl(this, 0, 0x8c);
}
