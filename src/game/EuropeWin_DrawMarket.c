/* EuropeWin_DrawMarket: candidate for 1048:8fa0 (524 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawMarket
 * name-source: sibling-build
 *
 * Draws the price strip along the bottom of the Europe screen: sixteen goods, 0x26 apart,
 * each with its icon and a "sell/buy" price pair under it, and a boycott mark over any the
 * current nation is barred from trading.
 *
 * THE SPRITES ARE DRAWN AT SEVEN-EIGHTHS SCALE, set once before the loop and put back at the
 * end. Two goods do not take the scaling: 0xa and 0xf -- cigars and muskets -- switch to 1:1 for
 * their own icon, draw it two pixels higher, and switch back. Their sprites must be a
 * different size from the other fourteen.
 *
 * The price text is built by hand rather than formatted: `LTOA1` of the sell price, the
 * literal "/", `LTOA1` of the buy price, all concatenated into an 0x50-byte buffer. The
 * separator is a one-character string in the data segment, so the corpus's DGROUP:0x225b.
 *
 * The refresh argument decides whether the strip is pushed to the screen: the rectangle it
 * copies is 0x262 wide from the window's origin at +0x196 down to +0x1c9, which is the strip
 * and nothing else.
 *
 * `x` is the register local that walks the strip and `i` the counter; `cx` is a frame copy of
 * `x` taken at the top of each pass, which is what the sprite and the text both use -- the
 * boycott mark uses `x` itself, and the two are the same value.
 *
 * RENAMED 2026-09-26 from europe_draw_market: the Mac build's EuropeWin::DrawMarket(short): this plus a refresh flag, painting the market strip.
 */

#include "colonization.h"

#include "w16types.h"
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
class EuropeWin : public ColChild { public: short nation; SRegion region; SButton recruit; SButton train; SButton purchase; SImageButton image; GRPort port; EuropeWin(); ~EuropeWin(); };
typedef struct { short hfont, height; } Font;
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "icons.h"

extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cx, int cy);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);
extern "C" int europe_bid_price(int g);
extern "C" int europe_ask_price(int g);
extern "C" int boycott(int bit);
extern "C" void copy_rect_to_screen(EuropeWin *win, RECT *r);

void EuropeWin_DrawMarket(EuropeWin *w, int refresh)
{
    int y;
    int cx;
    RECT r;
    RECT t1;
    RECT t2;
    RECT t3;
    char num[0x14];
    char buf[0x50];
    register int i;
    register int x;

    set_europe(w->nation);
    EuropeWin_UpdateEurope(w, 0, 0x198, 0x262, 0x31);
    x = w->rect.left + 0x14;
    y = w->rect.top + 0x199;
    SetGlobalSpriteScale(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        cx = x;
        if (i == 0xa || i == 0xf) {
            SetGlobalSpriteScale(1, 1);
            Sprite_Draw(&t1, &g_icon_sprites[ICON_GOODS + i], (GRPort *)w, cx, y - 2);
            SetGlobalSpriteScale(7, 8);
        } else {
            Sprite_Draw(&t2, &g_icon_sprites[ICON_GOODS + i], (GRPort *)w, cx, y);
        }
        buf[0] = 0;
        LTOA1(europe_bid_price(i), num, 10);
        STRCAT(buf, num);
        STRCAT(buf, "/");
        LTOA1(europe_ask_price(i), num, 10);
        STRCAT(buf, num);
        TextColor(0);
        GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, cx, y + 0x24, 0);
        if (boycott(i) != 0)
            Sprite_Draw(&t3, &g_icon_sprites[ICON_BOYCOTT], (GRPort *)w, x, y);
        x += 0x26;
    }
    if (refresh != 0) {
        SetRect(&r, w->rect.left, w->rect.top + 0x196, w->rect.left + 0x262, w->rect.top + 0x1c9);
        copy_rect_to_screen(w, &r);
    }
    SetGlobalSpriteScale(1, 1);
}
