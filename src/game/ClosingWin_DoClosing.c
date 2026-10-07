/* ClosingWin_DoClosing: candidate for 1010:032c (195 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ClosingWin_DoClosing
 * name-source: sibling-build
 *
 * Plays the closing sequence on a ClosingWin: set-up (show_window_1010_2af0), sound
 * 0x3d, the animation at +0x100 with DoFireworks as its callback, the
 * message pump until DGROUP:0x4b98 is set, then sound off, animation stop,
 * the two windows (the dialog part at +0x26 and `win` at +0x678) hidden and the palette range 10..245
 * restored from +0x704.  The five module helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from closing_play: the Mac build's ClosingWin::DoClosing(void): this only, playing the closing sequence and pumping until it ends. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, MagicWin, DiskFile, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


class DlgControl;



#include "records.h"

/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */
class ClosingWin : public MagicWin {
public:
    Flic flic;
    EventWin win;
    char palette[0x9c8 - 0x704];
    ClosingWin();
    ~ClosingWin();
};
   /* 0x41e bytes: the palette, its handle and the cycling state */

extern int g_stub_a;
extern void sound_play(int n);
extern void DoFireworks(void);
extern void Flic_Loop(Flic *anim, void *port);

extern void Flic_Reset(Flic *anim);
struct logpal { char pad[1]; };
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern "C" void show_window_1010_2af0(void *w);

extern "C" void set_handler_574(void *anim, void *fn);

extern "C" void MSHideWindow_of_1010_2b41(void *port);

void ClosingWin_DoClosing(ClosingWin *w)
{
    show_window_1010_2af0(w);
    MSDoAllTasks_1010_2a54();
    sound_play(0x3d);
    set_handler_574(&w->flic, (void *)DoFireworks);
    Flic_Loop(&w->flic, w);
    while (g_closing_flag == 0)
        MSDoTask_1010_2b2a();
    MSCloseSound();
    Flic_Reset(&w->flic);
    MSHideWindow_of_1010_2b41(&(DialogWin &)*w);
    CTable_Fill(&g_game_palette, 10, 0xec, (unsigned char *)w->palette);
    MSDoAllTasks_1010_2a54();
    MSHideWindow_of_1010_2b41(&w->win);
    MSDoAllTasks_1010_2a54();
}
