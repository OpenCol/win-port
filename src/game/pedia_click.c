/* pedia_click -- shipped at 1060:2c0a.
 *
 * Ported by tools/port.py from matched/game/pedia_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_heading_font;                      DGROUP:0x4b22
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * entry_display_name(PediaWin *w, void *e);
col_i16 rect_height_1060_63d4(RECT *r);
col_i16 rect_width_1060_63b5(RECT *r);
void * list_item_at_count(PediaWin *w);
void PediaWin_DrawArticle(PediaWin *w);
void copy_all_to_screen_1060_610c(PediaWin *w);
void pedia_redraw(PediaWin *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern void draw_bevel(void *p, RECT *r, col_i16 depth, col_i16 c1, col_i16 c2);
extern void draw_string_shadowed(void *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);



/* same object: 4-byte `push cs; call ` */








void pedia_click(PediaWin *w, col_i16 x, col_i16 y)
{
    RECT r;
    RECT save;
    register col_i16 i;
    register col_i16 px;

    px = x;
    if (w->sel != -1) {
        w->sel = -1;
        pedia_redraw(w);
        copy_all_to_screen_1060_610c(w);
        return;
    }
    w->sel = -1;
    for (i = 0; i < 8; i++) {
        SetRect(&r, (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x20,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x2a,
                (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x1c,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * (i + 1) + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x27);
        if (px > r.left && px < r.right && y > r.top && y < r.bottom
            && w->page * 16 + i * 2 - 16 <= w->count - 1) {
            w->sel = w->page * 16 + i * 2 - 16;
            save = r;
            CopyToPort(((char *)((unsigned char *)(w) + 0x818)), w, &save, &r);
            OffsetRect(&save, -(*((RECT *)((unsigned char *)(w) + 0x76a))).left, -(*((RECT *)((unsigned char *)(w) + 0x76a))).top);
            OffsetRect(&r, 2, 2);
            CopyToPort(((char *)((unsigned char *)(w) + 0x818)), w, &save, &r);
            draw_bevel(w, &r, -2, 0xff, 0xf8);
            draw_string_shadowed(w, &g_heading_font,
                                 entry_display_name(w, list_item_at_count(w)),
                                 (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 / 2
                                   + (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x1f,
                                 (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 / 2
                                   + ((rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i
                                      + (*((RECT *)((unsigned char *)(w) + 0x76a))).top) + 0x29,
                                 0);
            copy_all_to_screen_1060_610c(w);
            goto done;
        }
        SetRect(&r, (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x20,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x2a,
                rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) + (*((RECT *)((unsigned char *)(w) + 0x76a))).left - 0x39,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * (i + 1) + (*((RECT *)((unsigned char *)(w) + 0x76a))).top + 0x27);
        if (px > r.left && px < r.right && y > r.top && y < r.bottom
            && w->page * 16 + i * 2 - 15 <= w->count - 1) {
            w->sel = w->page * 16 + i * 2 - 15;
            save = r;
            CopyToPort(((char *)((unsigned char *)(w) + 0x818)), w, &save, &r);
            OffsetRect(&save, -(*((RECT *)((unsigned char *)(w) + 0x76a))).left, -(*((RECT *)((unsigned char *)(w) + 0x76a))).top);
            OffsetRect(&r, 2, 2);
            CopyToPort(((char *)((unsigned char *)(w) + 0x818)), w, &save, &r);
            draw_bevel(w, &r, -2, 0xff, 0xf8);
            draw_string_shadowed(w, &g_heading_font,
                                 entry_display_name(w, list_item_at_count(w)),
                                 (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left
                                   + (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 / 2 + 0x1f,
                                 (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 / 2
                                   + ((rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i
                                      + (*((RECT *)((unsigned char *)(w) + 0x76a))).top) + 0x29,
                                 0);
            copy_all_to_screen_1060_610c(w);
            goto done;
        }
    }
done:
    if (w->sel != -1)
        PediaWin_DrawArticle(w);
}
