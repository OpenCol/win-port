/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of copy_rect_to_screen removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_DrawEuropeShips: candidate for 1048:91ac (1045 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawEuropeShips
 * name-source: sibling-build
 *
 * Draws the Europe screen's dock panel: the ships in port and the holds of the selected one.
 * It is `ColonyWin_DrawColonyShips` for Europe, the panel `EuropeWin_EuropeMouseShipsProper`
 * and `EuropeWin_EuropeMouseShipsCargo` take clicks on, and it compiled RELOCATABLE on the first build.
 *
 * With no ship in port (DGROUP:0x1fca) it prints MISC word 11, "No Ships In Port", over six
 * empty hold sprites. Otherwise:
 *   - THE HEADING is "Loading: " (MISC word 23) and the selected ship's (DGROUP:0x4cde) type name.
 *     A MERCHANTMAN's name goes on a second line in the colour at DGROUP:0x337f, too long to share;
 *   - THE SHIPS are drawn in a row by `EuropeWin_DrawTransitUnit`, from x 0x11f. The selected
 *     ship is outlined in colour 10. It is 15 while goods are being dragged (0x1fc4 == 1 with
 *     0x4cea clear), and the ship at 0x4ce2 is 15 while 0x1fc0 == 1 with 0x4cd8 set;
 *   - THE HOLDS show the selected ship's cargo as goods sprites. The full-load set starts at 0x17
 *     for 100 or more, the part-load set at 0x27 below that. Holds the type lacks show the empty
 *     sprite. Unlike the colony panel there is no 4/5 scaling of wide sprites.
 * With `redraw`, the panel's rectangle goes to the screen.
 *
 * `TextColor` takes a `char`, so the byte at 0x337f is pushed as a word straight from memory (rule 71).
 *
 * `g_europe_ship_highlight` (DGROUP:0x4ce2) is the SECONDARY highlight slot. Every write in the
 * corpus sets it and `g_europe_selected_ship` to the same value in one chained assignment, and
 * its only read is `EuropeWin_DrawEuropeShips`, which draws the selected ship in colour 10
 * and this one only in 15, under the `0x1fc0 == 1 && 0x4cd8` test -- so while the two agree, no ship is highlighted without being selected. Something
 * outside the corpus must move one without the other; the name says what the value DOES,
 * not what makes them differ, which is not established.
 *
 * `g_4cd8` KEEPS ITS ADDRESS FOR A NAME, deliberately. Nothing in the 22,620 bytes of DGROUP
 * ever writes it -- `tools/check_dead_dgroup.py` finds two instructions touching 0x4cd8 in the
 * whole image and both are this `cmp word [0x4cd8],0x0`, in this file and its neighbour. It
 * ships zero, and its neighbours `g_cur_nation` at 0x4cc8 and `g_cheat_step` at 0x4cf6 are both
 * stored to absolutely, so no block load covers it either. The guard is therefore decided
 * before the game starts and this arm cannot run. A role name like `g_selection_flash` would
 * read as a flash the image never performs, so the address stays and this note carries the
 * meaning instead.
 *
 * `g_4cea` IS READ HERE AND WRITTEN NOWHERE, so `g_drag_mode == 1 && g_4cea == 0` is really
 * `g_drag_mode == 1`. One instruction in the image touches 0x4cea and it is this `cmp`; DGROUP
 * ships it zero; and 0x4ce8 and 0x4cec, the cells either side, are both stored to absolutely, so
 * no block load covers it (`tools/check_dead_dgroup.py`). The second test is decided before the
 * game starts. This is the same class as `g_4cd8` above and the same reason neither is named.
 *
 * RENAMED 2026-09-26 from europe_draw_dock_1048_91ac: the Mac build's EuropeWin::DrawEuropeShips(short): this plus a redraw flag, painting the ships in port and the selected ship's holds.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Font, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern int g_ships_in_port;                    /* DGROUP:0x1fca, ships in port */

extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cw, int ch);

extern char *strings(int n);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void colon_string(char *buf);
extern void space_string(char *buf);
extern void EuropeWin_DrawTransitUnit(EuropeWin *w, int u, int x, int y, int gap, int *spacing, int colour);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);
extern "C" void compute_europe_cargo_box(int i, int *x, int *y, int *a, int *b);
extern "C" int nth_europe_vessel(int n);

void EuropeWin_DrawEuropeShips(EuropeWin *w, int redraw)
{
    int spacing;
    int x;
    int y;
    int goods;
    int m1;
    int m2;
    int amount;
    int base;
    int colour;
    RECT r;
    RECT s;
    RECT s2;
    RECT s3;
    char buf[0x50];
    register int i;
    register int u;

    set_europe(w->nation);
    EuropeWin_UpdateEurope(w, 0x11c, 0x102, 0xa3, 0x90);
    if (g_ships_in_port == 0) {
        TextColor(0x77);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, strings(g_misc_words[MISC_NO_SHIPS_IN_PORT]),
                             w->rect.left + 0x16d, w->rect.top + 0x10c, 0);
        for (i = 0; i < 6; i++) {
            compute_europe_cargo_box(i, &x, &y, &m1, &m2);
            Sprite_Draw(&s, &g_icon_sprites[ICON_EMPTY_HOLD], (GRPort *)w, w->rect.left + x, w->rect.top + y);
        }
    } else {
        u = nth_europe_vessel(g_europe_selected_ship);
        buf[0] = 0;
        say_string(buf, g_misc_words[MISC_LOADING]);
        colon_string(buf);
        if (g_units[u].type != UNIT_MERCHANTMAN) {
            space_string(buf);
            say_string(buf, g_unit_types[g_units[u].type].name);
        }
        TextColor(0x77);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x16d, w->rect.top + 0x10c, 0);
        if (g_units[u].type == UNIT_MERCHANTMAN) {
            buf[0] = 0;
            say_string(buf, g_unit_types[g_units[u].type].name);
            TextColor(g_text_colour);
            GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x16d, w->rect.top + 0x11b, 0);
        }
        x = 0x11f;
        spacing = 0;
        for (i = 0; i < g_ships_in_port; i++) {
            u = nth_europe_vessel(i);
            colour = -1;
            if (i == g_europe_selected_ship || i == g_europe_ship_highlight) {
                if (i == g_europe_selected_ship)
                    colour = 10;
                if (g_drag_mode == 1 && g_4cea == 0)
                    colour = 15;
                if (g_selection_kind == 1 && g_4cd8 != 0 && i == g_europe_ship_highlight)
                    colour = 15;
            }
            EuropeWin_DrawTransitUnit(w, u, x, 5, 2, &spacing, colour);
        }
        u = nth_europe_vessel(g_europe_selected_ship);
        for (i = 0; i < 6; i++) {
            compute_europe_cargo_box(i, &x, &y, &m1, &m2);
            if (g_unit_types[g_units[u].type].holds > i) {
                goods = get_cargo(u, i);
                amount = get_cargo_amount(u, i);
                if (goods >= 0) {
                    base = amount >= 100 ? 0x17 : 0x27;
                    Sprite_Draw(&s2, &g_icon_sprites[base + goods], (GRPort *)w, w->rect.left + x + 0xc, w->rect.top + y);
                }
            } else
                Sprite_Draw(&s3, &g_icon_sprites[ICON_EMPTY_HOLD], (GRPort *)w, w->rect.left + x, w->rect.top + y);
        }
    }
    if (redraw != 0) {
        SetRect(&r, w->rect.left + 0x11c, w->rect.top + 0x102, w->rect.left + 0x1bf, w->rect.top + 0x192);
        copy_rect_to_screen(w, &r);
    }
}
