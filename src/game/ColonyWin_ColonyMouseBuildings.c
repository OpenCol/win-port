/* ColonyWin_ColonyMouseBuildings: candidate for 1010:58d3 (1241 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseBuildings
 * name-source: sibling-build
 * size-source: 1241, not the census 1237. The census stops 4 bytes before the RETF;
 * WhichColonist starts immediately after it.
 *
 * Panel 2 of the colony window, the buildings: a click, and the third drop target of the colonist
 * drag the work squares (`ColonyWin_ColonyMouseMap`) and the colonist row
 * (`ColonyWin_ColonyMouseColonists`) start. It finds the building under the point by walking the 15
 * building sites -- position from SEG21:0x6a, kind from SEG21:0x0, the kind's width and height
 * from SEG21:0x10 and 0x1a -- and, for a building with a job, which worker slot of its row the
 * point is on (`InIconRow`, the row origin from SEG21:0x24/0x2e). The walk stops at
 * the first site that holds a building.
 *
 *   - Hover mode (DGROUP:0x9ea) shows the building's information when the building changed (0x9f0).
 *   - A DROP (panel word 6) on a building with a job puts the dragged colonist to work there
 *     through `try_to_set_job`; job 0x15 (JOB_SOLDIER) instead opens the colonist-job popup (`colony_select_profession(1)`), and
 *     only for a colonist whose job is below 0x13.
 *   - A click on a worker selects that colonist, and a held button drags its portrait and offers
 *     the drop to panels 2, 1 and 0 (the lopsided 5/10-pixel test of the colonist row).
 *   - A click that is not a drag on a worker: select it or, when it is already selected, open the colonist-job popup (`colony_select_profession(0)`), and in
 *     Colonopedia mode open the specialty article (0x1c read as 0x13, FREE COLONISTS). On a
 *     building but not a worker: in Colonopedia mode its building article (category 4);
 *     otherwise assign the selected colonist to its job, or -- for a building whose top level is
 *     0x12, BUILDING18 "CUSTOM HOUSE" -- open `colony_custom_house`.
 *
 * The worker count is `n > 0 ? n : ~n + 1` -- the shipped NOT / INC, rule 11 -- with
 * `guys_at_building` called again in whichever arm runs. The drag-start store is
 * `g_row_colonist = g_selected_colonist = colonist`, 0x4bbc first. The selection is restored
 * twice on the path where the button came up before the second wait, in both the inner else
 * and the statement after it; the shipped code carries both stores.
 *
 * RENAMED 2026-09-27 from colony_building_click: BEYOND THE PROPOSALS. Mac ColonyWin::ColonyMouseBuildings(short, short), CODE 13 0x263e, 1202 bytes. Callers (bsr): ColonyMouseParse, ColonyMouseMove, ColonyMouseMap, itself, and ColonyMouseColonists. Windows callers: colony_panel_rclick, colony_panel_click, ColonyWin_ColonyMouseMap, itself, colony_row_click, the same five. Callees item for item: ColonyMouseArea, set_colony, job_of_building, guys_at_building x3, InIconRow, base_building x2, nth_guy_in_job, label_building, get_job, colony_select_profession x3, try_to_set_job x2, colony_change_selected x4, MSWaitTicks x2, LButtonDownNow x2, get_icon, DragIcon, ColonyMouseMap, get_specialty, ShowModal x2 (PediaWin_ShowModal x2), reset_colony x2, colony_custom_house. Arity: this+2 against (w, x, y). Layout: after ColonyMouseMap on both. Size 1202 against 1241. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "jobs.h"
#include "w16types.h"
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

extern char g_pedia_win;               /* SEG20:0x387c, declared */

extern void set_colony(int colony);
extern int job_of_building(int building);
extern int guys_at_building(int building);
extern int InIconRow(int pic, int a, int n, int x, int y, int span, int span2, int style, int px, int py);
extern int base_building(int building);
extern int nth_guy_in_job(int job, int nth);
extern int get_job(int colonist);
extern void colony_select_profession(int unit_roles_only);
extern int try_to_set_job(int c, int job);
extern int Screen_LButtonDownNow(void *w);
extern int get_icon(int colonist);
extern void ColChild_DragIcon(ColChild *w, void *sprite, int x, int y, int *rx, int *ry);
extern void ColonyWin_ColonyMouseColonists(ColonyWin *w, int x, int y);
extern int get_specialty(int u);
extern void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ColonyMouseArea(int x, int y);
extern "C" void label_building(int b, int x, int y, int kind);
extern "C" void colony_change_selected(void);

