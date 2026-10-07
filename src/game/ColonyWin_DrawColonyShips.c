/* PORT: this file's own declarations of ColonyWin_UpdateColony, GRPort_String, copy_rect_to_screen_1010_e6a4 removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of sprite_width_1010_e60f removed: the header declares it from the definition (W1 step 3). */
/* ColonyWin_DrawColonyShips: candidate for 1010:87ff (1314 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawColonyShips
 * name-source: sibling-build
 *
 * Draws the colony window's dock panel -- the ships and wagons in port and the cargo of the selected
 * one -- the panel `ColonyWin_ColonyMouseShipsProper` and `ColonyWin_ColonyMouseShipsCargo` take clicks on.
 *
 * With no carrier in port (DGROUP:0xc14 is zero) it prints MISC word 11, "No Ships In Port", and
 * six empty hold sprites (SEG20:0x71a6). Otherwise the heading is "Loading: " (MISC word 23) and the
 * selected carrier's type name, and the carriers are laid out in two rows: four big sprites 0x22
 * apart on the first row (ships raised five pixels, all but the first one more) and up to sixteen
 * 5 pixels apart on the second, which are counted but not drawn. The selected carrier (DGROUP:0xc16)
 * is outlined in 0xff and the one at DGROUP:0xc18 in 0xfa, unless DGROUP:0xa08 is set.
 *
 * Then the selected carrier's six holds: each hold it has shows its cargo's goods sprite -- the
 * full-load picture set from 0x17 for 100 or more, the part-load set from 0x27 below that -- scaled
 * to 4/5 when the sprite is wider than an empty hold less ten; holds the type lacks show the empty
 * hold sprite. With `redraw`, the panel's rectangle goes to the screen.
 *
 * The highlight colour is an `unsigned char` passed to a `TextColor(char)`: Borland pushes a char
 * argument as a WORD straight from its frame slot (`PUSH WORD [BP-1Bh]`), with no CBW or MOV AH,0.
 * The call after the first-row sprite has an empty `else {}` (rule 62a).
 *
 * `g_carrier_highlight` (DGROUP:0xc18) is the SECONDARY highlight slot. Every write in the
 * corpus sets it and `g_selected_carrier` to the same value in one chained assignment, and
 * its only read is `ColonyWin_DrawColonyShips`, which boxes the selected carrier in colour 0xff
 * and this one in 0xfa -- so while the two agree, the dimmer branch never fires. Something
 * outside the corpus must move one without the other; the name says what the value DOES,
 * not what makes them differ, which is not established.
 *
 * RENAMED 2026-09-26 from colony_draw_carriers: the Mac build's ColonyWin::DrawColonyShips(short): this plus a redraw flag, painting the ships and wagons in port and the selected one's holds.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Font, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern void set_colony(int colony);

extern char *strings(int n);
extern void compute_colony_cargo_box(int i, int *x, int *y, int *pw, int *ph);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void colon_string(char *buf);
extern int nth_cargo_vessel(int carrier);

extern void PieceDraw(GRPort *port, int u, int flags, int x, int y, int span, int mode);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern void SetGlobalSpriteScale(int num, int den);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void ColonyWin_DrawColonyShips(ColonyWin *w, int redraw)
{
    int cx;
    int cy;
    int px;
    int py;
    int n;
    int rowi;
    int goods;
    int m1;
    int m2;
    int base;
    int amount;
    int per_row;
    unsigned char colour;
    RECT r;
    RECT s;
    RECT s2;
    RECT s3;
    RECT s4;
    char buf[0x50];
    register int i;
    register int u;

    set_colony(w->colony);
    ColonyWin_UpdateColony(w, 0xf2, 0x110, 0xaa, 0x73);
    if (g_carrier_count == 0) {
        TextColor(0x39);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, strings(g_misc_words[MISC_NO_SHIPS_IN_PORT]),
                             w->rect.left + 0x147, w->rect.top + 0x119, 0);
        for (i = 0; i < 6; i++) {
            compute_colony_cargo_box(i, &cx, &cy, &m1, &m2);
            Sprite_Draw(&s, &g_icon_sprites[ICON_EMPTY_HOLD], (GRPort *)w, w->rect.left + cx, w->rect.top + cy);
        }
    } else {
        buf[0] = 0;
        say_string(buf, g_misc_words[MISC_LOADING]);
        colon_string(buf);
        u = nth_cargo_vessel(g_selected_carrier);
        STRCAT(buf, strings(g_unit_types[g_units[u].type].name));
        TextColor(0x39);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x147, w->rect.top + 0x119, 0);
        cx = 0x106;
        cy = 0x13c;
        px = cx;
        n = 0;
        rowi = 0;
        per_row = 4;
        for (i = 0; g_carrier_count > i; i++) {
            u = nth_cargo_vessel(i);
            py = cy;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && rowi == 0) {
                py -= 5;
                if (i != 0)
                    py--;
            }
            if (rowi == 0)
                PieceDraw((GRPort *)w, u, 0, w->rect.left + px, w->rect.top + py, 0x20, 0x64);
            else {
            }
            if (g_selected_carrier == i || g_carrier_highlight == i) {
                colour = 0xfa;
                if (g_selected_carrier == i)
                    colour = 0xff;
                if (colour != 0 && g_scout_visit == 0) {
                    TextColor(colour);
                    SetRect(&r, w->rect.left + px - 0x10, w->rect.top + py - 3, w->rect.left + px + 0x10, w->rect.top + py + 0x20);
                    GRPort_DrawRect((GRPort *)w, &r);
                }
            }
            px += rowi == 0 ? 0x22 : 5;
            n++;
            if (n >= per_row) {
                n = 0;
                rowi++;
                if (rowi > 1)
                    break;
                px = 0xf5;
                cy = 0x119;
                per_row = 0x10;
            }
        }
        u = nth_cargo_vessel(g_selected_carrier);
        for (i = 0; i < 6; i++) {
            compute_colony_cargo_box(i, &cx, &cy, &m1, &m2);
            if (g_unit_types[g_units[u].type].holds > i) {
                goods = get_cargo(u, i);
                amount = get_cargo_amount(u, i);
                if (goods >= 0) {
                    base = amount >= 100 ? 0x17 : 0x27;
                    if (sprite_width_1010_e60f(&g_icon_sprites[base + goods])
                        > sprite_width_1010_e60f(&g_icon_sprites[ICON_EMPTY_HOLD]) - 10) {
                        SetGlobalSpriteScale(4, 5);
                        Sprite_Draw(&s2, &g_icon_sprites[base + goods], (GRPort *)w, w->rect.left + cx + 0xd, w->rect.top + cy + 3);
                        SetGlobalSpriteScale(1, 1);
                    } else
                        Sprite_Draw(&s3, &g_icon_sprites[base + goods], (GRPort *)w, w->rect.left + cx + 0xd, w->rect.top + cy + 3);
                }
            } else
                Sprite_Draw(&s4, &g_icon_sprites[ICON_EMPTY_HOLD], (GRPort *)w, w->rect.left + cx, w->rect.top + cy);
        }
    }
    if (redraw != 0) {
        SetRect(&r, w->rect.left + 0xf2, w->rect.top + 0x110, w->rect.left + 0x19c, w->rect.top + 0x183);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
}
