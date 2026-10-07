/* PediaWin::~PediaWin() -- shipped at 1008:df80.
 *
 * Ported by tools/port.py from matched/game/PediaWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

#include "w16types.h"
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
extern "C" void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
extern "C" void list_unlink_by_id(void *list, col_i16 id);
extern "C" void MSCloseWindow(void *w);
class DlgControl {
public:
    col_i16 kind;
    col_i16 id;
    void *parent;
    RECT rect;
    col_i16 handle;
    void *next;
    char b16;
    char b17;
    col_i16 w18;
    DlgControl();
    ~DlgControl();
};
class SImageButton : public DlgControl {
public:
    char pad[0x22 - 0x1a];
    GRPort a;
    GRPort b;
    SImageButton();
    ~SImageButton();
};
extern "C" void image_button_release(col_i16 h);
class ColChild : public GameWin {
public:
    char pad2[0x6ea - 0xc4];
    SImageButton btn;
    char rest[0x77a - 0x6ea - 0x6e];
    ColChild();
    ~ColChild();
};








class GRPort2 { public: char body[0x26]; GRPort2(); ~GRPort2(); };
class ListBase { public: char body[0x10]; ListBase(); ~ListBase(); };
class ListMember : public ListBase { public: ListMember(); };
class Trivial { public: char body[2]; Trivial(); ~Trivial() { } };
class SScrollBar : public DlgControl {
public:
    char pad_s[0x28 - 0x1a];
    SScrollBar();
    ~SScrollBar();
};
class PediaWin : public ColChild {
public:
    char pad_p[0x77c - 0x77a];
    SImageButton btn;
    char pad_q[0x7f0 - 0x77c - 0x6e];
    SScrollBar bar;
    GRPort2 port;
    ListMember list;
    PediaWin();
    ~PediaWin();
    virtual void v0();
};
extern "C" void list_delete_items(PediaWin *p);


PediaWin::~PediaWin()
{
    list_delete_items(this);
}
