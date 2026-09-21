/* Flic::Flic() -- shipped at 1068:1cec.
 *
 * Ported by tools/port.py from matched/gfx/Flic_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class GRPort {
public:
    char body[0x26 - 2];
    GRPort();
    ~GRPort();
    virtual void v0();
};
class DiskFile { public: char body[0x8c]; DiskFile(); ~DiskFile(); };
class Flic : public GRPort {
public:
    char pad_26[0xa6 - 0x26];
    DiskFile file;
    char pad_132[0x232 - 0x132];
    col_i32 f232;
    col_i16 f236;
    col_i16 f238;
    col_i32 f23a;
    char pad_23e[0x24e - 0x23e];
    col_i16 f24e, f250;
    char pad_252[0x552 - 0x252];
    col_i16 f552, f554;
    char f556, f557;
    col_i16 f558, f55a;
    col_i32 f55c, f560;
    char pad_564[0x574 - 0x564];
    col_i32 f574;
    Flic();
    ~Flic();
    virtual void v0();
};

Flic::Flic()
{
    f24e = f250 = 0;
    f236 = -1;
    f23a = 0;
    f556 = 0;
    f552 = 0xa;
    f554 = 0xec;
    f574 = 0;
    f23a = 0;
    f238 = 0;
    f55c = 0;
    f558 = 0;
    f560 = 0;
    f55a = 0;
    f232 = 0;
}
