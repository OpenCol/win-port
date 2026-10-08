/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* PORT: this file's own declarations of ctl_window_1070_96a8 removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 38 of the imported file. */
/* PORT: this file's own declarations of get_win_1070_95f0, parse_hotkey_marker removed: the header declares them from the definition (W1 step 3). */
/* MSInitCheck: candidate for 1070:2f63 (393 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSInitCheck
 *
 * Creates a check-box control: the label is copied to a `new char[]` buffer, an
 * MSControlClass child window is made (style 0x40010000, plus WS_VISIBLE (0x10000000) for `visible`)
 * at the region's position and size under the owner chain, the hot-key marker parsed out of
 * the copy, the three check-box bitmaps (0x132, 0x131, 0x130) loaded, and the control block
 * (kind 2, the label at +0x14) filled; the control's window procedure is installed.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
/* PORT: this file's own Screen, DlgControl removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" CtrlBlock *InitCtrlBlock(unsigned hwnd, void *obj);

extern "C" unsigned MSInitCheck(RECT *r, void *ctl, const char *text, int visible)
{
    CtrlBlock *c;
    unsigned bmp_grey;
    unsigned bmp_on;
    unsigned bmp_off;
    void *ctl_copy = ctl;
    char *copy;
    unsigned long style;
    unsigned hwnd;

    copy = new char[STRLEN(text) + 1];
    STRCPY(copy, (char*)(text));
    style = 0x40010000L;
    if (visible)
        style |= WS_VISIBLE;
    hwnd = CreateWindow("MSControlClass", "", style, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
    parse_hotkey_marker((DlgControl *)ctl_copy, copy);
    bmp_grey = LoadBitmap(g_hinstance, (const char *)0x132L);
    bmp_off = LoadBitmap(g_hinstance, (const char *)0x131L);
    bmp_on = LoadBitmap(g_hinstance, (const char *)0x130L);
    c = InitCtrlBlock(hwnd, ctl);
    c->bmp0 = bmp_grey;
    c->bmp1 = bmp_off;
    c->bmp2 = bmp_on;
    c->kind = 2;
    c->data = copy;
    SetWindowLong(hwnd, -4, (intptr_t)(void *)checkbox_wndproc);
    return hwnd;
}
