/* SImageButton::~SImageButton() -- shipped at 1008:e8ee.
 *
 * Ported by tools/port.py from matched/game/SImageButton_dtor_1008_e8ee.c.
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
    col_i16 f0;
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
class GRPort {
public:
    char body[0x24];
    GRPort();
    ~GRPort();
    virtual void v0();
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


SImageButton::~SImageButton()
{
    if (handle != 0)
        image_button_release(handle);
}
