/* rtl_call_346c: candidate for 1040:5f1d (56 bytes).  Verdict: RELOCATABLE.
 *
 * The runtime routine at 1000:346c on the signed byte at +4 of the record,
 * returned as a long.
 */

#include "colonization.h"

typedef struct { char pad[4]; signed char handled; } Rec;

long rtl_call_346c(Rec *r)
{
    long v;

    v = L1_346c(r->handled);
    return v;
}
