/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* ColonyWin_ColonyMouseMap: candidate for 1010:544e (1157 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseMap
 * name-source: sibling-build
 * size-source: 1157, not the census 1153. The trailing 4 bytes are the dead closing-brace epilogue.
 *
 * Panel 1 of the colony window, the 3x3 of work squares drawn at (0x1a8, 0x24) in 64-pixel cells:
 * a click, and the drop target for a colonist dragged out of another panel. It returns unless the
 * point is in panel 1, and does nothing on the grid's border (column or row 0 or 4).
 *
 *   - In hover mode (DGROUP:0x9ea) it only shows the square's description,
 *     `label_map`, and only when the square changed.
 *   - In Colonopedia mode (0x9dc) it opens the square's terrain article, unless the square is
 *     marked 0x10.
 *   - As a DROP (panel word 6) on an empty, unmarked square it puts the dragged colonist to work:
 *     `colony_map_click`'s body, inlined -- the tribe's price asked through
 *     `try_to_set_job` as job 8 (fishing) on water or 0 on land for a colonist whose job
 *     is 0, 8 or 9 and up, the colonist moved with `pick_up_colonist` + `set_field`.
 *   - Otherwise a click on a new square selects it and assigns the selected colonist, then waits
 *     5 ticks. A button still held on an occupied square starts a DRAG of that colonist's portrait
 *     after another 5; if the drop lands more than 10 pixels away it is offered to panels 2
 *     (`ColonyWin_ColonyMouseBuildings`), 1 (this function) and 0 (`ColonyWin_ColonyMouseColonists`) under panel word 6. A click
 *     that turns out not to be a drag assigns the colonist, unless the first step already did.
 *
 * THE INLINED ASSIGNMENT REUSES THE COLUMN AND ROW REGISTERS for the map coordinates, both in the
 * drop arm and the pedia arm; declared separately, the second pair would not be in SI and DI.
 * The nested drag tests end in `goto done` on both arms of the distance test, which is why a
 * dead jump sits between them. The empty `else {}` after the first selection is rule 62a.
 *
 * RENAMED 2026-09-26 from colony_tiles_click: the Mac build's ColonyWin::ColonyMouseMap(short, short): this plus x and y, a click (or a drop) on the 3x3 work map. The width matches.
 */

#include "colonization.h"

#include "jobs.h"
#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e) ? 1 : -1];
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

extern char g_pedia_win;               /* SEG20:0x387c, declared */
extern void set_colony(int colony);

extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
extern char get_field(int col, int row);
extern void arm_indians(int col, int row);
extern "C" int get_job(int colonist);   /* PORT: C linkage, as defined (W2) */
extern int terrain_is_ocean(int x, int y);
extern "C" int try_to_set_job(int c, int job);   /* PORT: C linkage, as defined (W2) */
extern void pick_up_colonist(int id);
extern "C" void set_field(int col, int row, int colonist);   /* PORT: C linkage, as defined (W2) */
extern "C" int Screen_LButtonDownNow(void *w);   /* PORT: C linkage, as defined (W2) */
extern int get_icon(int colonist);
extern "C" void ColChild_DragIcon(ColChild *w, void *sprite, int x, int y, int *rx, int *ry);   /* PORT: C linkage, as defined (W2) */
extern void ColonyWin_ColonyMouseBuildings(ColonyWin *w, int x, int y);
extern void ColonyWin_ColonyMouseColonists(ColonyWin *w, int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);

extern "C" void label_map(int cell);
extern "C" void colony_change_selected(void);
extern "C" void colony_map_click(void);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void ColonyWin_ColonyMouseMap(ColonyWin *w, int x, int y)
{
    int rx;
    int ry;
    int ok;
    char colonist;
    int job;
    int cell;
    int terrain;
    char assigned;
    int ox;
    int oy;
    register int col;
    register int row;

    assigned = 0;
    if (ColonyMouseArea(x, y) != 1)
        return;
    set_colony(w->colony);
    rx = x - 0x1a8;
    ry = y - 0x24;
    rx = min_int_1010_e5d0(rx, 0xbf);
    ry = min_int_1010_e5d0(ry, 0xbf);
    col = rx / 0x40 + 1;
    row = ry / 0x40 + 1;
    if (col == 0 || row == 0 || col == 4 || row == 4)
        goto done;
    if (g_hover_mode != 0) {
        cell = (row << 3) + col;
        if (g_labelled_cell != cell)
            label_map(cell);
    } else if (g_pedia_mode != 0 && g_grid_marks[col][row] != 0x10) {
        col = g_cur_colony_rec->x + col - 2;
        row = g_cur_colony_rec->y + row - 2;
        terrain = terrain_at(col, row);
        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 2, terrain, &(DialogWin &)*w);
    } else if (w->panel == 6) {
        if ((int)get_field(col, row) < 0 && g_grid_marks[col][row] == 0) {
            g_cell_col = col;
            g_cell_row = row;
            arm_indians(col, row);
            job = get_job(g_selected_colonist);
            if (job >= JOB_INDOOR_FIRST || job == JOB_FISHERMAN || job == JOB_FARMER) {
                col = g_cur_colony_rec->x + g_cell_col - 2;
                row = g_cur_colony_rec->y + g_cell_row - 2;
                if (terrain_is_ocean(col, row) != 0)
                    ok = !try_to_set_job(g_selected_colonist, 8);
                else
                    ok = !try_to_set_job(g_selected_colonist, 0);
            } else
                ok = !try_to_set_job(g_selected_colonist, job);
            if (ok != 0) {
                pick_up_colonist(g_selected_colonist);
                set_field(g_cell_col, g_cell_row, g_selected_colonist);
            }
            colony_change_selected();
        }
    } else {
        if (g_cell_col != col || g_cell_row != row || g_panel_mode != 0) {
            g_panel_mode = 0;
            g_cell_col = col;
            g_cell_row = row;
            assigned = 1;
            if (g_colony_clicked == 0)
                g_colony_drag_armed = 0;
            colony_map_click();
            colony_change_selected();
        } else {
        }
        MSWaitTicks_1010_e5f6(5);
        if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0) {
            if (g_colony_drag_armed != 0) {
                colonist = get_field(col, row);
                if ((int)colonist >= 0) {
                    MSWaitTicks_1010_e5f6(5);
                    if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0 && g_pedia_mode == 0) {
                        ox = x;
                        oy = y;
                        ColChild_DragIcon((ColChild *)w, &g_icon_sprites[get_icon(colonist)], ox, oy, &x, &y);
                        if (ox + 10 < x || ox - 10 > x || oy + 10 < y || oy - 10 > y) {
                            w->panel = 6;
                            g_9e2 = 0;
                            g_colony_drop_active = 1;
                            g_colony_clicked = 0;
                            ColonyWin_ColonyMouseBuildings(w, x, y);
                            ColonyWin_ColonyMouseMap(w, x, y);
                            ColonyWin_ColonyMouseColonists(w, x, y);
                            w->panel = 0;
                            g_colony_clicked = 1;
                            g_9e2 = 1;
                            goto done;
                        } else
                            goto done;
                    } else {
                        if (g_pedia_mode == 0 && assigned == 0)
                            colony_map_click();
                    }
                }
            } else {
                if (g_pedia_mode == 0 && assigned == 0)
                    colony_map_click();
            }
        } else {
            if (g_pedia_mode == 0 && assigned == 0)
                colony_map_click();
        }
    }
done:
    ;
}
