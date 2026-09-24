/* text_prompt -- shipped at 1040:62b6.
 *
 * Ported by tools/port.py from matched/game/text_prompt.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void wrap_text(void *port, void *r, char *text, col_i16 draw);

void text_prompt(void *port, void *r, char *text)
{
    wrap_text(port, r, text, 1);
}
