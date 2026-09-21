/* Flic::~Flic() -- shipped at 1068:1df1.
 *
 * Ported by tools/port.py from matched/gfx/Flic_dtor.c.
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
    void *f232;
    col_i16 f236;
    col_u16 f238;
    void *f23a;
    char pad_23e[0x558 - 0x23e];
    col_u16 f558, f55a;
    void *f55c;
    void *f560;
    char pad_564[0x578 - 0x564];
    Flic();
    ~Flic();
    virtual void v0();
};
extern "C" {
extern void * MSHUnlock(col_u16 h);
extern col_i16 MSDisposeHandle(col_u16 h);
/* same object: 4-byte `push cs; call ` (rule 47) */
void free_resource_1068_9549(void *o) { }
}

Flic::~Flic()
{
    if (f23a != 0)
        f23a = (void *)MSHUnlock(f238);
    if (f238 != 0)
        MSDisposeHandle(f238);
    if (f55c != 0)
        f55c = MSHUnlock(f558);
    if (f558 != 0)
        MSDisposeHandle(f558);
    if (f560 != 0)
        f560 = MSHUnlock(f55a);
    if (f55a != 0)
        MSDisposeHandle(f55a);
    if (f232 != 0)
        free_resource_1068_9549(f232);
}
