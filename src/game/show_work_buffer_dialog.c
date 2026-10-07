/* PORT: this file's own declarations of GameWin_Init, port_height_1048_d86c, port_width_1048_d853, set_on_close_1048_df3e, win_center_1048_d80b removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 65 of the imported file. */
/* show_work_buffer_dialog: candidate for 1048:ae68 (278 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_work_buffer_dialog
 * name-source: descriptive
 *
 * The "Work Buffer" dialog: a window the size of the backdrop port at SEG28:0, centred, with
 * a click handler that sets DGROUP:0x1fdc, the port blitted into it, and then a pump loop
 * that runs `MSDoTask` until that flag goes non-zero. The flag is cleared on the way out and
 * the window destroyed by its own destructor.
 *
 * SO THE DIALOG IS MODAL BY SPINNING, not by a message loop of its own: the only thing that
 * ends it is `work_buffer_dismiss_handler`, whose whole body is setting that flag.
 *
 * The window's size is asked for TWICE -- once for `GameWin_Init` and
 * again for the `SetRect` that the blit uses -- rather than kept in a local, which is what a
 * source that computes nothing looks like at `-Od`.
 *
 * The title is read out of DGROUP: "Work Buffer" at 0x23b1 (§119).
 *
 * `GameWin`'s constructor and destructor are -called and so are defined here (rule 47);
 * `GameWin_Init`, `GRPort_CopyToPort` and `SetRect` are calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-
#pragma option -a-


class DlgControl;



   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;         /* SEG20:0x5ca */

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` */
extern "C" void show_window_1048_dd42(GameWin *w);

void show_work_buffer_dialog(void)
{
    GameWin w;
    RECT r;

    GameWin_Init(&w, "Work Buffer", 0x269, 0, 0,
                                     port_width_1048_d853(&g_map_port),
                                     port_height_1048_d86c(&g_map_port),
                                     &g_game_palette, (Screen *)&g_main_win);
    win_center_1048_d80b((DialogWin *)&w);
    set_on_close_1048_df3e((EventTable *)&w, (void*)(work_buffer_dismiss_handler));
    SetRect(&r, 0, 0, port_width_1048_d853(&g_map_port),
            port_height_1048_d86c(&g_map_port));
    GRPort_CopyToPort(&g_map_port, &w, &r, &r);
    show_window_1048_dd42(&w);
    while (g_work_buffer_dismissed == 0)
        MSDoTask_1048_db08();
    g_work_buffer_dismissed = 0;
}

