/* CDControlWin::CDControlWin() -- shipped at 1048:e3d6.
 *
 * Ported by tools/port.py from matched/game/CDControlWin_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class ColChild {
public:
    char body[0x77a - 2];
    ColChild();
    ~ColChild();
    virtual void v0();
};
class SImageButton { public: char body[0x74]; SImageButton(); ~SImageButton(); };
class SButton { public: char body[0x22]; SButton(); ~SButton(); };
class CDControlWin : public ColChild {
public:
    SImageButton ib1, ib2, ib3, ib4, ib5;
    SButton b;
    col_i16 pad;
    SImageButton ib6;
    char tail[8];
    CDControlWin();
    ~CDControlWin();
    virtual void v0();
};



CDControlWin::CDControlWin()
{
}
