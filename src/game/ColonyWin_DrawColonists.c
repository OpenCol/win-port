/* ColonyWin_DrawColonists: candidate for 1010:962d (2009 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawColonists
 * name-source: sibling-build
 * size-source: 2009, the census figure; grading at 2013 or 2018 differs, so nothing follows the
 * epilogue.
 *
 * PANEL 3 OF THE COLONY WINDOW: the row of colonists standing outside the buildings, the food bar
 * under it, and the Sons of Liberty / Tory percentages either side. `ColonyWin_DrawColony`
 * draws it with `redraw` 0 and `colony_change_mode` with 1, which also copies the
 * panel to the screen.
 *
 * THE ROW is the colony's own population plus DGROUP:0x4bb2, the units standing on the square that
 * the window shows outside it. Each portrait comes from the 32-byte sprite table at SEG20:0x6246
 * through `get_icon`; portrait 0x5a counts half width, which is how the units
 * outside pack tighter than the colonists. The row is fitted first: the gap DGROUP:0x4bc0 starts at
 * 2 and is decremented until the whole row plus the eight-pixel gap before the outside units fits
 * 0xda pixels. Even so a portrait can end up narrower than one pixel, and `budget` carries that
 * shortfall forward, taking a pixel off each later portrait until it is paid back.
 *
 * The selected colonist (DGROUP:0x4bbc) is boxed in black and the one under the pointer
 * (DGROUP:0x4bbe) in the highlight colour at DGROUP:0x337f -- neither while DGROUP:0xa08 is set,
 * which is the flag the drag code uses to suppress the boxes.
 *
 * THE FOOD BAR is built out of `AddMultiIcon` segments and drawn by
 * `DrawMultiIconRow`. Without a food shortfall (SEG21:0x18c) it is two segments: what is
 * eaten, and the surplus, each with the part the fishermen brought marked off. With a shortfall the
 * whole production is one segment and the shortfall a second in a different style. The
 * crosses (SEG21:0x144) and bells (0x146) add their own segments; there is no horse segment.
 *
 * THE PERCENTAGES: `colony_rebel` gives the rebel share; the Tory head count is
 * `(pop * (100 - sol) + 50) / 100` and the rebels are the rest. Each side is drawn as "NN% (n)" in
 * a colour that warns as the Tories grow: green until they reach `10 - difficulty`, yellow past
 * that, red at twice it -- except that a colony of a computer or native nation uses 0x32 as the
 * threshold, which no colony ever reaches, so their bars stay green.
 *
 * WHAT THE BYTES SETTLED: the shortfall segment is `g_prod[0] - g_fish_food`, not the other way about --
 * Borland loads the byte that needs promoting first and subtracts the other way round, so the
 * operand order has to be read off the SUB and not the loads. The half-width portrait divides with
 * `/ 2` (CWD/SUB/SAR) in the measuring pass but shifts with `>> 1` when the sprite is placed. `x`
 * and `step` are the two `register` locals, SI and DI in declaration order (rule 53).
 *
 * RENAMED 2026-09-26 from colony_draw_colonists: the Mac build's ColonyWin::DrawColonists(short): this plus a redraw flag, painting the colonists outside the buildings, the food bar and the Sons of Liberty figures.
 */

#include "colonization.h"

#include "w16types.h"
#include "nations.h"
#include "control.h"
#include "goods.h"
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
class ColonyWin : public ColChild { public: SRegion region; GRPort back; GRPort terrain; SImageButton ib[3]; SButton b[2]; SImageButton ib2; char buy_shown; char change_shown; short panel; short cx; short cy; int timer; int colony; ColonyWin(); ~ColonyWin(); };
typedef struct { short hfont, height; } Font;
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

#include "icons.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern int g_consumption[];                    /* SEG21:0x164 */
extern int g_deficit[];                  /* SEG21:0x18c */

extern void set_colony(int colony);
extern void ColonyWin_UpdateColony(void *o, int x, int y, int w, int h);
extern int get_icon(int colonist);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port,
                                  int px, int py, unsigned char mode);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern void ResetMultiIcon(void);
extern void AddMultiIcon(int kind, int count, int part);
extern void DrawMultiIconRow(GRPort *w, int x, int y, int width, int style);
extern int colony_rebel(void);

extern void percent_string(char *buf);
extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y,
                                     int size);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int sprite_width_1010_e60f(Sprite *sprite);

extern "C" int MSGetTextLength_1010_e2d2(void *font, const char *s);
extern "C" void copy_rect_to_screen_1010_e6a4(void *w, RECT *r);

