/* build_bordered_window -- shipped at 1020:669e.
 *
 * Ported by tools/port.py from matched/game/build_bordered_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern char far g_game_record;                       SEG20:0x05ca
 *   extern char far g_art_base[][0x20];                  SEG20:0x41a6
 *   extern int g_4bec;                                   DGROUP:0x4bec
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_handler_slot_16_1020_ab59(void *p, void ( *fn)());
void set_handler_slot_32_1020_ab92(void *p, void ( *fn)());
void set_handler_slot_4_1020_ab20(void *p, void ( *fn)());
void win_center_1020_abcb(void *p);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void build_window_with_port_1060_a16b(void *o, char *name, col_i16 style,
                                                 col_i16 x, col_i16 y, col_i16 w, col_i16 h,
                                                 void *pal, void *parent);

extern void FillColor(void *w, RECT *r, col_i16 color);
extern void port_set_clip(void *w, RECT *r);
extern void sprite_draw(RECT *out, void *rec, void *dst, col_i16 x, col_i16 y);
extern void port_apply_rect(void *w);
extern void TextColor(col_i16 c);
extern void GRPort_DrawRect(void *w, RECT *r);
extern void refresh_port_30_1020_667c(void);
extern void refresh_port_30_1020_665a(void);

/* same object: 4-byte `push cs; call ` */





void build_bordered_window(char *win, col_i16 x, col_i16 y, col_i16 w, col_i16 h)
{
    RECT r;
    RECT tmp;
    register col_i16 px;
    register col_i16 py;

    g_4bec = 0;
    build_window_with_port_1060_a16b(win, "", 0x801, x, y, w, h,
                                     &(*g_game_palette), &g_game_record);
    SetRect(&r, 0, 0, w, w);
    FillColor(win, &r, 0x24);
    SetRect(&r, 3, 3, w - 3, h - 3);
    port_set_clip(win, &r);
    for (py = 3; h - 3 > py; py += 0x2f)
        for (px = 3; w - 3 > px; px += 0x3e)
            sprite_draw(&tmp, (&g_art_base[411]), win, px, py);
    port_apply_rect(win);
    SetRect(&r, 0, 0, w, h);
    TextColor(0x21);
    GRPort_DrawRect(win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect(win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect(win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect(win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    TextColor(0x19);
    GRPort_DrawRect(win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect(win, &r);
    set_handler_slot_4_1020_ab20(win + 0x33, refresh_port_30_1020_667c);
    set_handler_slot_16_1020_ab59(win + 0x33, refresh_port_30_1020_667c);
    set_handler_slot_32_1020_ab92(win + 0x33, refresh_port_30_1020_665a);
    win_center_1020_abcb(win + 0x26);
}
