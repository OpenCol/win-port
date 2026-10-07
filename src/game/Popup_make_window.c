/* PORT: this file's own declarations of GameWin_Init, key_click removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 76, 77 of the imported file. */
/* Popup_make_window: candidate for 1050:3364 (395 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup_make_window
 * name-source: sibling-build
 *
 * BUILDS THE POPUP'S WINDOW, called by `Popup_draw`. It answers 0 when the popup already
 * has one or the allocation fails, and 1 once the window exists.
 *
 * `new GameWin` is 0xc4 bytes; the `SUB DWORD SS:[0x10],6` after its constructor is rule 49's
 * headcount, and it only comes out as 6 with the whole class tree written down -- GRPort plus
 * DialogWin over EventWin over Screen and EventTable. The window is built over the object at
 * DGROUP:0x24fc with the game palette, at the popup's own rectangle at +0x334, and titled from the
 * popup's +0x362 or, when that is null, an empty local string. It is centred only on a desktop bigger
 * than 640x480. The list's key handler goes on slot 0x33 and `port_update` on +0xc0.
 *
 * The empty title is a TWO-character local: a single `char` lands at an odd frame slot and puts
 * everything below it one byte out.
 *
 * RENAMED 2026-09-26 from popup_make_window: the Mac build's Popup::make_window(void): this only, building the popup's window. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


class DlgControl;


/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

   /* 0x41e bytes: the palette, its handle and the cycling state */

extern void port_update(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void win_center_1050_851f(DialogWin *w);
extern "C" void *set_on_keydown_after_1050_8544(EventTable *table, void *h);
extern "C" void set_relayout_handler_1050_857d(GameWin *w, void *h);

extern "C" int Popup_make_window(Popup *popup)
{
    char *title;
    char empty[2];
    RECT desk;

    if (popup->win != 0)
        return 0;
    popup->win = new GameWin;
    if (popup->win == 0)
        return 0;
    empty[0] = 0;
    title = popup->title;
    if (title == 0)
        title = empty;
    MSGetDesktopSize(&desk);
    GameWin_Init(popup->win, title, 0xc01, popup->win_x, popup->win_y, popup->win_w, popup->win_h,
                                     &g_game_palette, (Screen *)g_popup_owner);
    if (rect_width_1050_830c(&desk) > 0x280 || rect_height_1050_832b(&desk) > 0x1e0)
        win_center_1050_851f((DialogWin *)((char *)popup->win + 0x26));
    set_on_keydown_after_1050_8544((EventTable *)((char *)popup->win + 0x33), (void*)(key_click));
    set_relayout_handler_1050_857d(popup->win, (void*)(port_update));
    return 1;
}
