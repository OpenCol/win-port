/* CDControlWin_ctor: candidate for 1048:e3d6 (196 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CDControlWin::CDControlWin()
 *
 * The constructor of CDControlWin (0xa5e bytes, over ColChild, vtable at DGROUP:0x2028)
 * with five SImageButtons, an SButton and a sixth SImageButton: nothing beyond the
 * compiler's base and member construction through module 1048's copies.
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

CDControlWin::CDControlWin()
{
}
