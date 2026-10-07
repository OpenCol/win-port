/* PORT: this file's own declarations of ctl_window_1070_96a8, set_cur_item_1070_9994 removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of InitCtrlBlock, get_win_1070_95f0 removed: the header declares them from the definition (W1 step 3). */
/* build_radio_buttons: candidate for 1070:3976 (1115 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 1115, not the census 1104: the dead second epilogue and two bytes of padding reach the
 * next function at 1070:3dd1.
 *
 * BUILDS A COLUMN-MAJOR GRID OF TEXT CONTROLS for `1050:881c`: one 0x92-byte record and one
 * "MSControlClass" child window per caption, in a handle the function returns. The captions come from an
 * array of pointers, and the count is written back through `out`.
 *
 * THE LAYOUT divides the area by `cols` across and by the count down, and each control is one text row
 * high, centred in its cell. A column is filled top to bottom and the next starts when the area's bottom
 * is reached. Three bitmaps (0x12d, 0x12f, 0x12e) go into every control's block, whose kind is 3 and
 * whose window procedure is `radio_wndproc`. Only the FIRST control gets a clear +6 flag.
 *
 * THE CAPTION'S PREFIXES: `!` gives style 0 and `^` style 1 and also makes the control the owner's
 * current one (`set_cur_item_1070_9994`); anything else is style 1. A `~` marks the accelerator: the
 * character after it is remembered in lower case at +0x8e, its position at +0x90, and the `~` itself is
 * squeezed out of the string. A caption with no `~` gets -1 at +0x90.
 *
 * A SHIPPED BUG: the `~` squeeze reuses the LOOP INDEX as the character index (they share DI, and the
 * base of every record access is computed from the same variable). From the moment a caption contains a
 * `~`, the rest of the iteration reads and writes the record at the character position instead of the
 * control's, and the loop then continues from there. The reconstruction keeps the single variable
 * because that is what the bytes say.
 *
 * Spellings: `i == 0 ? 0 : 1` for the first-control flag (arms in that order), the scan's `else` arm, and
 * the accelerator test written as `<=` with the -1 arm first.
 *
 * RENAMED 2026-09-27 from build_text_buttons_1070_3976: every control it builds is a radio item: CtrlBlock kind 3, radio_wndproc, the radio bitmaps 0x12d/0x12f/0x12e, the ^ prefix marking the current item through set_cur_item; its callers are radioset_init and radio_init. Push buttons are kind 6.
 *
 * RENAMED 2026-09-27 from build_radio_buttons_1070_3976: with 1070:3dd1 bound to MSInitRadio this is the family's only copy, so the suffix goes (check_naming: a singleton drops its suffix). It is the array-of-captions twin that radioset_init calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */


short build_radio_buttons(int *out, RECT *area, int cols, void *owner,
                                     int count, char * *names, int visible)
{
    short h;
    MenuItem *items;
    CtrlBlock *blk;
    char *p;
    int cw;
    int ch;
    int x;
    int y;
    int rowh;
    unsigned bmp_grey;
    unsigned bmp_off;
    unsigned bmp_on;
    char *q;
    char accel[4];
    long style;
    register int i;

    bmp_grey = LoadBitmap(g_hinstance, (const char *)0x12dL);
    bmp_off = LoadBitmap(g_hinstance, (const char *)0x12fL);
    bmp_on = LoadBitmap(g_hinstance, (const char *)0x12eL);
    h = MSAllocateHandle((unsigned)(count * 0x92));
    items = (MenuItem *)MSHLock(h);
    cw = rect_width_1070_962a(area) / cols;
    ch = rect_height_1070_9649(area) * cols / count;
    rowh = font_row_height();
    x = area->left;
    y = area->top;
    style = 0x40010000L;
    if (visible != 0)
        style |= 0x10000000L;
    for (i = 0; i < count; i++) {
        items[i].hwnd = CreateWindow("MSControlClass", "", style, x,
                                     ch / 2 - rowh / 2 + y, cw, rowh,
                                     ((MSWin *)get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)owner))))->hwnd,
                                     0, g_hinstance, 0L);
        blk = (CtrlBlock *)InitCtrlBlock(items[i].hwnd, owner);
        blk->shared_bitmaps = i == 0 ? 0 : 1;
        blk->bmp0 = bmp_grey;
        blk->bmp1 = bmp_off;
        blk->bmp2 = bmp_on;
        blk->data = &items[i];
        blk->kind = 3;
        SetWindowLong(items[i].hwnd, -4, (long)(void *)radio_wndproc);
        SetRect(&items[i].rect, x, ch / 2 - rowh / 2 + y, x + cw,
                ch / 2 - rowh / 2 + y + rowh);
        items[i].index = i;
        p = names[i];
        if ((int)*p == '!') {
            items[i].style = 0;
            p++;
        } else if ((int)*p == '^') {
            items[i].style = 1;
            set_cur_item_1070_9994((SRadioSet *)owner, i);
            p++;
        } else {
            items[i].style = 1;
        }
        STRCPY(items[i].text, p);
        q = items[i].text;
        items[i].mark = 0;
        while (*q != 0) {
            if ((int)*q == '~') {
                i = items[i].mark;
                while (STRLEN(items[i].text) > i) {
                    items[i].text[i] = items[i].text[i + 1];
                    i++;
                }
                break;
            } else {
                items[i].mark++;
                q++;
            }
        }
        if (STRLEN(items[i].text) <= items[i].mark) {
            items[i].mark = -1;
        } else {
            accel[0] = items[i].text[items[i].mark];
            accel[1] = 0;
            STRLWR(accel);
            items[i].accel = accel[0];
        }
        y += ch;
        if (area->bottom <= y) {
            x += cw;
            y = area->top;
        }
    }
    *out = count;
    MSHUnlock(h);
    return h;
}
