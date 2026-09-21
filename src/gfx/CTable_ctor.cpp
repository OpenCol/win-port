/* CTable::CTable() -- shipped at 1068:004e.
 *
 * Ported by tools/port.py from matched/gfx/CTable_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class CTable {
public:
    char body[0x404];
    col_i16 hpal;
    char pad[0x414 - 0x406];
    char f414, f415, f416, f417;
    col_i16 f418, f41a, f41c;
    CTable();
    ~CTable();
};
extern "C" void palette_init(CTable *t);
extern "C" col_i16 create_palette(CTable *t);
extern "C" void seed_nonzero_random(CTable *t);

CTable::CTable()
{
    palette_init(this);
    hpal = create_palette(this);
    seed_nonzero_random(this);
    f414 = 0;
    f415 = 0;
    f416 = 0;
    f418 = f41a = f41c = 0;
}
