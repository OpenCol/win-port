/* perform_save: candidate for 1028:d1ec (144 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * SAVES THE GAME to a named slot. The slot picks the file name -- 10 is `youwin.sav`, 9 is
 * `auto01.sav`, 8 is `auto02.sav`, and anything else takes the season/year name from
 * `BuildSaveGameName` -- which is copied into a path buffer and handed to
 * `save_game`.
 *
 * RENAMED from `youwin_save_path`, which named the function after the first of its four cases and
 * after the buffer rather than the act. Three of the four slots have nothing to do with winning,
 * and the function does not hand back a path: it performs the save.
 *
 * RENAMED 2026-09-26 from save_game_to_slot: the Mac build's perform_save(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller DoEndofTurn; caller autosave), the runner-up gets 1, the arity agrees (1), and the Mac body is 1.40x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

extern void BuildSaveGameName(char *dst);
extern int save_game(char *path);

void perform_save(int slot)
{
    char name[0x50];
    char path[0x100];

    if (slot == 10)
        STRCPY(name, "youwin.sav");
    else if (slot == 9)
        STRCPY(name, "auto01.sav");
    else if (slot == 8)
        STRCPY(name, "auto02.sav");
    else
        BuildSaveGameName(name);
    STRCPY(path, name);
    save_game(path);
}
