/* ColonyWin_DrawColonyUnits: candidate for 1010:7b7e (861 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawColonyUnits
 * name-source: sibling-build
 *
 * Draws the colony window's unit strip and the line of text above it. The text is either the
 * name of what the colony is building -- when DGROUP:0xa08 is set -- or a fixed string from the
 * table; the strip is every unit standing on the colony's square, walked through the tile chain.
 *
 * THREE ROWS, AND THE FIRST IS NOT LIKE THE OTHER TWO. Row 0 holds five units at 0x22 apart,
 * drawn through `PieceDraw` at full size with a 0x20 box; rows 1 and 2 hold seventeen each
 * at 0xa apart, drawn as 3/8-scale PORTRAITS through `Sprite_Draw` with a 10 by 10 box (the 0xf and 0x11 put in `bw`/`bh` are read only by the row-0 box). Every
 * one of those seven numbers is re-assigned when the first row fills, which is why they are
 * variables and not constants.
 *
 * A fourth row would run off the panel, so the walk stops at row 3 -- units past that are not
 * drawn and not counted.
 *
 * `ten = 10` IS TESTED FOR ZERO immediately after being set, and the branch is dead. It is in
 * the shipped code, and it costs six bytes.
 *
 * The selection box is a plain `if/else` and not a ternary: the shipped stores the row's Y
 * straight into DI in both arms, where `y = row == 1 ? 0x13f : 0x12e;` computes into AX first
 * and moves. Same for the two `SetRect` shapes, which differ in both size and offset.
 *
 * `x` and `y` are the two `register` locals; the other twelve live in the frame, and `buf` is
 * 0x50 bytes of it.
 *
 * RENAMED 2026-09-26 from colony_draw_units: the Mac build's ColonyWin::DrawColonyUnits(void): this only, painting every unit standing on the colony's square.
 */

#include "colonization.h"

#include "w16types.h"
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

extern void GRPort_ResetClip(GRPort *port);
extern void set_colony(int colony);
extern char *get_production_name(int item);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);
extern char *strings(int n);
extern int top_unit_in_square(int x, int y);
extern void SetGlobalSpriteScale(int num, int den);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y, int span, int mode);
extern int UnitIcon(int u);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern int next_unit(int u);

void ColonyWin_DrawColonyUnits(ColonyWin *w)
{
    int u;
    int first;
    int x0;
    int col;
    int row;
    int n;
    int ten;
    int dy;
    int bw;
    int bh;
    int per_row;
    int dx;
    char *name;
    RECT box;
    RECT sr;
    char buf[0x50];
    register int x;
    register int y;

    GRPort_ResetClip((GRPort *)w);
    set_colony(w->colony);
    if (g_scout_visit != 0) {
        buf[0] = 0;
        name = get_production_name((int)g_colonies[w->colony].item);
        if (name != 0)
            STRCAT(buf, name);
        TextColor(0x39);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x1fb, w->rect.top + 0x118, 0);
    } else {
        TextColor(0x39);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, strings(g_cmisc_word[1]),
                             w->rect.left + 0x1fb, w->rect.top + 0x118, 0);
    }
    first = top_unit_in_square(g_colonies[w->colony].x, g_colonies[w->colony].y);
    x0 = 0x1aa;
    y = 0x15b;
    SetGlobalSpriteScale(1, 1);
    x = x0;
    col = 0;
    row = 0;
    n = 0;
    dx = 0x22;
    per_row = 5;
    bw = 0x20;
    bh = 0x20;
    dy = 0;
    u = first;
    while (u >= 0) {
        if (g_unit_types[g_units[u].type].holds > 0)
            goto next;
        if (row == 0) {
            PieceDraw((GRPort *)w, u, 0, w->rect.left + x + 0x12, w->rect.top + y, 0x20, 0x64);
        } else {
            SetGlobalSpriteScale(3, 8);
            Sprite_Draw(&sr, &g_icon_sprites[UnitIcon(u)], (GRPort *)w,
                        w->rect.left + x, w->rect.top + y);
            SetGlobalSpriteScale(1, 1);
        }
        if (g_tile_sel == n) {
            ten = 10;
            if (ten != 0 && g_scout_visit == 0) {
                TextColor(0xfa);
                if (row == 0)
                    SetRect(&box, x - 1, y - 1 - dy, x + bw, y + bh);
                else
                    SetRect(&box, x - 5, y, x + 5, y + 10);
                OffsetRect(&box, w->rect.left, w->rect.top);
                GRPort_DrawRect((GRPort *)w, &box);
            }
        }
        x += dx;
        col++;
        if (col >= per_row) {
            col = 0;
            row++;
            if (row >= 3)
                break;
            if (row == 1)
                y = 0x13f;
            else
                y = 0x12e;
            x = 0x1aa;
            per_row = 0x11;
            dx = 0xa;
            bw = 0xf;
            bh = 0x11;
            dy = 1;
        }
        n++;
next:
        u = next_unit(u);
    }
    SetGlobalSpriteScale(1, 1);
}
