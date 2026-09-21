/* EuropeWin::~EuropeWin() -- shipped at 1008:e02f.
 *
 * Ported by tools/port.py from matched/game/EuropeWin_dtor.c.
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
class SRegion { public: char body[0x7aa - 0x77c]; SRegion(); ~SRegion(); };
class SButton { public: char body[0x22]; SButton(); ~SButton(); };
class SImageButton { public: char body[0x884 - 0x810]; SImageButton(); ~SImageButton(); };
class GRPort { public: char body[0x26]; GRPort(); ~GRPort(); };
class EuropeWin : public ColChild {
public:
    col_i16 f77a;
    SRegion region;
    SButton b1, b2, b3;
    SImageButton image;
    GRPort port;
    EuropeWin();
    ~EuropeWin();
    virtual void v0();
};

EuropeWin::~EuropeWin()
{
}
