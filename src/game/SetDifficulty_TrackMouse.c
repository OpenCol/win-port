/* SetDifficulty_TrackMouse: candidate for 1030:0ec7 (133 bytes, 2 call sites).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty_TrackMouse
 * name-source: sibling-build
 * A two-band hit test: given screen coordinates it returns a region number 1-5, or 0
 * for a miss. Reconstructed from Ghidra's decompiled C, which recovered the nested
 * conditionals correctly; the only hand work was the frame layout, since the arguments
 * are read from [BP+0xa] and [BP+0xc] with two words ahead of them, so the first
 * parameter occupies 4 bytes and the body never touches it.
 *
 * Verdict EXACT. This function is one of the two that discriminate the -O flag: with
 * -O it diverges at +0x2d, where -O rewrites the branch layout. That is what corrected
 * the documented flag set from `-ml -WS -O -Ot -3` to `-ml -WS -Ot -3`; see
 * docs/findings/toolchain.md.
 * Re-verified 2026-09-10 at the full 133-byte extent, trailing dead epilogue included: EXACT.
 *
 * RENAMED 2026-09-26 from hittest_region: the Mac build's SetDifficulty::TrackMouse(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between SetDifficulty_Init and Customize_OkButtonClick, where the Mac gap holds 4 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.17x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order. The Windows dialogs split their hit test out of TrackMouse (next-session: 'the width is 6 not 8'); layout puts TrackMouse on the hit test and DoMessage on the tracker.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
int SetDifficulty_TrackMouse(SetDifficulty *unused, int x, int y)
{
    int r = 0;

    if (y > 0x11 && y < 0xe7) {
        if (x > 0xff && x < 0x188)       r = 1;
        else if (x > 0x1d1 && x < 0x25a) r = 2;
    } else if (y > 0xf8 && y < 0x1cf) {
        if (x > 0x2d && x < 0xb6)        r = 3;
        else if (x > 0xff && x < 0x188)  r = 4;
        else if (x > 0x1d1 && x < 0x25a) r = 5;
    }
    return r;
}
