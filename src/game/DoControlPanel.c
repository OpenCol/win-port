/* DoControlPanel: candidate for 1048:c05a (88 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DoControlPanel
 *
 * Builds a CDControlWin on the stack and hands it to CDControlWin_Init, which runs it. The
 * whole function is one local object and one call: the 0xa7c-byte frame, the exception
 * block and the destructor with flag 2 are all what `-P` emits for a local object with a
 * destructor. The class tower is copied from CDControlWin_ctor.c.
 *
 * RENAMED 2026-09-27 from jukebox_control_panel: the Mac build's DoControlPanel(void), bound by call-sequence alignment (tools/mac_bind.py): it builds the CD jukebox window (CDControlWin) and runs it -- one routine on the Mac, hence the size ratio.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
class CDControlWin : public ColChild {
public:
    SImageButton ib1, ib2, ib3, ib4, ib5;
    SButton b;
    short pad;
    SImageButton ib6;
    char tail[8];
    CDControlWin();
    ~CDControlWin();
};

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

extern void CDControlWin_Init(CDControlWin *w);

void DoControlPanel(void)
{
    CDControlWin w;

    CDControlWin_Init(&w);
}

