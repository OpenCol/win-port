/* VList::VList() -- shipped at 1008:e84f.
 *
 * Ported by tools/port.py from matched/game/VList_ctor_1008_e84f.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class VListItem {
public:
    col_i32 f0, f4, f8;
    VListItem();
    ~VListItem();
};
class VList : public VListItem {
public:
    VList();
    ~VList();
};

VList::VList()
{
}
