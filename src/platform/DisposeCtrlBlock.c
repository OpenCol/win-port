/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* DisposeCtrlBlock: candidate for 1070:09dd (264 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DisposeCtrlBlock
 *
 * Tears down a window's control block (window long 0): with the flag at +6 clear the three
 * GDI objects at +8/+0xa/+0xc -- `bmp0`, `bmp1`, `bmp2` -- are deleted; a kind-2 block (+0x1c) deletes the object at
 * +0x14 with `delete`; the block's handle is unlocked and disposed and the default control
 * procedure restored. A null window is reported ("Error: Tried to dispose NULL control in DisposeCtrlBlock").
 *
 * The three bitmap slots are numbered, not named, because what each holds depends on the kind.
 * `bitmap_button_proc` paints +8 up, +0xa down and +0xc when disabled, which is what `MSInitButton`
 * stores. `checkbox_wndproc` and `radio_wndproc` paint +8 when disabled, +0xa unchecked and +0xc
 * checked, which is what `MSInitCheck` (0x132, 0x131, 0x130) and `MSInitRadio`/`build_radio_buttons` (0x12d,
 * 0x12f, 0x12e) store. +0x14 is per-kind too: the check box's label copy, which is why only kind 2
 * deletes it, and a radio control's item record.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1070_969a(void *ctx, char *msg);

extern "C" void DisposeCtrlBlock(unsigned hwnd)
{
    CtrlBlock *c;
    unsigned h;

    if (hwnd != 0) {
        c = (CtrlBlock *)GetWindowLong(hwnd, 0);
        if (c == 0)
            return;
        if (c->shared_bitmaps == 0) {
            if (c->bmp0 != 0) {
                DeleteObject(c->bmp0);
                c->bmp0 = 0;
            }
            if (c->bmp1 != 0) {
                DeleteObject(c->bmp1);
                c->bmp1 = 0;
            }
            if (c->bmp2 != 0) {
                DeleteObject(c->bmp2);
                c->bmp2 = 0;
            }
        }
        if (c->kind == 2) {
            delete c->data;
            c->data = 0;
        }
        h = c->handle;
        MSHUnlock(h);
        MSDisposeHandle(h);
        SetWindowLong(hwnd, -4, (intptr_t)(void *)control_message_handler);
    } else {
        report_error_1070_969a(&g_error_log, "Error: Tried to dispose NULL control in DisposeCtrlBlock");
    }
}
