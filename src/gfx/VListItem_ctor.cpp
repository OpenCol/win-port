/* VListItem::VListItem() -- shipped at 1068:9c48.
 *
 * Ported by tools/port.py from matched/gfx/VListItem_ctor.c.
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
    col_i32 f0;
    col_i32 f4;
    col_i32 f8;
    VListItem();
    ~VListItem();
};

VListItem::VListItem()
{
    f0 = 0;
    f4 = 0;
    f8 = 0;
}