extern "C" void ColonyWin_ColonyMouseMap(ColonyWin *w, int x, int y);
extern "C" void reset_colony(void);
extern "C" void colony_custom_house(void);

void ColonyWin_ColonyMouseBuildings(ColonyWin *w, int x, int y)
{
    register int building = -1;
    register int job = -1;
    int i;
    int bx0;
    int by0;
    int kind;
    int slot = -1;
    int colonist;
    int workers;
    int saved;
    int ox;
    int oy;

    if (ColonyMouseArea(x, y) != 2)
        goto done;
    set_colony(w->colony);
    for (i = 0; i < 15 && building < 0; i++) {
        bx0 = g_site_pos[i].x;
        by0 = g_site_pos[i].y;
        kind = g_site_kind[i];
        if (x > bx0 && g_kind_w[kind] + bx0 > x && y > by0 && g_kind_h[kind] + by0 > y) {
            building = g_site_building[i];
            if (building >= 0) {
                job = job_of_building(building);
                if (job >= 0) {
                    workers = guys_at_building(building) > 0 ? guys_at_building(building)
                                                           : ~guys_at_building(building) + 1;
                    slot = InIconRow(job + 0x52, workers, workers,
                                                  g_kind_row_x[kind] + bx0, g_kind_row_y[kind] + by0,
                                                  g_slot_span[kind], g_slot_span[kind], 2, x, y);
                    if (base_building(building) == 0) {
                        if (slot >= 0)
                            colonist = g_cur_colony_rec->pop + slot;
                    } else if (slot >= 0)
                        colonist = nth_guy_in_job(job, slot);
                }
            }
        }
    }
    if (building < 0)
        goto done;
    if (g_hover_mode != 0) {
        if (g_labelled_building != building) {
            g_labelled_building = building;
            label_building(building, bx0, by0, kind);
        }
    } else if (w->panel == 6) {
        if (job >= 0) {
            if (job == JOB_SOLDIER) {
                if (get_job(g_selected_colonist) < 0x13)
                    colony_select_profession(1);
            } else {
                try_to_set_job(g_selected_colonist, job);
                colony_change_selected();
            }
        }
    } else {
        saved = g_selected_colonist;
        MSWaitTicks_1010_e5f6(5);
        if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0 && slot >= 0) {
            g_panel_mode = 1;
            g_row_colonist = g_selected_colonist = colonist;
            if (g_colony_clicked == 0)
                g_colony_drag_armed = 0;
            colony_change_selected();
            MSWaitTicks_1010_e5f6(5);
            if (Screen_LButtonDownNow(&(DialogWin &)*w) != 0) {
                ox = x;
                oy = y;
                ColChild_DragIcon((ColChild *)w, &g_icon_sprites[get_icon(colonist)], ox, oy, &x, &y);
                if (ox - 5 > x || ox + 10 < x || oy - 10 > y || oy + 10 < y) {
                    w->panel = 6;
                    g_9e2 = 0;
                    g_colony_drop_active = 1;
                    g_colony_clicked = 0;
                    ColonyWin_ColonyMouseBuildings(w, x, y);
                    ColonyWin_ColonyMouseMap(w, x, y);
                    ColonyWin_ColonyMouseColonists(w, x, y);
                    w->panel = 0;
                    g_9e2 = 1;
                    g_colony_clicked = 1;
                    goto done;
                } else {
                    g_selected_colonist = saved;
                    goto done;
                }
            } else {
                g_selected_colonist = saved;
            }
        }
        g_selected_colonist = saved;
        if (slot >= 0) {
            if (g_selected_colonist != colonist) {
                g_row_colonist = g_selected_colonist = colonist;
                colony_change_selected();
            } else if (g_pedia_mode == 0)
                colony_select_profession(0);
            if (g_pedia_mode != 0) {
                job = get_specialty(g_selected_colonist);
                if (job == JOB_COLONIST_ALT)
                    job = 0x13;
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, job, &(DialogWin &)*w);
                reset_colony();
            }
        } else if (g_pedia_mode != 0) {
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 4, building, &(DialogWin &)*w);
            reset_colony();
        } else if (job >= 0) {
            if (job == JOB_SOLDIER)
                colony_select_profession(1);
            else {
                try_to_set_job(g_selected_colonist, job);
                colony_change_selected();
            }
        } else if (base_building(building) == 0x12)
            colony_custom_house();
    }
done:
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
}
