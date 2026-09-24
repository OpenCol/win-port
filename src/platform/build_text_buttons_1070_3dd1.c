/* build_text_buttons_1070_3dd1 -- shipped at 1070:3dd1.
 *
 * Ported by tools/port.py from matched/platform/build_text_buttons_1070_3dd1.c.
 *
 * STRCPY -> strcpy, STRLEN -> strlen, STRNCPY -> strncpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *
 * The file's own MSWin, CtrlBlock dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_row_height(void);
col_i16 rect_height_1070_9649(RECT *r);
col_i16 rect_width_1070_962a(RECT *r);
col_i16 split_underscores(char *s);
void * InitCtrlBlock(col_u16 hwnd, void *obj);
void * ctl_window_1070_96a8(void *c);
void * get_p_6_1070_95f0(void *c);
void set_w_2a_1070_9994(void *c, col_i16 v);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 idx, col_i32 v);
extern col_u16 pascal CreateWindow(const char *cls, const char *title, col_u32 style,
                                        col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_u16 parent, col_u16 menu,
                                        col_u16 hinst, void *param);
extern col_u16 pascal LoadBitmap(col_u16 hinst, const char *name);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

typedef struct {
    col_u16 hwnd; /* +0 */
    RECT rect; /* +2 */
    col_i16 index; /* +0xa */
    col_i16 style; /* +0xc */
    char text[0x8e - 0xe]; /* +0xe */
    char accel; /* +0x8e */
    char pad8f;
    col_i16 mark; /* +0x90 */
} MenuItem;

extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);



extern char * STRLWR(char *s);




extern col_i32 pascal radio_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);

/* same object: 4-byte `push cs; call ` (rule 47) */









col_i16 build_text_buttons_1070_3dd1(col_i16 *out, RECT *area, col_i16 cols, void *owner,
                                       char *s, col_i16 visible)
{
    col_i16 h;
    MenuItem *items;
    CtrlBlock *blk;
    char *p;
    col_i16 k;
    col_i16 count;
    col_i16 cw;
    col_i16 ch;
    col_i16 x;
    col_i16 y;
    col_i16 rowh;
    col_u16 bmp_up;
    col_u16 bmp_down;
    col_u16 bmp_over;
    char *q;
    char accel[4];
    col_i32 style;
    char buf[0x400];
    register col_i16 i;

    bmp_up = LoadBitmap(g_hinstance, (const char *)0x12dL);
    bmp_down = LoadBitmap(g_hinstance, (const char *)0x12fL);
    bmp_over = LoadBitmap(g_hinstance, (const char *)0x12eL);
    strncpy(buf, s, 0x400);
    count = split_underscores(buf);
    p = buf;
    h = MSAllocateHandle((col_u16)(count * 0x92));
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
                                     ((MSWin *)get_p_6_1070_95f0(ctl_window_1070_96a8(owner)))->hwnd,
                                     0, g_hinstance, 0L);
        blk = (CtrlBlock *)InitCtrlBlock(items[i].hwnd, owner);
        (*((col_i16 *)((unsigned char *)(blk) + 0x6))) = i == 0 ? 0 : 1;
        (*((col_u16 *)((unsigned char *)(blk) + 0x8))) = bmp_up;
        (*((col_u16 *)((unsigned char *)(blk) + 0xa))) = bmp_down;
        (*((col_u16 *)((unsigned char *)(blk) + 0xc))) = bmp_over;
        (*((void * *)((unsigned char *)(blk) + 0x14))) = &items[i];
        blk->kind = 3;
        SetWindowLong(items[i].hwnd, -4, (col_i32)(void *)radio_wndproc);
        SetRect(&items[i].rect, x, ch / 2 - rowh / 2 + y, x + cw,
                ch / 2 - rowh / 2 + y + rowh);
        items[i].index = i;
        if ((col_i16)*p == '!') {
            items[i].style = 0;
            p++;
        } else if ((col_i16)*p == '^') {
            items[i].style = 1;
            set_w_2a_1070_9994(owner, i);
            p++;
        } else {
            items[i].style = 1;
        }
        strcpy(items[i].text, p);
        q = items[i].text;
        items[i].mark = 0;
        while (*q != 0) {
            if ((col_i16)*q == '~') {
                k = items[i].mark;
                while (strlen(items[i].text) > k) {
                    items[i].text[k] = items[i].text[k + 1];
                    k++;
                }
                break;
            } else {
                items[i].mark++;
                q++;
            }
        }
        if (strlen(items[i].text) <= items[i].mark) {
            items[i].mark = -1;
        } else {
            accel[0] = items[i].text[items[i].mark];
            accel[1] = 0;
            STRLWR(accel);
            items[i].accel = accel[0];
        }
        p += strlen(p) + 1;
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
