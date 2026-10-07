/* route_edit: candidate for 1028:ccf8 (252 bytes).  Verdict: RELOCATABLE.
 *
 * Opens the trade-route editor. A negative argument means "ask": `TRADESELECT` picks the
 * route, defaulting to the last one edited (DGROUP:0x1184), and a negative answer cancels.
 * The chosen index is remembered there.
 *
 * The editor is a ReportWin, and the route's first stop decides where its map view starts.
 * Stop id 0x3e7 -- 999, the sentinel for Europe rather than a colony -- puts it twenty
 * squares before SEG20:0x7796 in BOTH coordinates; any other id is a colony index and the
 * view goes to that colony's x and y.
 *
 * A unit record is allocated to carry the editor's state and freed again at the end, from
 * the top of the array (`g_game.unit_count - 1`), which is the record just taken.
 *
 * Two spellings the bytes fixed, both about the 0x4a-byte route record at +0x20.
 *
 * The field passed to `make_fake_unit_2` is at +0x20, not +0x21 -- one byte, and the two
 * are adjacent fields of the same record, so the wrong one compiles and grades DIFFERS
 * eight instructions later rather than anywhere the mistake.
 *
 * And it is UNSIGNED: the shipped promotion is `MOV AH,0`, where the `char kind` that
 * route_create.c declares gives `CBW`. That one-byte difference also accounted for a
 * persistent one-byte drift in every later offset, which reads as a much bigger problem
 * than it is.
 *
 * `g_4c24` IS WRITTEN ONCE AND NEVER READ. The `= 1` here is the only instruction in the image
 * that touches 0x4c24 (`tools/check_dead_dgroup.py`). Whatever asked "is a route being edited?"
 * is gone, so the flag keeps its address instead of a name promising that reader.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
#include "classes.h"

extern MainWin g_main_win;              /* SEG20:0x5ca */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int route_get(int kind, int route, char *msg);
extern int make_fake_unit_2(int kind, int tile_x, int tile_y, int owner);
extern int ReportWin_DoReport_parent(ReportWin *a, int b, char flag, void *d);
extern void delete_unit(int u);

extern void set_dest(int n);

void route_edit(int r)
{
    int x;
    int y;

    if (r < 0) {
        r = route_get(0, g_last_route, "TRADESELECT");
        if (r < 0)
            goto out;
    }
    g_last_route = (char)r;
    set_dest(r);
    set_route(0);
    if (g_cur_route_stop->colony == 0x3e7) {
        x = g_game.turn_nation - 20;
        y = g_game.turn_nation - 20;
    } else {
        x = g_colonies[g_cur_route_stop->colony].x;
        y = g_colonies[g_cur_route_stop->colony].y;
    }
    g_route_unit = make_fake_unit_2(g_cur_route_rec->sea, x, y, g_game.turn_nation);
    ReportWin_DoReport_parent((ReportWin *)&g_report_window, 0x63, 1, &g_main_win);
    g_4c24 = 1;
    delete_unit(g_game.unit_count - 1);
out:
    ;
}
