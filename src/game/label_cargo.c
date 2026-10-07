/* label_cargo: candidate for 1010:3a0e (641 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: label_cargo
 * name-source: sibling-build
 *
 * The goods popup of the colony window's warehouse strip: "Sugar (123 Tons)" -- the goods' name
 * (SEG20:0xd86e), the colony's stock of it (the word array at Colony+0x9a), and MISC word 58,
 * "Tons". The third of the colony window's hover popups with `label_map` and
 * `label_building`, and the same popup code; this one is centred over the goods'
 * 38-pixel column, sits 0x15 above the bottom of the colony window at SEG20:0x25be, and is clamped
 * to the window's width. Bit 0 of SEG20:0x7786 suppresses it, the same bit as the square
 * description; the goods id is kept at DGROUP:0x9f2.
 *
 * TWO REGISTER RULES DECIDE ITS FIRST BYTES (rules 69 and 70). `goods` is a `register` parameter:
 * its load precedes `__InitExceptBlock`. The popup's x takes DI, and it is NOT declared `register`
 * -- a `register` local would outrank the parameter for SI (rule 53). It is a PLAIN local that the
 * allocator promotes, and it is promoted only when declared before `w`; declared after, `w` is
 * promoted in its place and the frame offsets shift.
 *
 * The text width here is not reduced by one, where the other two popups subtract 1 from the font
 * extent before adding 2.
 *
 * RENAMED 2026-09-26 from colony_goods_info: the Mac build's label_cargo(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee ColorSync; callee TextColor; callee minimax), the runner-up gets 2, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "colopt.h"
#include "misc.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable, ColonyWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a
#include "w16types.h"

#pragma option -a-


class DlgControl;


#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern GameWin *g_colony_popup;         /* DGROUP:0x9ec */

   /* 0x41e bytes: the palette, its handle and the cycling state */



extern void open_string(char *s);
extern void close_string(char *s);
extern void space_string(char *buf);

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

extern "C" void label_cargo(register int goods)
{
    int x;
    int y;
    int w;
    int base;
    int cx;
    int ext;
    char buf[0x50];

    label_remove_label();
    if (g_game.colony_options & COLOPT_NO_TERRAIN_LABELS)
        return;
    g_labelled_goods = goods;
    buf[0] = 0;
    say_string(buf, g_goods_name_word[goods]);
    space_string(buf);
    open_string(buf);
    say_number(buf, g_cur_colony_rec->stock[goods]);
    space_string(buf);
    say_string(buf, g_misc_words[MISC_TONS]);
    close_string(buf);
    base = g_desk_x + 0x14;
    cx = goods * 0x26 + base + 0x13;
    ext = MSGetTextLength_1010_e2d2(&g_number_font, buf);
    w = ext + 2;
    x = cx - (w >> 1);
    y = port_height_1010_e3c7(&g_colony_window) - 0x15;
    x = minimax(x, g_desk_x, g_desk_x + 0x262 - w);
    g_colony_popup = new GameWin;
    GameWin_Init(g_colony_popup, "", 0x201, x, y, w, 0x12,
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
