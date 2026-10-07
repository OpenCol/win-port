/* game_win_set_text_1010_e756: candidate for 1010:e756 (75 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_win_set_text_1010_e756
 *
 * Gives a GameWin its text: MSSetWinTitle_of_1010_edd2 on the DialogWin part (+0x26),
 * STRCPY into the buffer at +0xc4, then ColChild_DrawTitleBar redraws.
 * MSSetWinTitle_of_1010_edd2 is -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern void ColChild_DrawTitleBar(ColChild *w);

extern "C" void MSSetWinTitle_of_1010_edd2(void *d, char *s);

void game_win_set_text_1010_e756(ColChild *o, char *s)
{
    MSSetWinTitle_of_1010_edd2(&(DialogWin &)*o, s);
    STRCPY(o->title, s);
    ColChild_DrawTitleBar(o);
}
