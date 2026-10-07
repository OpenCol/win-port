/* DoTheClosing: candidate for 1010:0026 (130 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DoTheClosing
 * name-source: sibling-build
 *
 * `show_opening`'s counterpart at the other end of the segment: open a resource DLL as a
 * ResourceFile, build a ClosingWin on the stack, play it, drop the mouse capture, and let
 * both destructors fire on the way out. The exception block is theirs.
 *
 * The frame is the sum of the two objects and it is checkable by hand: ClosingWin is 0x9c8
 * bytes at `bp-0x9cc` and ResourceFile is TWO at `bp-4`, which puts the ClosingWin's last
 * byte exactly where the ResourceFile begins. A four-byte ResourceFile -- the reading that
 * kept `show_opening` parked for a day -- would overlap the saved `DS` at `bp-2`.
 *
 * `set_wing_orientation(0)` between the play and the teardown is a call and is `extern`; the
 * two constructors, the play and the two destructors are all this module's own.
 *
 * The literal is `coldata7.dll`, read out of the string table at DGROUP:0x99c -- `show_opening`
 * opens `coldata5.dll` from 0x9b6, 0x1a (26) bytes further on, past `winflc.wav`. §119 says a green verdict cannot
 * check a string literal, so this one is checked the only way it can be: against the address
 * the shipped code pushes.
 *
 * RENAMED 2026-09-26 from show_closing: the Mac build's DoTheClosing(void), bound by call-sequence alignment (1.0 aligned gaps in GameBrains_DoEndofTurn leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.11x this one.
 * ResourceFile's constructor and destructor are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "classes.h"
#include "w16types.h"
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
extern "C" void ClosingWin_DoClosing(ClosingWin *w);

extern "C" void DoTheClosing(void)
{
    ResourceFile rf("coldata7.dll");
    ClosingWin cw;

    ClosingWin_DoClosing(&cw);
    set_wing_orientation(0);
}
