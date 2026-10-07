/* europe_label_cargo: candidate for 1048:3d72 (725 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_label_cargo
 * name-source: sibling-build
 *
 * The Europe market's goods popup, shown when `EuropeWin_EuropeMouseMarket` moves to a new column:
 * "Sugar (Bidding 3, Asking 5)" -- MISC words 125 and 124 around the market's selling and buying
 * prices -- or "Sugar (Boycott)" (MISC word 131) when the current nation has the goods boycotted. The
 * goods id is kept at DGROUP:0x1fd4, the column test `EuropeWin_EuropeMouseMarket` makes.
 *
 * It is the Europe screen's copy of `label_cargo`, the same popup code line for line
 * with the Europe module's own helpers: centred over the goods' 38-pixel column, 20 pixels above the
 * bottom of the Europe window at SEG20:0x2fd2, clamped to 0..0x262 minus its width and shifted by the
 * screen origin at SEG20:0x373c, with `LabelKeyProc` in handler slot 0x33. Bit 0 of
 * SEG20:0x7786 suppresses it, as it does the colony's.
 *
 * Rules 69 and 70 again, exactly as in the colony copy: the goods id is a `register` parameter, and
 * the popup's x is a plain local declared first so that it is promoted into DI. Unlike the colony
 * copy it takes one off the text extent before adding two.
 *
 * RENAMED 2026-09-26 from europe_goods_info: the Mac build's europe_label_cargo(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee ColorSync; callee TextColor; callee comma_string; callee minimax; callee say_number), the runner-up gets 6, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "colopt.h"
#include "misc.h"
typedef struct { short hfont, height; } Font;
#pragma option -a
#include "w16types.h"
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern GameWin *g_europe_popup;         /* DGROUP:0x1fd0 */

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } EuropeWin;

extern void open_string(char *s);
extern void close_string(char *s);
extern void space_string(char *buf);

extern void GameWin_Init(GameWin *o, char *name, int style,
                                                 int x, int y, int w, int h,
                                                 CTable *bits, Screen *parent);
extern void GRPort_ColorSync(GRPort *port, CTable *page);
extern void GRPort_FillColor_all(GRPort *p, int color);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
extern void LabelKeyProc(int key);
extern int boycott(int bit);
extern int europe_bid_price(int g);
extern int europe_ask_price(int g);
extern void comma_string(char *buf);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void europe_remove_label(void);
extern "C" int MSGetTextLength_1048_dc4e(void *font, const char *s);
extern "C" void set_on_keydown_1048_dceb(EventTable *table, void *h);

extern "C" void show_window_1048_dd42(void *o);

extern "C" void europe_label_cargo(register int goods)
{
    int x;
    int y;
    int w;
    int base;
    int cx;
    int ext;
    char buf[0x50];

    europe_remove_label();
    if (g_game.colony_options & COLOPT_NO_TERRAIN_LABELS)
        return;
    g_europe_popup_id = goods;
    buf[0] = 0;
    say_string(buf, g_goods_name_word[goods]);
    space_string(buf);
    open_string(buf);
    if (boycott(goods) != 0)
        say_string(buf, g_misc_words[MISC_BOYCOTT]);
    else {
        say_string(buf, g_misc_words[MISC_BIDDING]);
        space_string(buf);
        say_number(buf, europe_bid_price(goods));
        comma_string(buf);
        say_string(buf, g_misc_words[MISC_ASKING]);
        space_string(buf);
        say_number(buf, europe_ask_price(goods));
    }
    close_string(buf);
    base = 1;
    cx = goods * 0x26 + base + 0x13;
    ext = MSGetTextLength_1048_dc4e(&g_number_font, buf) - 1;
    w = ext + 2;
    x = cx - (w >> 1);
    y = port_height_1048_d7c5(&g_europe_win) - 0x14;
    x = minimax(x, 0, 0x262 - w) + g_europe_x0;
    g_europe_popup = new GameWin;
    GameWin_Init(g_europe_popup, "", 0x201, x, y, w, 0x12,
                                     &g_game_palette, (Screen *)&g_europe_win_dlg);
    GRPort_ColorSync(g_europe_popup, &g_game_palette);
    GRPort_FillColor_all(g_europe_popup, 0);
    set_on_keydown_1048_dceb((EventTable *)((char *)g_europe_popup + 0x33), LabelKeyProc);
    TextColor(0xff);
    GRPort_String(g_europe_popup, (unsigned *)&g_number_font, buf, port_width_1048_d4f6(g_europe_popup) / 2,
                         port_height_1048_d7c5(g_europe_popup) / 2, 0);
    copy_all_to_screen_1048_dd24(g_europe_popup);
    show_window_1048_dd42(g_europe_popup);
}
