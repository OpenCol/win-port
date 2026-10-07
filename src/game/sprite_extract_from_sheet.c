/* sprite_extract_from_sheet: candidate for 1008:d6b2 (53 bytes).  Verdict: RELOCATABLE.
 * symbol: sprite_extract_from_sheet
 *
 * Runs the message pump once, then extracts the sprite from the sprite-sheet port at
 * SEG20:0xdea0 (g_sprite_sheet). call_message_pump is defined here because the shipped call to it is the
 * compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-27 from sprite_extract_to_screen: the global it reads from is the loaded sprite sheet (g_sprite_sheet, renamed with it): the body copies a cell OUT of that sheet into a caller buffer, never onto the screen.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void Sprite_Extract_mask(Sprite *s, void *src, unsigned char transparent, int x, int y, int w, int h);

void sprite_extract_from_sheet(void *s, int mode, int x, int y, int w, int h)
{
    call_message_pump();
    Sprite_Extract_mask((Sprite *)s, &g_sprite_sheet, mode, x, y, w, h);
}
