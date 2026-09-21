/* MagicWin::~MagicWin() -- shipped at 1068:a70b.
 *
 * Ported by tools/port.py from matched/gfx/MagicWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class GameWin {
public:
    char body[0xc4 - 2];
    GameWin();
    ~GameWin();
    virtual void v0();
};
class GRPort {
public:
    char body[0x26];
    GRPort();
    ~GRPort();
};
class MagicWin : public GameWin {
public:
    GRPort port;
    char pad[0xfa - 0xea];
    char fa;
    char fb;
    col_i32 ffc;
    MagicWin();
    ~MagicWin();
    virtual void v0();
};

MagicWin::~MagicWin()
{
}
