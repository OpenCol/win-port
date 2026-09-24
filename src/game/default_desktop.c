/* default_desktop -- shipped at 1048:ce37.
 *
 * Ported by tools/port.py from matched/game/default_desktop.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_report_window;                     SEG20:0x1d06
 *   extern char far g_pedia_ctx;                         SEG20:0x387c
 *   extern char far g_win_158c;                          SEG20:0x158c
 *   extern char far g_win_e06;                           SEG20:0x0e06
 *   extern char far g_win_67e;                           SEG20:0x067e
 *   extern char far g_game_record;                       SEG20:0x05ca
 *   extern char far g_info_win;                          SEG20:0x1566
 *   extern char far g_map_overlay;                       SEG20:0x0de0
 *   extern char far g_map_window;                        SEG20:0x0658
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1048_d4f6(void *p);
col_i16 get_w_4_1048_d7c5(void *p);
col_i16 rect_height_1048_d6ba(RECT *r);
col_i16 rect_width_1048_d69b(RECT *r);
col_i16 win_client_height_1048_e5aa(void *o);
col_i16 win_client_width_1048_e585(void *o);
void MSHideWindow_of_1048_d4d5(void *o);
void close_dialog_at_26_1048_dd42(void *o);
void win_move_to_rect_1048_d6da(void *o, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void close_report_window(void *w);
extern void win_close_1060_2a47(void *w);
extern void MSGetDesktopSize(RECT *r);


/* same object: 4-byte `push cs; call ` (rule 47) */










void default_desktop(void *unused1)
{
    col_i16 ch;
    col_i16 mh;
    RECT all;
    RECT left;
    RECT top;
    RECT bottom;
    RECT desk;
    register col_i16 cw;
    register col_i16 mw;

    close_report_window(&g_report_window);
    win_close_1060_2a47(&(*g_pedia_ctx));
    MSHideWindow_of_1048_d4d5(&g_win_158c);
    MSHideWindow_of_1048_d4d5(&g_win_e06);
    MSHideWindow_of_1048_d4d5(&g_win_67e);
    cw = win_client_width_1048_e585(&g_game_record);
    ch = win_client_height_1048_e5aa(&g_game_record);
    mw = get_w_2_1048_d4f6(&(*g_info_win));
    mh = get_w_4_1048_d7c5(&(*g_info_win));
    SetRect(&all, 0, 0, cw, ch);
    SetRect(&left, 0, 0, cw - mw, ch);
    MSGetDesktopSize(&desk);
    if (mw + mh > ch
            && (rect_width_1048_d69b(&desk) <= 0x280
                || rect_height_1048_d6ba(&desk) <= 0x1e0)) {
        SetRect(&top, cw - mw, 0, cw, mw - (mw + mh - ch));
        SetRect(&bottom, cw - mw, mw - (mw + mh - ch), cw,
                mw - (mw + mh - ch) + mh);
    } else {
        SetRect(&top, cw - mw, 0, cw, mw);
        SetRect(&bottom, cw - mw, mw, cw, mw + mh);
    }
    win_move_to_rect_1048_d6da(&g_win_67e, &left);
    win_move_to_rect_1048_d6da(&g_win_e06, &top);
    win_move_to_rect_1048_d6da(&g_win_158c, &bottom);
    close_dialog_at_26_1048_dd42(&(*g_info_win));
    close_dialog_at_26_1048_dd42(&(*g_map_overlay));
    close_dialog_at_26_1048_dd42(&g_map_window);
}
