/* EuropeWin_LoadEurope: candidate for 1048:89dc (45 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_LoadEurope
 * name-source: sibling-build
 *
 * Loads picture 0xfb (40x96) with the game palette (SEG20:0x1ac) into the
 * Europe window's GRPort at +0x884.
 *
 * RENAMED 2026-09-26 from europe_load_picture: the Mac build's EuropeWin::LoadEurope(void): this only, loading the Europe screen's picture into the window's port. It sits in the Europe module (1048) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x41e bytes: the palette, its handle and the cycling state */

struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *port, int id, int w, int h, struct logpal *pal);

void EuropeWin_LoadEurope(EuropeWin *w)
{
    GRPort_LoadPicture(&w->port, 0xfb, 0x28, 0x60, (struct logpal *)&g_game_palette);
}
