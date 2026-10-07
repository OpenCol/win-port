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
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class EuropeWin : public ColChild { public: short nation; SRegion region; SButton recruit; SButton train; SButton purchase; SImageButton image; GRPort port; EuropeWin(); ~EuropeWin(); };
typedef struct { short hfont, height; } Font;
#include "records.h"

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern int g_ships_in_port;                    /* DGROUP:0x1fca, ships in port */

extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cw, int ch);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
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
extern "C" void copy_rect_to_screen(EuropeWin *win, RECT *r);

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
