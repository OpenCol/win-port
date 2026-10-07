/* set_window_caption: candidate for 1058:bd6c (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: set_window_caption
 *
 * Sets a game window's caption: the Windows title through the module's wrapper (defined here
 * because the shipped call is the compiler's own `push cs / call `), the copy kept in
 * the ColChild's `title` (+0xc4), and the frame redrawn (ColChild_DrawTitleBar).
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
/* The bases' and members' constructors are defined in this file because the shipped calls to
 * them are the compiler's own `push cs / call ` (GRPort's and MenuClass's are calls). */
#include "records.h"
extern void MSSetWinTitle(MSWin *win, const char *s);

extern void ColChild_DrawTitleBar(ColChild *w);

extern "C" void MSSetWinTitle_of_1058_c555(DialogWin *o, const char *title);

void set_window_caption(ColChild *w, char *s)
{
    MSSetWinTitle_of_1058_c555(&(DialogWin &)*w, s);
    STRCPY(w->title, s);
    ColChild_DrawTitleBar(w);
}
