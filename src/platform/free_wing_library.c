/* free_wing_library: candidate for 1070:b158 (23 bytes).  Verdict: RELOCATABLE.
 *
 * Frees the WinG library whose module handle is kept at DGROUP:0x56e0.
 *
 * RENAMED 2026-09-26 from free_resource_library: it FreeLibrary's the handle load_wing_library (was load_resource_library) got from LoadLibrary("WinG.dll"); the library is WinG, not a resource DLL.
 */

#include "colonization.h"

extern unsigned g_wing_library;   /* DGROUP:0x56e0 */

void free_wing_library(void)
{
    FreeLibrary(g_wing_library);
}
