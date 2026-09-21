/* EventTable::~EventTable() -- shipped at 1010:f0eb.
 *
 * Ported by tools/port.py from matched/game/EventTable_dtor_1010_f0eb.c.
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
