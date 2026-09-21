/* PanelWin::PanelWin() -- shipped at 1008:dd64.
 *
 * Ported by tools/port.py from matched/game/PanelWin_ctor.c.
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
    char body[0x26];
    GRPort();
    ~GRPort();
};
class ColChild {
public:
    char body[0x77a - 2];
    ColChild();
    ~ColChild();
    virtual void v0();
};
class SRegion {
public:
    char body[0x2e];
    SRegion();
    ~SRegion();
};
class SImageButton {
public:
    char body[0x74];
    SImageButton();
    ~SImageButton();
};
class SButton {
public:
    char body[0x22];
    SButton();
    ~SButton();
};
class PanelWin : public ColChild {
public:
    SRegion region; /* +0x77a */
    GRPort p1; /* +0x7a8 */
    GRPort p2; /* +0x7ce */
    SImageButton ib[3]; /* +0x7f4 */
    SButton b[2]; /* +0x950 */
    SImageButton ib2; /* +0x994 */
    char tail[0xa14 - 0xa08];
    PanelWin();
    ~PanelWin();
    virtual void v0();
};





PanelWin::PanelWin()
{
}
