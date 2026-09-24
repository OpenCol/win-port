/* super_win_init -- shipped at 1050:6a16.
 *
 * Ported by tools/port.py from matched/game/super_win_init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_modal_done;                             DGROUP:0x2520
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *
 * The file's own EventWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1050_832b(RECT *r);
col_i16 rect_width_1050_830c(RECT *r);
void * set_handler_slot_12_1050_8f80(void *h, void ( *fn)());
void * set_handler_slot_24_1050_8fb9(void *h, void ( *fn)());
void * set_handler_slot_40_1050_8544(void *h, void ( *fn)());
void MSDoAllTasks_1050_8be7(void);
void MSShowWindow_of_1050_8ff2(EventWin *w);
void win_set_back_color_1050_8f5c(EventWin *w, col_i16 c);

typedef struct {
    char base[0xc4];
    EventWin win; /* +0xc4 */
    char palette[0x2c4]; /* +0x150 */
} SuperWin;


extern void empty_1070_daca(void);
extern void pack_palette_rgb_fwd(void *pal, void *buf, col_i16 first, col_i16 count);
extern void MSGetDesktopSize(RECT *r);
extern void build_window_1060_9de8(EventWin *w, char *name, col_i16 style,
                                       col_i16 x, col_i16 y, col_i16 cx, col_i16 cy, void *pal);
extern void build_window_with_port_1060_a16b(SuperWin *o, char *name, col_i16 style,
                                                 col_i16 x, col_i16 y, col_i16 cx, col_i16 cy,
                                                 void *pal, EventWin *parent);
extern void mark_2520_no_super_1050_6bfb(void);
extern void mark_2520_no_super_1050_6c16(void);
extern void mark_2520_no_super_1050_6c31(void);

/* same object: 4-byte `push cs; call ` */









void super_win_init(SuperWin *o)
{
    RECT r;

    empty_1070_daca();
    g_modal_done = 0;
    pack_palette_rgb_fwd(&(*g_game_palette), o->palette, 10, 0xec);
    MSGetDesktopSize(&r);
    build_window_1060_9de8(&o->win, "", 0x801, 0, 0, rect_width_1050_830c(&r),
                           rect_height_1050_832b(&r), &(*g_game_palette));
    win_set_back_color_1050_8f5c(&o->win, 0);
    build_window_with_port_1060_a16b(o, "", 0x801, 0, 0, 0x280, 0x1e0,
                                     &(*g_game_palette), &o->win);
    set_handler_slot_12_1050_8f80((char *)o + 0x33, mark_2520_no_super_1050_6bfb);
    set_handler_slot_24_1050_8fb9((char *)o + 0x33, mark_2520_no_super_1050_6c16);
    set_handler_slot_40_1050_8544((char *)o + 0x33, mark_2520_no_super_1050_6c31);
    set_handler_slot_12_1050_8f80((char *)o + 0xd1, mark_2520_no_super_1050_6bfb);
    set_handler_slot_24_1050_8fb9((char *)o + 0xd1, mark_2520_no_super_1050_6c16);
    set_handler_slot_40_1050_8544((char *)o + 0xd1, mark_2520_no_super_1050_6c31);
    MSShowWindow_of_1050_8ff2(&o->win);
    MSDoAllTasks_1050_8be7();
}
