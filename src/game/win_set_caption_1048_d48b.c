/* win_set_caption_1048_d48b: candidate for 1048:d48b (74 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_set_caption_1048_d48b
 * name-source: descriptive
 *
 * Sets a game window's caption in all three places it lives: hands it to the window's port
 * at +0x26 through `win_set_title` (which calls MSSetWinTitle), keeps a copy in the window's
 * own buffer at +0xc4, and redraws the title bar with `ColChild_DrawTitleBar`.
 *
 * `message_timer_expired` is the caller that shows what it is for: when the map window's
 * message timer fires, the map's message line is refreshed by writing "Area Map" as its
 * caption through this. That file already carried this exact body as a rule-47 stub; this
 * is the same text graded at its own address.
 *
 * One of two per-module copies: `win_set_caption_1008_cf0f` is module 1008's, which
 * reaches its title setter under that module's name and ColChild_DrawTitleBar as a call.
 *
 * Both inner calls are the four-byte `push cs; call ` with no NOP, so both callees are
 * defined here (rule 47); STRCPY is a call into the runtime.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void MSSetWinTitle(MSWin *win, char *s);

extern "C" void win_set_title(DialogWin *w, char *s);
extern "C" void ColChild_DrawTitleBar(ColChild *w);

void win_set_caption_1048_d48b(ColChild *w, char *s)
{
    win_set_title(&(DialogWin &)*w, s);
    STRCPY(w->title, s);
    ColChild_DrawTitleBar(w);
}
