/* Opening_RefreshUpdatePort: candidate for 1010:0d96 (285 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_RefreshUpdatePort
 * name-source: sibling-build
 *
 * Redraws the opening into its window port: the title band shifted by the
 * scroll counter (0x500 - scroll while under 0x500), the lower band, the
 * ship sprite (+0x1e7e, the last ship frame Opening_DoShip drew) at (ship_x - shift, ship_y)
 * when present, and sound 0x25 unless sound_played (+0x1e86) is set.
 *
 * RENAMED 2026-09-26 from opening_title_draw: the Mac build's Opening::RefreshUpdatePort(void): this only, redrawing the whole opening (title band, lower band, ship sprite) into the window's port. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "sounds.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern "C" void sound_play(int n);   /* PORT: C linkage, as defined (W2) */

void Opening_RefreshUpdatePort(Opening *w)
{
    RECT r1;
    RECT r2;
    RECT r3;
    register int shift = 0;

    if (w->scroll <= 0x500) {
        shift = 0x500 - w->scroll;
        SetRect(&r1, shift, 0x38, shift + 0x280, 0x17b);
    } else {
        SetRect(&r1, 0, 0x38, 0x280, 0x17b);
    }
    SetRect(&r2, 0, 0x38, 0x280, 0x17b);
    GRPort_CopyToPort(&w->port1, (GRPort *)w, &r1, &r2);
    SetRect(&r2, 0, 0x17b, 0x280, 0x1e0);
    GRPort_CopyToPort(&w->port1, (GRPort *)w, &r2, &r2);
    if (w->ship_sprite != 0)
        Sprite_Draw(&r3, w->ship_sprite, (GRPort *)w, w->ship_x - shift, w->ship_y);
    if (w->sound_played == 0)
        sound_play(SOUND_MUSIC_JINE_THE_CAVALRY);
}
