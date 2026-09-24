/* menu_attach_1050_906a -- shipped at 1050:906a.
 *
 * Ported by tools/port.py from matched/game/menu_attach_1050_906a.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[6]; void *menu; } DialogWin;
extern void * menu_attach_to_window(void *menu);

void menu_attach_1050_906a(DialogWin *d)
{
    d->menu = menu_attach_to_window(d->menu);
}
