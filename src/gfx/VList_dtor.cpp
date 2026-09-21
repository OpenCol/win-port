/* VList::~VList() -- shipped at 1068:9d79.
 *
 * Ported by tools/port.py from matched/gfx/VList_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class VList {
public:
    col_i32 f0, f4, f8;
    VList();
    ~VList();
};
extern "C" void list_clear(VList *l);

VList::~VList()
{
    list_clear(this);
}
