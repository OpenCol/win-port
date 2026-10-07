/* EuropeWin_DrawOutBound: candidate for 1048:978c (383 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawOutBound
 * name-source: sibling-build
 *
 * The left-hand twin of EuropeWin_DrawInbound: the 0x8b by 0x79 cell at (2, 0x102)
 * repainted, a heading from the text string the word at SEG20:0xdcda indexes, and every unit in
 * the nation's two pseudo-squares at index minus 0x10 and minus 0xc laid along one row, the
 * slot counter carried from the first chain into the second by `EuropeWin_DrawTransitUnit`. There is no
 * second line with the nation's name.
 *
 * RENAMED 2026-09-26 from europe_draw_left: the Mac build's EuropeWin::DrawInbound(short): this plus a refresh flag; its panel is headed MISC_EXPECTED_SOON -- the ships on their way IN to Europe. The width matches.
 *
 * RENAMED 2026-09-27 from EuropeWin_DrawInbound: CONST PROPOSAL, SETTLED AS A SWAP with 95c1; the evidence is in 95c1's row. 978c ('Expected soon', squares nation-0x10 and -0xc, x 3/0x47) matches Mac DrawOutBound: its callees (strings, String x1), its position (second), its call order in DrawTransit (second) and its unique constants (0x47, -0x10, -0xc).
 */

#include "colonization.h"

#include "w16types.h"
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

extern char *strings(int n);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

/* 5-byte NOP form, so extern (rule 47) */
extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cx, int cy);
extern void EuropeWin_DrawTransitUnit(EuropeWin *w, int u, int x, int count3, int extra,
                              int *slot, int colour);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);
extern "C" void copy_rect_to_screen(EuropeWin *win, RECT *r);

void EuropeWin_DrawOutBound(EuropeWin *w, int refresh)
{
    int slot;
    RECT r;
    char buf[0x50];
    register int u;
    register int x;

    set_europe(w->nation);
    EuropeWin_UpdateEurope(w, 2, 0x102, 0x8b, 0x79);
    STRCPY(buf, strings(g_misc_words[MISC_EXPECTED_SOON]));
    TextColor(0x77);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x47, w->rect.top + 0x10c, 0);
    x = 3;
    slot = 0;
    u = top_unit_in_square(g_cur_nation - 0x10, g_cur_nation - 0x10);
    while (u >= 0) {
        EuropeWin_DrawTransitUnit(w, u, x, 0xd, 1, &slot, -1);
        u = next_unit(u);
    }
    u = top_unit_in_square(g_cur_nation - 0xc, g_cur_nation - 0xc);
    while (u >= 0) {
        EuropeWin_DrawTransitUnit(w, u, x, 0xd, 1, &slot, -1);
        u = next_unit(u);
    }
    if (refresh != 0) {
        SetRect(&r, w->rect.left + 2, w->rect.top + 0x102, w->rect.left + 0x8d, w->rect.top + 0x17b);
        copy_rect_to_screen(w, &r);
    }
}
