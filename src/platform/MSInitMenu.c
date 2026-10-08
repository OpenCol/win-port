/* PORT: integer <-> pointer through uintptr_t/intptr_t: a resource id, a 16-bit handle token or a parameter block, the same bits on i386 and on 64-bit Windows (W9). */
/* MSInitMenu: candidate for 1070:d5e6 (45 bytes).  Verdict: RELOCATABLE.
 *
 * LoadMenu(hInstance, MAKEINTRESOURCE(id)); the instance handle lives at DGROUP:0x56e2
 * (declared , so it is read through an explicit segment load).
 */

#include "colonization.h"

unsigned MSInitMenu(unsigned id)
{
    return LoadMenu(g_hinstance, (const char *)(uintptr_t)id);
}
