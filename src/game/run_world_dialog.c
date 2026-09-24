/* run_world_dialog -- shipped at 1030:1397.
 *
 * Ported by tools/port.py from matched/game/run_world_dialog.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_world_opt[4];                       SEG22:0x0000
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
col_i16 get_w_2_1030_e93a(Screen *s);
col_i16 get_w_4_1030_e959(Screen *s);
void MSDoAllTasks_1030_e5fe(void);
void MSDoTask_1030_e630(void);
void MSHideWindow_of_1030_e647(void *w);
void ctl_init_button_1030_e55b(void *c, void *w, col_i16 id, RECT *r, char *title);
void draw_selected_panels(Screen *s);
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
extern void dispatch_click_1030_1241(void);
extern void dialog_click_19e(void);
/* same object: 4-byte `push cs; call ` (rule 47) */












col_i16 run_world_dialog(Screen *s)
{
    RECT r;
    void *parent;
    register col_i16 i;

    (*((col_i16 *)((unsigned char *)(s) + 0x124))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x126))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x128))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x12a))) = 0;
    ((col_i16 *)((unsigned char *)(s) + 0x12c))[0] = g_world_opt[0];
    ((col_i16 *)((unsigned char *)(s) + 0x12c))[1] = g_world_opt[1] + 3;
    ((col_i16 *)((unsigned char *)(s) + 0x12c))[2] = g_world_opt[2] + 6;
    ((col_i16 *)((unsigned char *)(s) + 0x12c))[3] = g_world_opt[3] + 9;
    (*((col_i16 *)((unsigned char *)(s) + 0x1a2))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x19e))) = 0;
    (*((col_i16 *)((unsigned char *)(s) + 0x1a0))) = 0;
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[0], 0x14, 0x27, 0xa3, 0x9c);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[1], 0x14, 0xb6, 0xa3, 0x12b);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[2], 0x14, 0x144, 0xa3, 0x1b8);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[3], 0xac, 0x27, 0x13b, 0x9c);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[4], 0xac, 0xb6, 0x13b, 0x12b);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[5], 0xac, 0x144, 0x13b, 0x1b8);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[6], 0x144, 0x27, 0x1d3, 0x9c);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[7], 0x144, 0xb6, 0x1d3, 0x12b);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[8], 0x144, 0x144, 0x1d3, 0x1b8);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[9], 0x1db, 0x27, 0x26a, 0x9c);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[10], 0x1db, 0xb6, 0x26a, 0x12b);
    SetRect(&((RECT *)((unsigned char *)(s) + 0xc4))[11], 0x1db, 0x144, 0x26a, 0x1b8);
    parent = overlay_window();
    if (parent != 0)
        build_window_with_port_1060_a16b(s, "", 0x801, 0, 0, 0x280, 0x1e0, &(*g_game_palette), parent);
    else
        build_window_with_port_1060_a16b(s, "", 0x801, 0, 0, 0x280, 0x1e0, &(*g_game_palette), &g_game_record);
    SetRect(&r, 0, 0, 0x280, 0x1e0);
    load_picture_resource(((char *)((unsigned char *)(s) + 0x178)), 0x44e, 0x28, 0xc0, &(*g_game_palette));
    draw_selected_panels(s);
    set_handler_slot_4_1030_e522((char *)s + 0x33, dispatch_click_1030_1241);
    SetRect(&r, 0, 0, get_w_2_1030_e93a(s) / 3, 0x19);
    OffsetRect(&r, get_w_2_1030_e93a(s) / 3 / 2, get_w_4_1030_e959(s) - 0x1e);
    ctl_init_button_1030_e55b(((char *)((unsigned char *)(s) + 0x134)), (char *)s + 0x26, 0x64, &r, "~Ok");
    set_p_1c_1030_e5bf(((char *)((unsigned char *)(s) + 0x134)), dialog_click_19e);
    SetRect(&r, get_w_2_1030_e93a(s) / 2, 0, get_w_2_1030_e93a(s) / 2 + get_w_2_1030_e93a(s) / 3, 0x19);
    OffsetRect(&r, 0, get_w_4_1030_e959(s) - 0x1e);
    ctl_init_button_1030_e55b(((char *)((unsigned char *)(s) + 0x156)), (char *)s + 0x26, 0x65, &r, "~Cancel");
    set_p_1c_1030_e5bf(((char *)((unsigned char *)(s) + 0x156)), dialog_click_19e);
    win_center_1030_e5d9((char *)s + 0x26);
    MSDoAllTasks_1030_e5fe();
    win_show_focus_at_26_1030_e611(s);
    while ((*((col_i16 *)((unsigned char *)(s) + 0x19e))) == 0)
        MSDoTask_1030_e630();
    MSHideWindow_of_1030_e647((char *)s + 0x26);
    MSDoAllTasks_1030_e5fe();
    if ((*((col_i16 *)((unsigned char *)(s) + 0x1a0))) == 0) {
        for (i = 0; i < 4; i++)
            g_world_opt[i] = ((col_i16 *)((unsigned char *)(s) + 0x12c))[i] % 3;
        return 1;
    } else
        return 0;
}
