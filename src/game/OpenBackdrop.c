/* OpenBackdrop: candidate for 1008:4a32 (257 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: OpenBackdrop
 *
 * OPENS THE FULL-SCREEN BACKDROP that the game's dialogs sit on, and does nothing when it is already
 * open. `CloseBackdrop` (1008:4b58) is the other half, and five functions call this one (eight call sites: end_of_game_check four times, GameBrains_DoEndofTurn, KeyEvents, MainWin_MainMenuProc and game_init once each).
 *
 * The window is an `EventWin` allocated with `new`, built at the desktop's size with style 0x801, given
 * a black background and the game palette, registered through `SetPopupParent` and shown.
 *
 * The early exit is a plain `return;` -- with no destructible local, both exits restore the exception
 * word through AX, so rule 41 does not apply here.
 *
 * RENAMED 2026-09-26 from open_overlay_window: the Mac build's OpenBackdrop(void), bound by call-sequence alignment (1.0 aligned gaps in GameBrains_DoEndofTurn leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.61x this one.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
extern EventWin *g_backdrop_win;            /* DGROUP:0xc6 */
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */

extern void MSHideMenuBar(void);

extern void EventWin_Init(char *w, char *name, int style, int x, int y,
                                       int width, int height, CTable *pal);
extern void SetPopupParent(void *parent);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void win_set_back_color_1008_d60e(Screen *w, int index);
extern "C" void win_show(Screen *w);

extern "C" void OpenBackdrop(void)
{
    RECT desk;

    if (g_backdrop_win != 0)
        return;
    {
        MSHideMenuBar();
        MSGetDesktopSize(&desk);
        g_backdrop_win = new EventWin;
        EventWin_Init((char *)g_backdrop_win, "", 0x801, 0, 0, rect_width_1008_d5cf(&desk),
                               rect_height_1008_d5ee(&desk), &g_game_palette);
        win_set_back_color_1008_d60e(g_backdrop_win, 0);
        SetPopupParent(g_backdrop_win);
        win_show(g_backdrop_win);
    }
}
