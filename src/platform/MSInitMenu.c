/* MSInitMenu: candidate for 1070:d5e6 (45 bytes).  Verdict: RELOCATABLE.
 *
 * LoadMenu(hInstance, MAKEINTRESOURCE(id)); the instance handle lives at DGROUP:0x56e2
 * (declared , so it is read through an explicit segment load).
 */

#include "colonization.h"

unsigned MSInitMenu(unsigned id)
{
    return LoadMenu(g_hinstance, (const char *)(unsigned long)id);
}
