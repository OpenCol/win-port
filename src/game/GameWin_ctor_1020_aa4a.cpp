/* GameWin::GameWin() -- shipped at 1020:aa4a.
 *
 * Ported by tools/port.py from matched/game/GameWin_ctor_1020_aa4a.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class Screen {
public:
    col_i16 f0; col_i32 f2; col_i32 f6; col_i16 fa; char fc;
    Screen();
    ~Screen();
};
class EventTable {
public:
    char body[0x5c];
    EventTable();
    ~EventTable();
};
class EventWin : public Screen, public EventTable {
public:
    char f69; col_i32 f6a; col_i16 f6e, f70, f72, f74, f76, f78, f7a; col_i32 f7c; col_i32 f80; col_i16 f84, f86; col_i32 f88;
    EventWin();
    ~EventWin();
};
class DialogWin : public EventWin {
public:
    col_i32 f8c, f90, f94;
    char f98;
    char f99;
    DialogWin();
    ~DialogWin();
};
class GRPort {
public:
    char body[0x24];
    GRPort();
    ~GRPort();
    virtual void v0();
};
class GameWin : public GRPort, public DialogWin {
public:
    col_i32 fc0;
    GameWin();
    ~GameWin();
    virtual void v0();
};


GameWin::GameWin()
{
    f8c = 0;
    fc0 = 0;
}