void ColonyWin_DrawColonists(ColonyWin *w, int redraw)
{
    int i;
    int y;
    int tx;
    int pic;
    int total;
    int pop;
    int extra;
    int fish_eaten;
    int fish_surplus;
    int count;
    RECT r;
    int rebels;
    int tories;
    int budget;
    int tory_pct;
    int sol;
    int colour;
    int limit;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    RECT r6;
    RECT r7;
    char buf[0x50];
    register int x;
    register int step;

    set_colony(w->colony);
    ColonyWin_UpdateColony(w, 0, 0x110, 0xf0, 0x73);
    pop = (int)g_colonies[w->colony].pop + g_tile_colonists;
    x = w->rect.left + 1;
    y = w->rect.top + 0x139;
    total = 0;
    for (i = 0; i < pop; i++) {
        pic = get_icon(i);
        if (pic != 0x5a)
            total += sprite_width_1010_e60f(&g_icon_sprites[pic]);
        else
            total += sprite_width_1010_e60f(&g_icon_sprites[pic]) / 2;
    }
    g_portrait_gap = 2;
    extra = 8;
    if (g_tile_colonists == 0)
        extra = 0;
    while ((int)g_portrait_gap * (pop - 1) + (total + extra) >= 0xda)
        g_portrait_gap--;
    x++;
    y--;
    budget = 0;
    for (i = 0; i < pop; i++) {
        pic = get_icon(i);
        Sprite_Draw(&r2, &g_icon_sprites[pic], (GRPort *)w,
                    (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) + x, y);
        if (g_scout_visit == 0) {
            if (g_selected_colonist == i && (g_panel_mode != 1 || g_panel_toggle != 0) && w->panel != 0) {
                SetRect(&r, x, y, sprite_width_1010_e60f(&g_icon_sprites[pic]) + x, y + 0x20);
                TextColor(0);
                GRPort_DrawRect((GRPort *)w, &r);
            }
            if (g_row_colonist == i && g_panel_mode == 1) {
                TextColor(g_text_colour);
                SetRect(&r, x, y, sprite_width_1010_e60f(&g_icon_sprites[pic]) + x, y + 0x20);
                GRPort_DrawRect((GRPort *)w, &r);
            }
        }
        if (pic != 0x5a)
            step = sprite_width_1010_e60f(&g_icon_sprites[pic]) + (int)g_portrait_gap;
        else
            step = sprite_width_1010_e60f(&g_icon_sprites[pic]) / 2 + (int)g_portrait_gap;
        if (step < 1)
            budget += 1 - step;
        step = max_int_1010_e632(1, step);
        while (step > 1 && budget > 0) {
            step--;
            budget--;
        }
        x += step;
        if ((int)g_colonies[w->colony].pop - 1 == i)
            x += 8;
    }
    g_draw_labels = g_show_icon_counts;
    y = w->rect.top + 0x161;
    ResetMultiIcon();
    if (g_deficit[GOODS_FOOD] == 0) {
        fish_eaten = min_int_1010_e5d0(g_fish_food, g_consumption[GOODS_FOOD]);
        fish_surplus = max_int_1010_e632(0, g_fish_food - fish_eaten);
        AddMultiIcon(0x4017, g_consumption[GOODS_FOOD], g_consumption[GOODS_FOOD] - fish_eaten);
        count = g_prod[GOODS_FOOD] - g_consumption[GOODS_FOOD];
        AddMultiIcon(0x4017, count, count - fish_surplus);
    } else {
        AddMultiIcon(0x4017, g_prod[GOODS_FOOD], g_prod[GOODS_FOOD] - g_fish_food);
        if (g_deficit[GOODS_FOOD] != 0)
            AddMultiIcon(0x8017, g_deficit[GOODS_FOOD], 0);
    }
    if (g_prod[PROD_CROSSES] != 0)
        AddMultiIcon(0x39, g_prod[PROD_CROSSES], 0);
    if (g_prod[PROD_BELLS] != 0)
        AddMultiIcon(0x3f, g_prod[PROD_BELLS], 0);
    DrawMultiIconRow((GRPort *)w, w->rect.left + 0x12, y, 0xe4, 8);
    sol = colony_rebel();
    tory_pct = 100 - sol;
    tories = ((int)g_colonies[w->colony].pop * (100 - sol) + 0x32) / 100;
    rebels = (int)g_colonies[w->colony].pop - tories;
    y = w->rect.top + 0x10d;
    limit = 10 - g_game.difficulty;
    if (g_colonies[w->colony].nation >= NATION_EUROPEAN_COUNT
        || g_nation_text[g_colonies[w->colony].nation].control != CONTROL_HUMAN)
        limit = 0x32;
    colour = 0xe;
    if (tories >= limit)
        colour = 4;
    if (limit * 2 <= tories)
        colour = 0xc;
    if (g_scout_visit == 0) {
        if (sol != 0) {
            x = w->rect.left + 0xe;
            Sprite_DrawMono(&r3, &g_icon_sprites[ICON_SOL_FLAG], (GRPort *)w, x - 2, y + 2, 0xf);
            Sprite_Draw(&r4, &g_icon_sprites[ICON_SOL_FLAG], (GRPort *)w, x, y);
            x += 0xe;
            LTOA1(sol, buf, 10);
            percent_string(buf);
            space_string(buf);
            open_string(buf);
            say_number(buf, rebels);
            close_string(buf);
            TextColor(colour);
            GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, x, y + 0xa, 5);
        }
        if (tory_pct != 0) {
            LTOA1(tory_pct, buf, 10);
            percent_string(buf);
            space_string(buf);
            open_string(buf);
            say_number(buf, tories);
            close_string(buf);
            x = w->rect.left + 0xef;
            x -= 0xd;
            tx = x - MSGetTextLength_1010_e2d2(&g_report_font_small, buf) - 0xe;
            TextColor(colour);
            GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, tx, y + 0xa, 5);
            Sprite_DrawMono(&r5, &g_icon_sprites[ICON_TORY_FLAG], (GRPort *)w, x - 2, y + 2, 0xf);
            Sprite_Draw(&r6, &g_icon_sprites[ICON_TORY_FLAG], (GRPort *)w, x, y);
        }
    }
    g_draw_labels = 0;
    if (redraw) {
        SetRect(&r7, w->rect.left, w->rect.top + 0x110, w->rect.left + 0xf0, w->rect.top + 0x183);
        copy_rect_to_screen_1010_e6a4(w, &r7);
    }
}
