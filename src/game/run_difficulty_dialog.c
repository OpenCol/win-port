/* run_difficulty_dialog -- shipped at 1030:0be3.
 *
 * Ported by tools/port.py from matched/game/run_difficulty_dialog.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern char far g_game_record;                       SEG20:0x05ca
 *
 * The file's own Screen dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1030_e5fe(void);
void MSDoTask_1030_e630(void);
void MSHideWindow_of_1030_e647(void *w);
void SetDifficulty_DrawIt(Screen *s);
void copy_all_to_screen_1030_e504(Screen *s);
void ctl_init_button_1030_e55b(void *c, void *w, col_i16 id, RECT *r,
                                   char *title);
void set_handler_slot_4_1030_e522(void *o, void ( *fn)());
void set_p_1c_1030_e5bf(void *c, void ( *fn)());
void win_center_1030_e5d9(void *w);
void win_show_focus_at_26_1030_e611(Screen *s);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void * overlay_window(void);
extern void build_window_with_port_1060_a16b(Screen *o, char *name, col_i16 style,
                                                 col_i16 x, col_i16 y, col_i16 w, col_i16 h,
                                                 void *pal, void *parent);
extern void load_picture_resource(void *p, col_i16 res, col_i16 w, col_i16 h, void *pal);
extern void draw_bevel(Screen *s, RECT *r, col_i16 depth, col_i16 c1, col_i16 c2);
extern void dispatch_click_1030_091b(void);
extern void dialog_click_15a(void);

/* same object: 4-byte `push cs; call ` (rule 47) */











col_i16 run_difficulty_dialog(Screen *s)
{
    RECT r;
    void *parent;

    (*((col_i16 *)((unsigned char *)(s) + 0xec))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0xee))) = 1;
    (*((col_i16 *)((unsigned char *)(s) + 0x15c))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x15a))) = 0;
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[0], 0xff, 0x11, 0x188, 0xe7);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[1], 0x1d1, 0x11, 0x25a, 0xe7);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[2], 0x2d, 0xf8, 0xb6, 0x1cf);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[3], 0xff, 0xf8, 0x188, 0x1cf);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[4], 0x1d1, 0xf8, 0x25a, 0x1cf);
    parent = overlay_window();
    if (parent != 0)
        build_window_with_port_1060_a16b(s, "", 0x801, 0, 0, 0x280, 0x1e0,
                                         &(*g_game_palette), parent);
    else
        build_window_with_port_1060_a16b(s, "", 0x801, 0, 0, 0x280, 0x1e0,
                                         &(*g_game_palette), &g_game_record);
    SetRect(&r, 0, 0, 0x280, 0x1e0);
    load_picture_resource(((char *)((unsigned char *)(s) + 0x134)), 0x44d, 0x28, 0xa8, &(*g_game_palette));
    SetDifficulty_DrawIt(s);
    set_handler_slot_4_1030_e522((char *)s + 0x33, dispatch_click_1030_091b);
    copy_all_to_screen_1030_e504(s);
    SetRect(&r, 0, 0, 0xa0, 0x78);
    OffsetRect(&r, 0x20, 0x32);
    draw_bevel(s, &r, -2, 0xff, 0);
    SetRect(&r, r.left + 2, r.bottom - 0x3e, r.right - 2, r.bottom - 0x20);
    ctl_init_button_1030_e55b(((char *)((unsigned char *)(s) + 0xf0)), (char *)s + 0x26, 0x64, &r, "~Ok");
    set_p_1c_1030_e5bf(((char *)((unsigned char *)(s) + 0xf0)), dialog_click_15a);
    OffsetRect(&r, 0, 0x1e);
    ctl_init_button_1030_e55b(((char *)((unsigned char *)(s) + 0x112)), (char *)s + 0x26, 0x65, &r, "~Cancel");
    set_p_1c_1030_e5bf(((char *)((unsigned char *)(s) + 0x112)), dialog_click_15a);
    win_center_1030_e5d9((char *)s + 0x26);
    MSDoAllTasks_1030_e5fe();
    win_show_focus_at_26_1030_e611(s);
    while ((*((col_i16 *)((unsigned char *)(s) + 0x15a))) == 0)
        MSDoTask_1030_e630();
    MSHideWindow_of_1030_e647((char *)s + 0x26);
    MSDoAllTasks_1030_e5fe();
    if ((*((col_i16 *)((unsigned char *)(s) + 0x15c))) != 0)
        (*((col_i16 *)((unsigned char *)(s) + 0xee))) = 0;
    return (*((col_i16 *)((unsigned char *)(s) + 0xee)));
}
