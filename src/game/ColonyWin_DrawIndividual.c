/* ColonyWin_DrawIndividual: candidate for 1010:92c0 (405 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawIndividual
 * name-source: sibling-build
 *
 * Draws the colonists holding `job` in a row at (x, y) of the colony
 * window: ComputeIconRow lays the row out (count from job_count,
 * the SEG21:0x38 width for `slot`) and returns the step; each colonist's
 * portrait goes through PieceIconDraw, and the selected one
 * (DGROUP:0x4bbc) gets a frame half a portrait wide either side.  The
 * portrait-width reader is -called and defined in-file as a stub.
 *
 * `g_slot_span` (SEG21:0x38) is the pixel width a slot's row of portraits gets. It is passed
 * to `ComputeIconRow` as BOTH `span` and `span2`, which is how that function is told the row and
 * the centring width are the same, and `ColonyWin_ColonyMouseBuildings` passes it twice as well when
 * it hit-tests the same row.
 *
 * RENAMED 2026-09-26 from colony_draw_job_row: the Mac build's ColonyWin::DrawIndividual(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawABuilding; callee DrawRect; callee TextColor; callee get_icon; callee get_job), the runner-up gets 4, and the arity agrees (5).
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

extern int g_stub_a;
extern void set_colony(int colony);
extern int job_count(int job);
extern int ComputeIconRow(int pic, int n, int m, int *px, int y, int w1, int w2, int *pa, int *pb, int style);
extern int get_job(int colonist);
extern int get_icon(int colonist);
extern void PieceIconDraw(void *dst, int icon, int x, int y, int flags);

extern void GRPort_DrawRect(GRPort *w, RECT *r);

extern "C" int sprite_width_1010_e60f(void *sprite);

void ColonyWin_DrawIndividual(ColonyWin *w, int job, int x, int y, int slot)
{
    register int i;
    register int pic;
    int step;
    int a;
    int n;
    int sel;
    int shift;
    RECT r;

    set_colony(w->colony);
    n = job_count(job);
    step = ComputeIconRow(job + 0x52, n, n, &x, y, g_slot_span[slot], g_slot_span[slot], &a, &shift, 2);
    for (i = 0; i < g_colonies[w->colony].pop; i++) {
        if (get_job(i) == job) {
            pic = get_icon(i);
            PieceIconDraw(w, pic, w->rect.left + x, w->rect.top + y + 1, 7);
            sel = g_selected_colonist;
            if (i == sel) {
                TextColor(g_text_colour);
                SetRect(&r, w->rect.left + x - (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) - 4, w->rect.top + y,
                        (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) + (w->rect.left + x) + 1, w->rect.top + y + 0x20);
                GRPort_DrawRect((GRPort *)w, &r);
            }
            x += step;
        }
    }
}
