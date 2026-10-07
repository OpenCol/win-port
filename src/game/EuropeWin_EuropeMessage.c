/* EuropeWin_EuropeMessage: candidate for 1048:1a52 (58 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeMessage
 * name-source: sibling-build
 *
 * Makes the message's own nation current, then appends the indexed word from the table in segment 20.
 *
 * RENAMED 2026-09-26 from msg_add_nation_word: the Mac build's EuropeWin::EuropeMessage(short): this plus a message index, the word appended to the window's message -- what colony_message is for the colony window. It sits in the Europe module (1048) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void set_europe(int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_AddToMessage(EuropeWin *m, int idx);

void EuropeWin_EuropeMessage(EuropeWin *win, int i)
{
    set_europe(win->nation);
    ColChild_AddToMessage(win, g_colony_message_words[i]);
}
