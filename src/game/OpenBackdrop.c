/* PORT: this file's own declarations of SetPopupParent removed: the header declares it from the definition (W1 step 3). */
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
/* PORT: this file's own Screen, EventTable, EventWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


extern EventWin *g_backdrop_win;            /* DGROUP:0xc6 */
   /* 0x41e bytes: the palette, its handle and the cycling state */

extern void MSHideMenuBar(void);

extern void EventWin_Init(char *w, char *name, int style, int x, int y,
                                       int width, int height, CTable *pal);

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
