/* MSLoadCursor: candidate for 1078:2875 (60 bytes).  Verdict: RELOCATABLE.
 *
 * LoadCursor(hInstance, MAKEINTRESOURCE(id)) through a local.
 */

#include "colonization.h"

unsigned MSLoadCursor(unsigned id)
{
    unsigned h = LoadCursor(g_hinstance, (const char *)(unsigned long)id);
    return h;
}
