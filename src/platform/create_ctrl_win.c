/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* create_ctrl_win: candidate for 1070:2688 (236 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSInitRegion`'s sibling and the same six steps: create an `MSControlClass` child window,
 * attach a control block, stamp a kind into +0x1c and subclass it. What differs is that the
 * geometry arrives as four loose words rather than out of an object, the style is built
 * rather than constant, and the kind is **5**.
 *
 * The style is `0x40810000` -- WS_CHILD | WS_BORDER | WS_TABSTOP -- plus WS_VISIBLE
 * (`0x10000000`) when the last argument is non-zero. So the caller's flag is the visibility,
 * and it is ORed in rather than selected between two constants.
 *
 * The width and height are NOT the two words the caller passes. Those are added to x and y
 * to make a rectangle, and the rectangle is then measured by `rect_width_1070_962a` and
 * `rect_height_1070_9649` -- the same pair `MSInitRegion` measures its object with. So the caller
 * gives a corner and an extent, and the widget layer's own measure functions turn that into
 * the CreateWindow width and height.
 *
 * The local pointer at BP-8 is a DEAD STORE: the parameter is copied into it and every
 * use reads the parameter instead. The store is in the shipped code and a reconstruction
 * that drops it is four bytes short.
 *
 * 236 bytes and not the census 227: `return hwnd;` costs a second, unreachable epilogue, and
 * this frame's epilogue is nine bytes (rule 44).
 *
 * NOT `MSInitImageButton`, whose eight arguments come to eighteen bytes where both call sites
 * clean fourteen.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */

extern void *ctl_window_1070_96a8(void *ctl);
extern CtrlBlock *InitCtrlBlock(unsigned hwnd, void *obj);

unsigned create_ctrl_win(int x, int y, int w, int h, void *obj, int visible)
{
    unsigned hwnd;
    void *o;
    CtrlBlock *c;
    RECT r;
    unsigned long style;

    style = 0x40810000L;
    if (visible != 0)
        style |= 0x10000000L;
    o = obj;
    SetRect(&r, x, y, x + w, y + h);
    hwnd = CreateWindow("MSControlClass", "", style, x, y,
                        rect_width_1070_962a(&r), rect_height_1070_9649(&r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)obj)))->hwnd,
                        0, g_hinstance, 0);
    c = InitCtrlBlock(hwnd, obj);
    c->kind = 5;
    SetWindowLong(hwnd, -4, (long)(void *)image_button_proc);
    return hwnd;
}
