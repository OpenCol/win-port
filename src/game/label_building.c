/* label_building: candidate for 1010:374e (668 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: label_building
 * name-source: sibling-build
 *
 * The name popup for a building site in the colony window, shown by the buildings panel
 * (`ColonyWin_ColonyMouseBuildings`) in hover mode -- the buildings' counterpart of
 * `label_map`, and the same popup code: `new GameWin`, 0x12 tall, text drawn
 * shadowed in colour 0xff, `LabelKeyClick` in handler slot 0x32, kept at DGROUP:0x9ec.
 * Bit 1 of SEG20:0x7786 suppresses it, as bit 0 suppresses the square description.
 *
 * The text is the building's name (the word at +0 of its 12-byte record at SEG20:0xd422) when the
 * colony has it; a site kind 3 appends " (Outside Colony)", MISC word 123. A site with no building
 * at all reads "Trees", MISC word 122 -- the empty lots of the colony view are drawn as woods.
 * The popup is centred on the site horizontally, clamped to 0..0x190 minus its width, and for
 * kinds 2 and 4 and the empty lot centred vertically on the site as well.
 *
 * RULE 69 CAME FROM THIS FUNCTION. The building and the kind are `register` PARAMETERS: their loads
 * precede the `__InitExceptBlock` call, where register locals initialised from the parameters load
 * after it.
 *
 * RENAMED 2026-09-26 from colony_building_info: the Mac build's label_building(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee ColorSync; callee TextColor; callee has_building; callee minimax), the runner-up gets 3, and the arity agrees (4). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
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
extern GameWin *g_colony_popup;         /* DGROUP:0x9ec */

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */

extern int has_building(int bit);
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

extern "C" void label_building(register int building, int bx0, int by0, register int kind)
{
    int kw;
    int kh;
    int w;
    int x;
    int y;
    int ext;
    int text_h;
    char buf[0x50];

    if (g_game.colony_options & COLOPT_NO_BUILDING_LABELS)
        return;
    label_remove_label();
    g_labelled_building = building;
    kw = g_kind_w[kind];
    kh = g_kind_h[kind];
    buf[0] = 0;
    if (building >= 0) {
        if (has_building(building) != 0)
            say_string(buf, g_buildings[building].name);
        if (kind == 3) {
            space_string(buf);
            say_string(buf, g_misc_words[MISC_OUTSIDE_COLONY]);
        }
    } else
        say_string(buf, g_misc_words[MISC_TREES]);
    ext = MSGetTextLength_1010_e2d2(&g_number_font, buf) - 1;
    w = ext + 2;
    text_h = 7;
    x = (kw >> 1) + bx0 - (w >> 1);
    y = by0;
    if (kind == 2 || kind == 4 || building < 0)
        y += (kh >> 1) - (text_h >> 1);
    x = minimax(x, 0, 0x190 - w);
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
