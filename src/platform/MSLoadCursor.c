/* PORT: integer <-> pointer through uintptr_t/intptr_t: a resource id, a 16-bit handle token or a parameter block, the same bits on i386 and on 64-bit Windows (W9). */
/* MSLoadCursor: candidate for 1078:2875 (60 bytes).  Verdict: RELOCATABLE.
 *
 * LoadCursor(hInstance, MAKEINTRESOURCE(id)) through a local.
 */

#include "colonization.h"

unsigned MSLoadCursor(unsigned id)
{
    unsigned h = LoadCursor(g_hinstance, (const char *)(uintptr_t)id);
    return h;
}
