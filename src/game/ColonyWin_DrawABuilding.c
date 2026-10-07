/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of DrawIconRow, Sprite_Init removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_DrawABuilding: candidate for 1010:9e22 (1090 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawABuilding
 * name-source: sibling-build
 *
 * Draws one building site of the colony window's buildings panel, for `ColonyWin_DrawBuildings`:
 * the building's picture, what it produces, who works in it, and a number for the two buildings that
 * count something. The layout comes from the per-kind tables in SEG21 the click handler
 * (`ColonyWin_ColonyMouseBuildings`) also reads.
 *
 * The picture is resource 0x15e + building + 1, loaded into a local `Sprite`, with three substitutions:
 * building 0 shows picture 0 when the colony lacks it, and sites 0xf and 0x11 show picture 0x12 when
 * the colony lacks building 0xf, or picture 0x11 when it has both 0xf and 0x11. The building
 * `ColonyWin_DoBuildingFizzle` is
 * animating (DGROUP:0xc20) is not drawn at all.
 *
 * For a building with a job (or 0x11, 0x13, 0x14): the production row from `output_of_building`
 * at the kind's output position, then either the workers' row (`ColonyWin_DrawIndividual`, called
 * from two identical arms of a test for top level 0xc) or, for a negative worker count on a top-level
 * building, the people outside (`ColonyWin_DrawFort`). Sites 0xf and 0x1e then print the
 * colony's bytes at +0x95 and +0x96 when above one, twice, one pixel apart, in colour 0xff.
 *
 * FRAME LAYOUT AND INITIALISATION ORDER ARE DIFFERENT THINGS (rule 72). The shipped stores `yoff = 0`
 * before constructing the Sprite, yet `yoff` sits below all the other scalars. Declared first, it
 * lands at the top of the scalars; declared last but after the Sprite, its store follows the
 * constructor. Every scalar declared, `yoff = 0` last of them, and THEN `Sprite spr;` gives both.
 * The building and kind are `register` parameters (rule 69).
 *
 * RENAMED 2026-09-26 from colony_draw_site: the Mac build's ColonyWin::DrawABuilding(short, short, short, short): this plus four shorts, painting one built site for ColonyWin_DrawBuildings.
 */

#include "colonization.h"

#include "building.h"
#include "w16types.h"
/* PORT: this file's own Font, Sprite, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SImageButton, ColChild, SRegion, SButton, ColonyWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-



class DlgControl;








#include "records.h"

extern void set_colony(int colony);
extern int has_building(int bit);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int job_of_building(int building);
extern int guys_at_building(int building);
extern int output_of_building(int building, int *kind_out, int *pic_out, int *extra_out);
extern int base_building(int building);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColonyWin_DrawIndividual(ColonyWin *w, int job, int x, int y, int slot);
extern "C" void ColonyWin_DrawFort(ColonyWin *w, int x, int y, int count);

extern "C" void ColonyWin_DrawABuilding(ColonyWin *w, register int building, int x, int y, register int kind)
{
    int job;
    int workers;
    int out_kind;
    int amount;
    int pic;
    int marked;
    int count;
    int pic_id;
    int yoff = 0;
    Sprite spr;
    struct { int l, t, r, b; } out;
    char buf[0x50];

    set_colony(w->colony);
    g_draw_labels = g_show_icon_counts;
    pic_id = building + 1;
    if (building == 0 && has_building(BUILDING_STOCKADE) == 0)
        pic_id = 0;
    if (building == BUILDING_WAREHOUSE || building == BUILDING_STABLES) {
        if (has_building(BUILDING_WAREHOUSE) != 0) {
            if (has_building(BUILDING_STABLES) != 0)
                pic_id = 0x11;
        } else
            pic_id = 0x12;
    }
    Sprite_Init(&spr, pic_id + 0x15e);
    if (g_new_building == building)
        return;
    Sprite_Draw((RECT *)&out, &spr, w, w->rect.left + x, w->rect.top + y);
    job = job_of_building(building);
    if (building == BUILDING_WAREHOUSE && has_building(BUILDING_STABLES) != 0)
        building = BUILDING_STABLES;
    if (job >= 0 || building == BUILDING_PRINTING_PRESS || building == BUILDING_NEWSPAPER || building == BUILDING_STABLES) {
        workers = guys_at_building(building);
        amount = output_of_building(building, &out_kind, &pic, &marked);
        if (building == BUILDING_STABLES)
            yoff = 0x14;
        if (amount != 0)
            DrawIconRow(w, pic, amount, amount, g_kind_out_x[kind] + x + w->rect.left + 5,
                                    g_kind_out_y[kind] + y + yoff + w->rect.top,
                                    g_kind_out_w[kind] - 10, g_kind_out_w[kind] - 10, marked, 0);
        if (workers != 0) {
            if (workers > 0) {
                if (base_building(building) == BUILDING_SCHOOLHOUSE)
                    ColonyWin_DrawIndividual(w, job, g_kind_row_x[kind] + x, g_kind_row_y[kind] + y, kind);
                else
                    ColonyWin_DrawIndividual(w, job, g_kind_row_x[kind] + x, g_kind_row_y[kind] + y, kind);
            } else if (base_building(building) == 0)
                ColonyWin_DrawFort(w, g_kind_row_x[kind] + x, g_kind_row_y[kind] + y, 0 - workers);
        }
    }
    if (building == BUILDING_STABLES) {
        if (has_building(BUILDING_WAREHOUSE) != 0)
            building = BUILDING_WAREHOUSE;
        yoff = -11;
    } else
        yoff = 0;
    count = 0;
    if (building == BUILDING_WAREHOUSE)
        count = g_colonies[w->colony].warehouses;
    if (building == BUILDING_CAPITOL)
        count = g_colonies[w->colony].capitols;
    if (count > 1) {
        LTOA1(count, buf, 10);
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_heading_font, buf, (g_kind_w[kind] >> 1) + (w->rect.left + x),
                             (g_kind_h[kind] >> 1) + (w->rect.top + y) + yoff, 0);
        GRPort_String(w, (unsigned *)&g_heading_font, buf, (g_kind_w[kind] >> 1) + (w->rect.left + x) + 1,
                             (g_kind_h[kind] >> 1) + (w->rect.top + y) + yoff, 0);
    }
    g_draw_labels = 0;
}
