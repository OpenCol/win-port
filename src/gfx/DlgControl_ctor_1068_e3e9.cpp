/* DlgControl::DlgControl() -- shipped at 1068:e3e9.
 *
 * Ported by tools/port.py from matched/gfx/DlgControl_ctor_1068_e3e9.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

#include "w16types.h"
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
DlgControl::DlgControl()
{
    id = 0;
    parent = 0;
    SetRect(&rect, 0, 0, 0, 0);
    handle = 0;
    next = 0;
    b16 = 0;
    b17 = 0;
    w18 = -1;
}
