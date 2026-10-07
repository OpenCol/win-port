/* load_wing_library: candidate for 1070:b13e (26 bytes).  Verdict: RELOCATABLE.
 *
 * Loads the resource DLL "WinG.dll" and keeps its instance handle at DGROUP:0x56e0.
 *
 * RENAMED 2026-09-26 from load_resource_library: Its whole body is g_wing_library = LoadLibrary("WinG.dll"), and COLONIZE.EXE does contain the string 'WinG.dll'. It pins the WinG graphics DLL during start-up (register_window_classes). It is not a resource DLL, and nothing loads resources through that handle (DGROUP:0x56e0 is read only by FreeLibrary in free_resource_library, which should follow this rename, along with g_wing_library).
 */

#include "colonization.h"

extern unsigned g_wing_library;   /* DGROUP:0x56e0 */

void load_wing_library(void)
{
    g_wing_library = LoadLibrary("WinG.dll");
}
