/* pedia_redraw -- shipped at 1060:2116.
 *
 * Ported by tools/port.py from matched/game/pedia_redraw.c.
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1060_65ca(void *port);
col_i16 get_w_4_1060_65e3(void *port);
col_i16 rect_height_1060_63d4(RECT *r);
col_i16 rect_width_1060_63b5(RECT *r);
void control_hide_1060_669f(void *c);
void control_show_1060_68a5(void *c);
void pedia_prompt(PediaWin *o);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern void draw_bevel(void *p, RECT *r, col_i16 depth, col_i16 c1, col_i16 c2);


extern void PediaWin_DrawArticle(PediaWin *w);
extern void pedia_draw_list_page(PediaWin *w);

/* same object: 4-byte `push cs; call ` */








void pedia_redraw(PediaWin *w)
{
    RECT r;
    register col_i16 i;

    if (w->sel != -1) {
        PediaWin_DrawArticle(w);
        return;
    }
    SetRect(&r, 0, 0, get_w_2_1060_65ca(((char *)((unsigned char *)(w) + 0x818))), get_w_4_1060_65e3(((char *)((unsigned char *)(w) + 0x818))));
    CopyToPort(((char *)((unsigned char *)(w) + 0x818)), w, &r, &(*((RECT *)((unsigned char *)(w) + 0x76a))));
    pedia_prompt(w);
    SetRect(&r, 0x1b, 0x25, rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) + ((*((col_i16 *)((unsigned char *)(w) + 0x850))) == 1 ? 0 : 0x19) - 0x34,
            rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x24);
    OffsetRect(&r, (*((RECT *)((unsigned char *)(w) + 0x76a))).left, (*((RECT *)((unsigned char *)(w) + 0x76a))).top);
    draw_bevel(w, &r, -3, 0xff, 0xf8);
    for (i = 0; i < 8; i++) {
        SetRect(&r, (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x20,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x2a,
                (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x1c,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * (i + 1) + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x27);
        draw_bevel(w, &r, 2, 0xff, 0xf8);
        SetRect(&r, (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x20,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x2a,
                rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) + (*((RECT *)((unsigned char *)(w) + 0x76a))).left - 0x39,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * (i + 1) + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x27);
        draw_bevel(w, &r, 2, 0xff, 0xf8);
    }
    if ((*((col_i16 *)((unsigned char *)(w) + 0x850))) != 1)
        control_show_1060_68a5(((char *)((unsigned char *)(w) + 0x7f0)));
    else
        control_hide_1060_669f(((char *)((unsigned char *)(w) + 0x7f0)));
    pedia_draw_list_page(w);
}
