/* GRPort_Init_wh: candidate for 1068:34ed (66 bytes).  Verdict: RELOCATABLE.
 *
 * GRPort_Init with a rectangle (0, 0, w, h); its result is returned (the trailing dead
 * epilogue says so).
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"
extern int GRPort_Init(GRPort *p, void *r);

int GRPort_Init_wh(void *p, int w, int h)
{
    RECT r;

    SetRect(&r, 0, 0, w, h);
    return GRPort_Init((GRPort *)p, &r);
}
