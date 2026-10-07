/* MSGetNamedResource: candidate for 1078:0277 (271 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Finds a named resource of a four-character type: spaces, dots and apostrophes in the name
 * become underscores, the game's own instance (DGROUP:0x56e2) is searched first and then each
 * loaded resource library (DGROUP:0x56e6, count at 0x3c76); the first hit is loaded and its
 * global handle returned, 0 when none has it.
 *
 * RENAMED 2026-09-27 from find_and_load_resource: the Mac build's MSGetNamedResource(unsigned char*, unsigned char*): the body takes (type4, char *name), rewrites spaces, dots and apostrophes in the name and looks it up by string; the numeric twin (1078:019c) is MSGetResource.
 *
 * Returns `void *` (2026-10-02), as its callers use it: the LoadResource handle,
 * zero-extended, the same bytes as the `unsigned long` it was spelled with.
 */

#include "colonization.h"

void *MSGetNamedResource(unsigned char *type4, unsigned char *name)
{
    char type[5];
    unsigned i;
    unsigned h;

    type[0] = type4[0];
    type[1] = type4[1];
    type[2] = type4[2];
    type[3] = type4[3];
    type[4] = 0;
    for (i = 0; STRLEN(name) > i; i++)
        if (name[i] == ' ' || name[i] == '.' || name[i] == '\'')
            name[i] = '_';
    i = -1;
    if ((h = FindResource(g_hinstance, name, type)) == 0) {
        for (i = 0; i < g_resource_lib_count; i++) {
            if ((h = FindResource(g_resource_libs[i], name, type)) != 0)
                break;
        }
    }
    if (h != 0)
        return (void *)(unsigned long)LoadResource(i == -1 ? g_hinstance : g_resource_libs[i], h);
    else
        return 0;
}
