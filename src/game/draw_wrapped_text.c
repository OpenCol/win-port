/* PORT: this file's own declarations of ParseFancyTextAndShow removed: the header declares it from the definition (W1 step 3). */
/* draw_wrapped_text: candidate for 1040:62b6 (42 bytes).  Verdict: RELOCATABLE.
 *
 * ParseFancyTextAndShow with a trailing 1.
 *
 * RENAMED 2026-09-26 from text_prompt: Body is one call, ParseFancyTextAndShow(port, r, text, 1): it lays the text out word by word in the rectangle and draws it (draw=1). Its callers, DoFancyText and draw_text_resource, use it to render a page of text or a text resource into a window. Nothing in it prompts for or reads input, and 'prompt' is a misleading word here because text_prompt_unused really is an unfinished text-entry dialog with an edit box.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void draw_wrapped_text(void *port, void *rect, char *text)
{
    ParseFancyTextAndShow((GRPort *)port, (RECT *)rect, text, 1);
}
