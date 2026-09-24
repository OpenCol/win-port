/* colony_draw_warehouse -- shipped at 1010:8ef4.
 *
 * Ported by tools/port.py from matched/game/colony_draw_warehouse.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *   extern char far g_font_4b0e;                         DGROUP:0x4b0e
 *
 * The file's own Colony, ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1010_e2d2(void *f, const char *s);
col_i16 get_w_2_1010_e73d(void *r);
void copy_rect_to_screen_1010_e6a4(void *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "building.h"
#include "goods.h"
extern void set_current_colony(col_i16 c);
extern void colony_blit_cell_1010_bd3f(void *o, col_i16 x, col_i16 y, col_i16 w, col_i16 h);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(RECT *out, void *rec, void *dst, col_i16 x, col_i16 y);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);


extern col_i16 current_colony_flag_get(col_i16 bit);
extern col_i16 bitmap_test(col_i16 n);
extern col_i16 colony_warehouse_capacity(void);
extern char * append_number_padded(char *s, col_i16 value, col_i16 digits);
extern void TextColor(col_i16 c);
extern void draw_string_shadowed(void *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);

/* same object: 4-byte `push cs; call ` (rule 47) */




void colony_draw_warehouse(ColChild *w, col_i16 redraw)
{
    col_i16 colx;
    col_i16 iy;
    col_i16 ty;
    col_i16 x;
    col_i16 ty2;
    col_i16 hundreds;
    col_i16 rem;
    col_i16 c1;
    col_i16 c2;
    RECT r;
    RECT s1;
    RECT s2;
    char num[20];
    char buf[0x50];
    register col_i16 i;
    register col_i16 tx;

    set_current_colony(w->colony);
    colony_blit_cell_1010_bd3f(w, 0, 0x186, 0x262, 0x32);
    colx = w->ox + 0x14;
    iy = w->oy + 0x18b;
    scale_map_select(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        x = colx;
        if (i == GOODS_CIGARS || i == GOODS_MUSKETS) {
            scale_map_select(1, 1);
            sprite_draw(&s1, (&g_goods_sprites[i]), w, x, iy - 2);
            scale_map_select(7, 8);
        } else
            sprite_draw(&s2, (&g_goods_sprites[i]), w, x, iy);
        scale_map_select(7, 8);
        buf[0] = 0;
        LTOA1(((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[i], num, 10);
        strcat(buf, num);
        ty = w->oy + 0x1ad;
        tx = colx;
        hundreds = ((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[i] / 100;
        rem = ((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[i] % 100;
        c1 = 0x67;
        c2 = 0x84;
        if (current_colony_flag_get(BUILDING_CUSTOM_HOUSE) != 0 && bitmap_test(i) != 0)
            c2 = 0xfa;
        if (i != GOODS_FOOD && colony_warehouse_capacity() < ((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[i]) {
            c1 = 0x3d;
            c2 = 0x3c;
        }
        if (hundreds > 0) {
            LTOA1(hundreds, buf, 10);
            strcpy(num, buf);
            append_number_padded(num, rem, 2);
            tx -= font_text_extent_1010_e2d2(&g_font_4b0e, num) >> 1;
            TextColor(c1);
            buf[0] = 0;
            LTOA1(hundreds, buf, 10);
            ty2 = ty - (get_w_2_1010_e73d(&g_font_4b0e) >> 1);
            draw_string_shadowed(w, &g_font_4b0e, buf, tx, ty2, 5);
            tx += font_text_extent_1010_e2d2(&g_font_4b0e, buf);
            buf[0] = 0;
            append_number_padded(buf, rem, 2);
            TextColor(c2);
            draw_string_shadowed(w, &g_font_4b0e, buf, tx, ty2, 5);
            tx += font_text_extent_1010_e2d2(&g_font_4b0e, buf) + 2;
        } else {
            LTOA1(rem, buf, 10);
            TextColor(c2);
            draw_string_shadowed(w, &g_font_4b0e, buf, tx + 1, ty, 0);
            tx += font_text_extent_1010_e2d2(&g_font_4b0e, buf) + 2;
        }
        colx += 0x26;
    }
    if (redraw != 0) {
        SetRect(&r, w->ox, w->oy + 0x186, w->ox + 0x262, w->oy + 0x1b8);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
    scale_map_select(1, 1);
}
