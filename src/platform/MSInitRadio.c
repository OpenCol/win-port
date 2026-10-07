/* PORT: this file's own declarations of InitCtrlBlock, get_win_1070_95f0 removed: the header declares them from the definition (W1 step 3). */
/* MSInitRadio: candidate for 1070:3dd1 (1241 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1241, not the census 1230: the dead second epilogue and padding reach 1070:42aa.
 *
 * `build_radio_buttons` TAKING ONE STRING instead of an array: the captions are separated by
 * underscores, and `split_underscores` turns a copy of the string (up to 1024 bytes) into that
 * many NUL-terminated captions and returns how many there are. Everything else -- the grid, the three
 * bitmaps, the kind-3 blocks, the `!` and `^` prefixes, the `~` accelerator and the handle of 0x92-byte
 * records -- is the same, and its caller is `1068:df8f`.
 *
 * TWO DIFFERENCES BEYOND THE CAPTIONS:
 *  - the cell height divides by the count, except with more than one column and an ODD count, where it
 *    divides by count + 1;
 *  - the column wraps on the CONTROL'S TOP reaching the area's bottom, where the array version compares
 *    the cell's own y.
 *
 * AND THE `~` SQUEEZE USES ITS OWN INDEX HERE, where 1070:3976 reuses the loop counter and walks off the
 * record. The same source written twice, with the bug in only one of them.
 *
 * RENAMED 2026-09-27 from build_text_buttons_1070_3dd1: every control it builds is a radio item: CtrlBlock kind 3, radio_wndproc, the radio bitmaps 0x12d/0x12f/0x12e, the ^ prefix marking the current item through set_cur_item; its callers are radioset_init and radio_init. Push buttons are kind 6.
 *
 * RENAMED 2026-09-27 from build_radio_buttons_1070_3dd1: the Mac build's MSInitRadio(short*, Rect*, short, void*, short, char**), which the KB and radio_init already bound to this address: it sits immediately before MSDisposeRadio (1070:42aa, self-named by its diagnostic), its 20 argument bytes match, and the Mac char** is a Handle holding ONE caption string -- this copy's underscore-separated string, not the array copy's list. The file name was the only place still calling it build_*.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void *ctl_window_1070_96a8(void *ctl);
extern void set_cur_item_1070_9994(void *radios, int cur_item);

short MSInitRadio(int *out, RECT *area, int cols, void *owner,
                                       char *s, int visible)
{
    short h;
    MenuItem *items;
    CtrlBlock *blk;
    char *p;
    int mark;
    int count;
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
    char buf[0x400];
    register int i;

    bmp_grey = LoadBitmap(g_hinstance, (const char *)0x12dL);
    bmp_off = LoadBitmap(g_hinstance, (const char *)0x12fL);
    bmp_on = LoadBitmap(g_hinstance, (const char *)0x12eL);
    STRNCPY(buf, s, 0x400);
    count = split_underscores(buf);
    p = buf;
    h = MSAllocateHandle((unsigned)(count * 0x92));
    items = (MenuItem *)MSHLock(h);
    cw = rect_width_1070_962a(area) / cols;
    if (cols == 1 || count % 2 == 0)
        ch = rect_height_1070_9649(area) * cols / count;
    else
        ch = rect_height_1070_9649(area) * cols / (count + 1);
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
                mark = items[i].mark;
                while (STRLEN(items[i].text) > mark) {
                    items[i].text[mark] = items[i].text[mark + 1];
                    mark++;
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
        p += STRLEN(p) + 1;
        y += ch;
        if (ch / 2 - rowh / 2 + y >= area->bottom) {
            x += cw;
            y = area->top;
        }
    }
    *out = count;
    MSHUnlock(h);
    return h;
}
