/* Screen_dtor: candidate for 1060:a482 (82 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Screen::~Screen()
 *
 * The destructor of Screen: the window record at +6 is disposed through MSDisposeWindow and
 * the field takes what it returns (0). It was read as a menu after that function's old name,
 * menu_attach_to_window; get_win reads the same +6 as the window. (Corrected 2026-09-26.)
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void *MSDisposeWindow(void *win);

Screen::~Screen()
{
    win = MSDisposeWindow((MSWin *)win);
}
