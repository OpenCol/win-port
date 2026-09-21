/* create_palette -- shipped at 1070:0442.
 *
 * Ported by tools/port.py from matched/platform/create_palette.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_palette_mode;                           DGROUP:0x3380
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal CreatePalette(void *lp);;
#endif

col_i16 create_palette(void *lp)
{
    register col_i16 r;

    if (g_palette_mode == 1)
        r = CreatePalette(lp);
    else
        r = 0;
    return r;
}
