/* MapWin_LeftMouseClick -- shipped at 1038:9371.
 *
 * Ported by tools/port.py from matched/game/MapWin_LeftMouseClick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern char far g_view_nation;                       SEG20:0x7798
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int g_map_lclick_busy;                        DGROUP:0x1c50
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *   extern char far g_info_win;                          SEG20:0x1566
 *
 * The file's own MapRect, MapWin, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1038_e2a2(void);
void close_dialog_1038_e279(void *w);

/* 0xca bytes at SEG20:0x7c28 */








extern char * current_window(void);
extern void MapWin_WinPos2MapPos(MapWin *o, col_i16 x, col_i16 y, col_i16 *mx, col_i16 *my);
extern col_i16 colony_at(col_i16 x, col_i16 y);
extern void colony_window_open(void *w, col_i16 colony);
extern void MapWin_Reposition(MapWin *o, col_i16 x, col_i16 y);
extern void set_cursor_pos(col_i16 x, col_i16 y);
extern void window_relayout(void *rec);
extern void arrived_landfall(void *rec, col_i16 dir);

/* same object: 4-byte `push cs; call ` */



void MapWin_LeftMouseClick(col_i16 x, col_i16 y)
{
    MapWin *o;
    col_i16 mx;
    col_i16 my;
    col_i16 colony;
    char *w;

    if (g_map_lclick_busy != 0)
        return;
    g_map_lclick_busy = 1;
    w = current_window();
    o = (MapWin *)(w != 0 ? w - 0x26 : w);
    if ((*((MapRect *)((unsigned char *)(o) + 0x76a))).left > x || (*((MapRect *)((unsigned char *)(o) + 0x76a))).right < x || (*((MapRect *)((unsigned char *)(o) + 0x76a))).top > y || (*((MapRect *)((unsigned char *)(o) + 0x76a))).bottom < y) {
        close_dialog_1038_e279((char *)o + 0x26);
    } else {
        x -= (*((MapRect *)((unsigned char *)(o) + 0x76a))).left;
        y -= (*((MapRect *)((unsigned char *)(o) + 0x76a))).top;
        if (o->drag == -1) {
            MapWin_WinPos2MapPos(o, x, y, &mx, &my);
            colony = colony_at(mx, my);
            if (colony >= 0 && (g_colonies[colony].nation == g_view_nation || g_reveal_all != 0))
                colony_window_open(&(*g_colony_window), colony);
            else
                MapWin_Reposition(o, x, y);
            if (g_view_mode == 1) {
                set_cursor_pos(mx, my);
                window_relayout(&(*g_info_win));
            }
        } else if (o->drag == 0) {
        } else {
            if (g_view_mode != 1)
                arrived_landfall(&(*g_game_record2), o->drag - 1);
        }
    }
    MSDoAllTasks_1038_e2a2();
    g_map_lclick_busy = 0;
}
