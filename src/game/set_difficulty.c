/* set_difficulty: candidate for 1030:116a (128 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: set_difficulty
 * name-source: sibling-build
 *
 * The twin of `set_nation` 0x928 bytes below it, for the difficulty dialog: run
 * `SetDifficulty`, store the answer less one at SEG20:0x77a8 -- the difficulty byte
 * `king_adjust_tax`, `europe_new_wars` and `unit_from_docks` all read -- and return 1 when
 * nothing was chosen.
 *
 * The two functions are the same program with three substitutions, and the third is the
 * interesting one: `set_nation` stores a WORD to 0x779a and this stores a BYTE to 0x77a8.
 * The subtraction is `ADD AL,0xff` in both, but only this one keeps the result 8-bit all the
 * way to memory, because the global itself is `unsigned char` here and `int` there. Two
 * dialogs, two answer widths, one shape.
 *
 * The class is `SetDifficulty_dtor`'s, unchanged; the object is declared last (§130) and the
 * return crosses the destructor through `PUSH AX` / `POP AX`.
 *
 * RENAMED 2026-09-27 from choose_difficulty: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). LEAD 1. The Mac build has no choose_difficulty or choose_nation. The only choose_* symbols are choose_cargo(char*) and choose_freedom(short), and none of the 194 long-form MacsBug names that the extractor missed is one either (see chains). Call graph: Mac set_difficulty(void) is called only by get_game_options(void), and 116a is called only by 1008:bd72 get_game_options. Both call orders are the same: Mac get_game_options' seq runs set_difficulty then set_nation, and bd72 calls choose_difficulty (line 130) then choose_nation (line 137). Callees: Mac set_difficulty builds the dialog object (ctor inlined: GRPort/Screen ctors, ClearEventTable) and then calls SetDifficulty::Init, inside an anon_6_467a/4726 EH frame. 116a calls SetDifficulty_ctor, SetDifficulty_Init and SetDifficulty_dtor inside __InitExceptBlock. Arity: (void) on both sides. Body: it runs the SetDifficulty dialog and stores the answer less one in g_game.difficulty, the job of a set_. Layout: Mac CODE 17 runs SetDifficulty OkButtonClick, LeftMouseClick, DrawIt, Init, TrackMouse, DoMessage, then set_difficulty and the dtor. Windows 1030 runs 08c4, 091b, SetDifficulty_DrawIt, _Init, _TrackMouse, _DoMessage, then 116a. Each is the last function of its class block. Size: 866 Mac body against 128 Windows. The Mac inlines the SetDifficulty constructor, and Windows calls it out of line at 1030:e836. Strings: none on either side. The earlier docs/naming rejections of set_nation were token-similarity pairings to four other Windows functions and do not bear on this. No collision. Also update tools/check_call_forms.py KNOWN, which lists 'choose_difficulty.c' by filename.
 * SetDifficulty::SetDifficulty, SetDifficulty_Init and SetDifficulty::~SetDifficulty are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" extern "C" int SetDifficulty_Init(SetDifficulty *s);
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern "C" int set_difficulty(void)
{
    register int cancelled;
    int answer;
    SetDifficulty sd;

    cancelled = 0;
    answer = (unsigned char)SetDifficulty_Init(&sd);
    if (answer == 0)
        cancelled = 1;
    else
        g_game.difficulty = (unsigned char)(answer - 1);
    return cancelled;
}
