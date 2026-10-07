/* wing_dib_pointer: candidate for 1070:a86b (48 bytes).  Verdict: RELOCATABLE.
 *
 * WinGGetDIBPointer for the bitmap handle at +6 of the record; null for a
 * null record.
 */

#include "colonization.h"

#include "records.h"

unsigned char *wing_dib_pointer(GrafRec *graf)
{
    if (graf == 0)
        return 0;
    return (unsigned char *)WinGGetDIBPointer(graf->hbm, 0);
}
