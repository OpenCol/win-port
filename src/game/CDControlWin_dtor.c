/* CDControlWin_dtor: candidate for 1048:e49a (210 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CDControlWin::~CDControlWin()
 *
 * The destructor of CDControlWin: nothing beyond the compiler's member and base destruction.
 * SImageButton::~SImageButton, SButton::~SButton and ColChild::~ColChild are defined in-file first, for the 4-byte calls.
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

CDControlWin::~CDControlWin()
{
}
