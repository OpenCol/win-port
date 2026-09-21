/* AboutWin::~AboutWin() -- shipped at 1060:7758.
 *
 * Ported by tools/port.py from matched/game/AboutWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class GameWin { public: char body[0xc4 - 2]; GameWin(); ~GameWin(); virtual void v0(); };
class EventWin { public: char body[0x8c]; EventWin(); ~EventWin(); };
class CTable { public: char body[0x41e]; CTable(); ~CTable(); };
class GRPort { public: char body[0x26]; GRPort(); ~GRPort(); };
class Sprite { public: char body[0x20]; Sprite(); ~Sprite(); };
class AboutWin : public GameWin {
public:
    EventWin win;
    CTable table;
    GRPort p1, p2, p3;
    char pad[0x8bc - 0x5e0];
    Sprite s1;
    Sprite frames[0x15];
    Sprite s2;
    AboutWin();
    ~AboutWin();
    virtual void v0();
};

AboutWin::~AboutWin()
{
}
