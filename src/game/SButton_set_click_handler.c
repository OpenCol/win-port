/* SButton_set_click_handler -- shipped at 1058:c17f.
 *
 * Ported by tools/port.py from matched/game/SButton_set_click_handler.c.
 *
 * The file's own SButton dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: click -> f1c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void SButton_set_click_handler(SButton *b, void *h)
{
    b->f1c = h;
}
