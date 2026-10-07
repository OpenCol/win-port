/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of DrawIconRow, sprite_height_1010_e681, sprite_width_1010_e60f removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_DrawProduction: candidate for 1010:7edb (693 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawProduction
 * name-source: sibling-build
 *
 * The colony window's construction panel: rows of hammer sprites for the hammers already
 * laid in, the name of what is being built above them, and -- when the item also wants
 * tools -- a parenthesised "(needs N tools)" line coloured by whether the colony has them.
 *
 * The layout is derived, not fixed. One row holds `0xb9 / (sprite_width + 1)` hammers, and
 * the build needs `(cost - 1) / that + 1` rows; past four rows the rows are widened instead
 * (`percol` is recomputed from the capped row count) so a cathedral still fits the panel.
 * Rows step by HALF a sprite height, so they overlap.
 *
 * `min_int_1010_e5d0` splits the hammers over the rows: each row draws `percol` outlines
 * with the first `k` of them filled in, which is exactly `DrawIconRow`'s
 * n-versus-m pair. Sprite 0x37 is the hammer, and its width and height come from the
 * sprite record's embedded rect through the two -called accessors.
 *
 * `if (rows == 8) ;` IS IN THE SHIPPED CODE: a `jnz` over nothing, two bytes, testing a
 * count that the cap above has already forced to four or less. A debugging stub left in.
 *
 * The tools colour is a TERNARY and not an if/else: both arms load a byte into AL and the
 * widening `mov ah,0` with the store happens once at the join. Written as an if/else each
 * arm widens and stores for itself, which is five bytes longer (rule 54's shape test).
 *
 * `row` and `y` are the register locals -- SI then DI in declaration order (rule 53); the
 * other twelve and the 0x50-byte buffer live in the frame.
 *
 * RENAMED 2026-09-26 from colony_draw_hammers: the Mac build's ColonyWin::DrawMakeList(void): this only, the construction panel -- hammers laid in and the item being made -- which ColonyWin_DrawMiscellaneous paints in its build mode. The width matches.
 *
 * RENAMED 2026-09-27 from ColonyWin_DrawMakeList: DISAGREE, SETTLED AS A SWAP with 78cb. Mac DrawProduction(void), CODE 30 0x1086, 654 bytes. Its seq is set_colony, get_production_cost, get_production_name, DrawIconRow, ResetClip, TextColor, String, open_string, say_string, space_string, say_number, space_string, say_string, close_string, TextColor, String. 7edb calls set_colony, get_production_cost, get_production_name, DrawIconRow, GRPort_ResetClip, TextColor x2, GRPort_String x2, open_string, say_string x2, space_string x2, say_number and close_string. It draws what is being built, the hammer box. Layout: after DrawColonyUnits and before DrawMiscellaneous on both. Caller: DrawMiscellaneous on both. Arity: this. Size 654 against 693. Constants: rare 0x1ad, 0xae, 0xb9 and 0x134 are shared with Mac DrawProduction only (overlap 1.0).
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "records.h"

extern void set_colony(int colony);
extern int get_production_cost(int item, int *tools);
extern char *get_production_name(int item);
extern void GRPort_ResetClip(GRPort *port);

extern void open_string(char *s);
extern void space_string(char *buf);
extern void close_string(char *s);

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` */

void ColonyWin_DrawProduction(ColonyWin *w)
{
    int cost;
    int sw;
    int sh;
    int x;
    int per;
    int maxrows;
    int rows;
    int percol;
    int have;
    int filled;
    int tools;
    int colour;
    char *name;
    char buf[0x50];
    register int row;
    register int y;

    set_colony(w->colony);
    buf[0] = 0;
    cost = get_production_cost(g_colonies[w->colony].item, &tools);
    name = get_production_name(g_colonies[w->colony].item);
    sw = sprite_width_1010_e60f(&g_icon_sprites[0x37]);
    sh = sprite_height_1010_e681(&g_icon_sprites[0x37]);
    per = 0xb9 / (sw + 1);
    maxrows = 4;
    sh = sh >> 1;
    percol = per;
    rows = (cost - 1) / per + 1;
    if (rows > maxrows) {
        rows = maxrows;
        percol = (cost - 1) / rows + 1;
    }
    have = g_colonies[w->colony].hammers;
    if (rows == 8)
        ;
    x = 0x1ad;
    y = 0x134;
    for (row = 0; row < rows; row++) {
        filled = min_int_1010_e5d0(have, percol);
        DrawIconRow((GRPort *)w, 0x37, percol, filled, w->rect.left + x, w->rect.top + y, 0xae, 0, 0, 1);
        have -= filled;
        y += sh;
    }
    GRPort_ResetClip((GRPort *)w);
    if (name != 0)
        STRCAT(buf, name);
    TextColor(0x39);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x1fb, w->rect.top + 0x11a, 0);
    if (tools != 0) {
        buf[0] = 0;
        open_string(buf);
        say_string(buf, g_misc_words[MISC_REQUIRES]);
        space_string(buf);
        say_number(buf, tools);
        space_string(buf);
        say_string(buf, g_goods_name_word[GOODS_TOOLS]);
        close_string(buf);
        colour = g_colonies[w->colony].stock[GOODS_TOOLS] < tools ? g_text_colour : g_draw_color;
        TextColor(colour);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x1fb, w->rect.top + 0x175, 0);
    }
}
