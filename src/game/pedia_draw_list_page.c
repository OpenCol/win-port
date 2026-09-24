/* pedia_draw_list_page -- shipped at 1060:282e.
 *
 * Ported by tools/port.py from matched/game/pedia_draw_list_page.c.
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
char * entry_display_name(void *w, void *e);
col_i16 rect_height_1060_63d4(RECT *r);
col_i16 rect_width_1060_63b5(RECT *r);
void * list_first_1060_68db(void *list);
void * list_next_1060_68fb(void *list);

extern unsigned char TextColor(unsigned char v);
extern void draw_string_shadowed(void *port, void *font, const char *s,
                                     col_i16 x, col_i16 y, col_i16 align);

/* same object: 4-byte `push cs; call ` */






void pedia_draw_list_page(PediaWin *w)
{
    void *e;
    register col_i16 i;

    e = list_first_1060_68db(((char *)((unsigned char *)(w) + 0x83e)));
    if (w->page != 1)
        for (i = 0; w->page * 16 - 16 > i; i++)
            e = list_next_1060_68fb(((char *)((unsigned char *)(w) + 0x83e)));
    for (i = 0; i < 8 && e != 0; i++) {
        TextColor(0xff);
        draw_string_shadowed(w, &g_heading_font, entry_display_name(w, e),
            (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left + 0x1e,
            (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i + (*((RECT *)((unsigned char *)(w) + 0x76a))).top
                + (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 / 2 + 0x28,
            0);
        e = list_next_1060_68fb(((char *)((unsigned char *)(w) + 0x83e)));
        if (e != 0) {
            draw_string_shadowed(w, &g_heading_font, entry_display_name(w, e),
                (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 + (*((RECT *)((unsigned char *)(w) + 0x76a))).left
                    + (rect_width_1060_63b5(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x55) / 2 / 2 + 0x1e,
                (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 * i + (*((RECT *)((unsigned char *)(w) + 0x76a))).top
                    + (rect_height_1060_63d4(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - 0x50) / 8 / 2 + 0x28,
                0);
            e = list_next_1060_68fb(((char *)((unsigned char *)(w) + 0x83e)));
        }
    }
}
