/* MainWin_RightMouseClick -- shipped at 1048:a6db.
 *
 * Ported by tools/port.py from matched/game/MainWin_RightMouseClick.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void track_popup_submenu(char *w, char *m, void *p);

extern char * current_window(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

void MainWin_RightMouseClick(void *p)
{
    char *w = current_window();

    track_popup_submenu(w, w + 0x8c, p);
}
