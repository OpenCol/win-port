/* ColonyWin_DrawFort: candidate for 1010:9455 (472 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawFort
 * name-source: sibling-build
 *
 * Draws the colonists standing OUTSIDE the colony -- the ones on the colony's own map square,
 * walked through the tile's unit chain rather than through the colony record. It is
 * `ColonyWin_DrawIndividual` written against a different source of colonists: the same
 * `ComputeIconRow` spacing from the SEG21:0x38 width, the same portraits through
 * `PieceIconDraw`, and the same half-portrait-wide frame around the selected one.
 *
 * DGROUP:0x4bb2 CAPS THE ROW. The count is incremented for every colonist on the square, and
 * one past the cap the loop stops drawing -- but keeps counting, and keeps walking the chain,
 * so the cap trims the display and not the walk.
 *
 * `x` is an in-out PARAMETER: `ComputeIconRow` may move the start through the pointer, and each
 * drawn portrait advances it by the step. The caller's copy is what moves.
 *
 * The cap test is a `goto` and not an `if`: `JNL SHORT` over a `JMP NEAR` is rule 12's two-jump
 * form, where the colonist test above it is a direct `JZ NEAR` and so a real `if` block.
 *
 * RENAMED 2026-09-26 from colony_draw_outside: the Mac build's ColonyWin::DrawFort(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawABuilding; callee DrawRect; callee TextColor; callee next_unit; callee set_colony), the runner-up gets 4, and the arity agrees (4).
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
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

extern void set_colony(int colony);
extern int ComputeIconRow(int pic, int n, int m, int *px, int y,
                                    int w1, int w2, int *pa, int *pb, int style);
extern int top_unit_in_square(int x, int y);
extern int is_colonist(int u);
extern int guy_number_of_unit(int u);
extern int get_icon(int colonist);
extern void PieceIconDraw(void *dst, int icon, int x, int y, int flags);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern int next_unit(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int sprite_width_1010_e60f(void *sprite);

void ColonyWin_DrawFort(ColonyWin *w, int x, int y, int count)
{
    int step;
    int a;
    int slot;
    int colonist;
    int b;
    int sel;
    int n;
    RECT r;
    register int u;
    register int pic;

    n = 0;
    set_colony(w->colony);
    slot = 3;
    step = ComputeIconRow(0x67, count, count, &x, y,
                                g_slot_span[slot], g_slot_span[slot], &a, &b, 2);
    u = top_unit_in_square(g_colonies[w->colony].x, g_colonies[w->colony].y);
    while (u >= 0) {
        if (is_colonist(u) != 0) {
            n++;
            if (g_tile_colonists < n)
                goto next;
            colonist = guy_number_of_unit(u);
            pic = get_icon(colonist);
            PieceIconDraw(w, pic, w->rect.left + x, w->rect.top + y, 7);
            sel = g_selected_colonist;
            if (colonist == sel) {
                TextColor(g_text_colour);
                SetRect(&r, w->rect.left + x - (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1),
                        w->rect.top + y,
                        (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) + (w->rect.left + x),
                        w->rect.top + y + 0x20);
                GRPort_DrawRect((GRPort *)w, &r);
            }
            x += step;
        }
next:
        u = next_unit(u);
    }
}
