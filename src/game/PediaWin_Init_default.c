/* PediaWin_Init_default: candidate for 1060:15d0 (33 bytes).  Verdict: RELOCATABLE.
 *
 * PediaWin_Init with the defaults: the plain form, the default record. The call is a call
 * TLINK converted to its NOP-padded `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void PediaWin_Init(PediaWin *o, int flag, void *rec);

void PediaWin_Init_default(void *o)
{
    PediaWin_Init((PediaWin *)o, 0, 0);
}
