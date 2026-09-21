/* EventTable::~EventTable() -- shipped at 1068:e6d2.
 *
 * Ported by tools/port.py from matched/gfx/EventTable_dtor_1068_e6d2.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class EventTable {
public:
    ~EventTable();
};

EventTable::~EventTable()
{
}
