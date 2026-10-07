/* ColonyWin_DrawMarket: candidate for 1010:8ef4 (972 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawMarket
 * name-source: sibling-build
 *
 * Draws the colony window's warehouse strip: the sixteen goods, 38 pixels apart along the bottom, each
 * as its icon at 7/8 size (cigars and muskets at full size, two pixels higher) with the colony's stock
 * written under it. The strip `ColonyWin_ColonyMouseMarket` and the goods popup
 * (`label_cargo`) work on.
 *
 * A stock of 100 or more is written in two parts, hundreds and the two-digit remainder, in two colours
 * and centred on the column; a stock below 100 is written whole in the second colour. The colours are
 * 0x67 and 0x84, with two exceptions:
 *
 *   - above the warehouse's capacity (`get_warehouse`), for any goods but food, both parts
 *     turn to 0x3d / 0x3c;
 *   - when the colony has building 0x12 -- BUILDING18, "CUSTOM HOUSE" -- and the goods' bit is set in
 *     the bitmap `custom_sell` reads, the second part is 0xfa. That reads as the goods the
 *     custom house exports.
 *
 * The first thing the loop does with the text buffer is dead: the stock is formatted into `num` and
 * appended to an emptied `buf`, and both are overwritten before either is drawn. The redraw rectangle
 * covers the strip, and the scale is reset to 1:1 on the way out.
 *
 * RENAMED 2026-09-26 from colony_draw_warehouse: the Mac build's ColonyWin::DrawMarket(short): this plus a redraw flag, painting the sixteen-goods strip -- what EuropeWin_DrawMarket paints on the Europe screen.
 */

#include "colonization.h"

#include "w16types.h"
#include "building.h"
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
#include "records.h"

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "icons.h"

extern void set_colony(int colony);
extern void ColonyWin_UpdateColony(void *o, int x, int y, int w, int h);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern int has_building(int bit);
extern int custom_sell(int n);
extern int get_warehouse(void);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int MSGetTextLength_1010_e2d2(void *font, const char *s);
extern "C" int font_height_1010_e73d(void *font);
extern "C" void copy_rect_to_screen_1010_e6a4(void *w, RECT *r);

void ColonyWin_DrawMarket(ColonyWin *w, int redraw)
{
    int colx;
    int iy;
    int ty;
    int x;
    int ty2;
    int hundreds;
    int rem;
    int c1;
    int c2;
    RECT r;
    RECT s1;
    RECT s2;
    char num[20];
    char buf[0x50];
    register int i;
    register int tx;

    set_colony(w->colony);
    ColonyWin_UpdateColony(w, 0, 0x186, 0x262, 0x32);
    colx = w->rect.left + 0x14;
    iy = w->rect.top + 0x18b;
    SetGlobalSpriteScale(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        x = colx;
        if (i == GOODS_CIGARS || i == GOODS_MUSKETS) {
            SetGlobalSpriteScale(1, 1);
            Sprite_Draw(&s1, &g_icon_sprites[ICON_GOODS + i], (GRPort *)w, x, iy - 2);
            SetGlobalSpriteScale(7, 8);
        } else
            Sprite_Draw(&s2, &g_icon_sprites[ICON_GOODS + i], (GRPort *)w, x, iy);
        SetGlobalSpriteScale(7, 8);
        buf[0] = 0;
        LTOA1(g_colonies[w->colony].stock[i], num, 10);
        STRCAT(buf, num);
        ty = w->rect.top + 0x1ad;
        tx = colx;
        hundreds = g_colonies[w->colony].stock[i] / 100;
        rem = g_colonies[w->colony].stock[i] % 100;
        c1 = 0x67;
        c2 = 0x84;
        if (has_building(BUILDING_CUSTOM_HOUSE) != 0 && custom_sell(i) != 0)
            c2 = 0xfa;
        if (i != GOODS_FOOD && get_warehouse() < g_colonies[w->colony].stock[i]) {
            c1 = 0x3d;
            c2 = 0x3c;
        }
        if (hundreds > 0) {
            LTOA1(hundreds, buf, 10);
            STRCPY(num, buf);
            append_number_padded(num, rem, 2);
            tx -= MSGetTextLength_1010_e2d2(&g_font_tiny, num) >> 1;
            TextColor(c1);
            buf[0] = 0;
            LTOA1(hundreds, buf, 10);
            ty2 = ty - (font_height_1010_e73d(&g_font_tiny) >> 1);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, tx, ty2, 5);
            tx += MSGetTextLength_1010_e2d2(&g_font_tiny, buf);
            buf[0] = 0;
            append_number_padded(buf, rem, 2);
            TextColor(c2);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, tx, ty2, 5);
            tx += MSGetTextLength_1010_e2d2(&g_font_tiny, buf) + 2;
        } else {
            LTOA1(rem, buf, 10);
            TextColor(c2);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, tx + 1, ty, 0);
            tx += MSGetTextLength_1010_e2d2(&g_font_tiny, buf) + 2;
        }
        colx += 0x26;
    }
    if (redraw != 0) {
        SetRect(&r, w->rect.left, w->rect.top + 0x186, w->rect.left + 0x262, w->rect.top + 0x1b8);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
    SetGlobalSpriteScale(1, 1);
}
