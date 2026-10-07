/* MSOpenResFile: candidate for 1078:00ef (141 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * LoadLibrary(name); a failure is reported ("Error: Resource file <name> not found.") and
 * returns 0, else the handle is appended to the table at DGROUP:0x56e6 (count at 0x3c76)
 * and returned.
 *
 * RENAMED 2026-09-27 from open_resource_dll: NOT PROPOSED directly (mac_bind proposed ResourceFile_ctor_1010_281e, a ctor that calls this). Mac MSOpenResFile(char*). Callers through the three ResourceFile_ctor copies: run_game (Mac GameMain), DoTheClosing and show_opening (Mac DoTheOpening) are Mac MSOpenResFile's 3 callers (Windows adds the about box and the opening art). Arity 1 (name). RESOURCE BLOCK: Windows 1078 open_resource_dll (00ef), free_resource (0186), MSGetResource (019c), MSGetNamedResource (0277), lock_resource (0386), unlock_resource (03a0). The Mac MS file has MSOpenResFile, MSReleaseResource, MSGetResource, MSGetNamedResource, MSLockResource, MSUnlockResource. The order is EXACT, anchored on the two already-bound names. The per-module forwarders take MSName_<seg>_<off>, as the existing MSGetResource_1068_94c7 family does.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

unsigned MSOpenResFile(const char *name)
{
    char msg[128];
    unsigned h = LoadLibrary(name);

    if (h == 0) {
        STRCPY(msg, "Error: Resource file ");
        STRCAT(msg, name);
        STRCAT(msg, " not found.");
        stub_reporter(&g_error_log, msg);
        return 0;
    } else {
        g_resource_libs[g_resource_lib_count] = h;
        g_resource_lib_count++;
        return h;
    }
}
