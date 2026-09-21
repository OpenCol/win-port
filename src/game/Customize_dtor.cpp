/* Customize::~Customize() -- shipped at 1030:ea51.
 *
 * Ported by tools/port.py from matched/game/Customize_dtor.c.
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
class SButton {
public:
    char body[0x22];
    SButton();
    ~SButton();
};
class GRPort {
public:
    char body[0x26];
    GRPort();
    ~GRPort();
};
class Customize : public GameWin {
public:
    char pad[0x70];
    SButton b1;
    SButton b2;
    GRPort port;
    char tail[6];
    Customize();
    ~Customize();
    virtual void v0();
};

Customize::~Customize()
{
}
