/* PORT: this file's own declarations of MSSetWinTitle_of_1038_e517 removed: the header declares it from the definition (W1 step 3). */
/* game_win_set_text_1038_dffc: candidate for 1038:dffc (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_win_set_text_1038_dffc
 *
 * The map module's copy of game_win_set_text: MSSetWinTitle_of_1038_e517 on the
 * DialogWin part (+0x26), STRCPY into +0xc4, ColChild_DrawTitleBar redraws.
 * MSSetWinTitle_of_1038_e517 is -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern void ColChild_DrawTitleBar(ColChild *w);


void game_win_set_text_1038_dffc(ColChild *o, char *s)
{
    MSSetWinTitle_of_1038_e517(&(DialogWin &)*o, s);
    STRCPY(o->title, s);
    ColChild_DrawTitleBar(o);
}
