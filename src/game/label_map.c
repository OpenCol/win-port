/* label_map: candidate for 1010:3cb4 (920 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: label_map
 * name-source: sibling-build
 *
 * The one-line description of a square in the colony window's 5x5 work area -- "Grassland/Minor
 * River/Road" -- shown in a small popup beside it. `cell` packs the column in its low three bits
 * and the row above them; the popup is not shown in the mode bit 0 of SEG20:0x7786 selects, for a
 * negative cell, or for a square marked 0x10 in the grid at SEG21:0x14a. The cell is kept at
 * DGROUP:0x9f4 and the popup at 0x9ec, which `label_remove_label` (called first) disposes of.
 *
 * The text is the terrain's name, or "Lake" (MISC word 40) for OCEAN whose plane-2 low nibble is
 * not 1 (the low nibble looks like a water-region number, with 1 the open sea; that part is a
 * reading, not established). Then, each after a "/":
 *
 *   - MAP0_RIVER: "Major River" or "Minor River" (OTHER_NAMES words 2 and 3), by MAP0_MAJOR_RIVER;
 *   - MAP1_SETTLEMENT or MAP1_ROAD: "Road" (MISC word 31);
 *   - MAP1_PLOWED: "Plowed" (MISC word 83).
 *
 * THIS FUNCTION IS WHERE THOSE BITS GOT THEIR NAMES. Plane 0 bit 6 was "unaccounted for" and plane
 * 1 bits 3 and 6 had no names; the words the game prints for them are the evidence, and
 * include/terrain.h and the two map findings now carry them (RE_STATUS 344).
 *
 * THE CENTRE-SQUARE TEST USES THE WRONG VARIABLES. Beside the road bits it tests `tx == 2 && ty == 2`
 * on the ABSOLUTE map coordinates. Column 2, row 2 is the colony's own square, which is what the
 * test reads as meant; as shipped it adds "Road" to one square the map's top-left corner.
 *
 * The popup is `new GameWin` (0xc4 bytes), text width plus one (the extent less one, plus two), 0x12 tall, centred on the cell and
 * clamped between x 0x1a8 and 0x268 minus its width; the text is drawn shadowed in colour 0xff at
 * its centre, with `LabelKeyClick` in handler slot 0x32. The three "/" literals are
 * separate strings at DGROUP 0xa22, 0xa24 and 0xa26, which a green verdict cannot check (§119).
 * `continent_at` returns unsigned char: the shipped compares AL with no widening.
 *
 * RENAMED 2026-09-26 from colony_square_info: the Mac build's label_map(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller ColonyMouseMap; callee ColorSync; callee TextColor; callee minimax; callee terrain_at), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "colopt.h"
#include "terrain.h"
#include "misc.h"
#include "labels.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a
#include "w16types.h"

#pragma option -a-


class DlgControl;


#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern GameWin *g_colony_popup;         /* DGROUP:0x9ec */

   /* 0x41e bytes: the palette, its handle and the cycling state */

extern unsigned char continent_at(int x, int y);
extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);

extern void say_terrain(char *s, int kind);

extern void GameWin_Init(GameWin *o, char *name, int style,
                                                 int x, int y, int w, int h,
                                                 CTable *bits, Screen *parent);
extern void GRPort_ColorSync(GRPort *port, CTable *page);
extern void GRPort_FillColor_all(GRPort *p, int color);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
extern void LabelKeyClick(int key);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void label_remove_label(void);
extern "C" int MSGetTextLength_1010_e2d2(void *font, const char *s);
extern "C" void set_on_keydown_1010_e36f(void *table, void *h);

extern "C" void show_window_1010_e404(void *o);

extern "C" void label_map(int cell)
{
    int x;
    int y;
    int w;
    int cx;
    int cy;
    int col;
    int row;
    int ext;
    int kind;
    char buf[0x50];
    register int tx;
    register int ty;

    label_remove_label();
    if (g_game.colony_options & COLOPT_NO_TERRAIN_LABELS)
        return;
    g_labelled_cell = cell;
    if (cell < 0)
        return;
    col = cell & 7;
    row = cell >> 3;
    if (g_grid_marks[col][row] & 0x10)
        return;
    tx = g_cur_colony_rec->x + col - 2;
    ty = g_cur_colony_rec->y + row - 2;
    kind = terrain_at(tx, ty);
    buf[0] = 0;
    if (kind == TERRAIN_OCEAN && continent_at(tx, ty) != 1)
        say_string(buf, g_misc_words[MISC_LAKE]);
    else
        say_terrain(buf, kind);
    if (map_get(tx, ty) & MAP0_RIVER) {
        STRCAT(buf, "/");
        if (map_get(tx, ty) & MAP0_MAJOR_RIVER)
            say_string(buf, g_feature_words[FEATURE_MAJOR_RIVER]);
        else
            say_string(buf, g_feature_words[FEATURE_MINOR_RIVER]);
    }
    if ((feature_get(tx, ty) & (MAP1_SETTLEMENT | MAP1_ROAD)) || (tx == 2 && ty == 2)) {
        STRCAT(buf, "/");
        say_string(buf, g_misc_words[MISC_ROAD]);
    }
    if (feature_get(tx, ty) & MAP1_PLOWED) {
        STRCAT(buf, "/");
        say_string(buf, g_misc_words[MISC_PLOWED]);
    }
    cx = (col << 6) + 0x188;
    cy = (row << 6) - 0x1c;
    ext = MSGetTextLength_1010_e2d2(&g_number_font, buf) - 1;
    w = ext + 2;
    x = cx - (w >> 1);
    y = cy;
    x = minimax(x, 0x1a8, 0x268 - w);
    g_colony_popup = new GameWin;
    GameWin_Init(g_colony_popup, "", 0x201, g_desk_x + x, g_desk_y + y, w, 0x12,
                                     &g_game_palette, (Screen *)&g_colony_win_rec);
    GRPort_ColorSync(g_colony_popup, &g_game_palette);
    GRPort_FillColor_all(g_colony_popup, 0);
    set_on_keydown_1010_e36f(&(EventTable &)*g_colony_popup, LabelKeyClick);
    TextColor(0xff);
    GRPort_String(g_colony_popup, (unsigned *)&g_number_font, buf, port_width_1010_e3a8(g_colony_popup) / 2,
                         port_height_1010_e3c7(g_colony_popup) / 2, 0);
    copy_all_to_screen_1010_e3e6(g_colony_popup);
    show_window_1010_e404(g_colony_popup);
}
