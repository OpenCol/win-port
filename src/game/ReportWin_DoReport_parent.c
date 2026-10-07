/* ReportWin_DoReport_parent: candidate for 1058:02a6 (631 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_DoReport_parent
 * size-source: 631, not the census 629 -- the census stops two bytes inside the epilogue.
 *
 * Opens a report window of kind `kind` and, when its +0x7f8 flag says modal, runs it to its
 * answer and returns the word at +0x7f6. Everything before that is setup: `ReportWin_Init`,
 * the three SButtons at +0x8ac..+0x8b4 deleted and nulled, the layout, the icon 0x83, the
 * keyboard handler in slot 40, and `MSFlushInput` three times over to let the queue drain.
 *
 * TWO REPORTS DIE AT INDEPENDENCE. Kind 8 with bit 0 of SEG20:0x7784 set answers
 * "FOREIGNNOTAVAIL" and returns 0, kind 2 answers "RELIGIOUSNOTAVAIL" -- the foreign-affairs
 * and religious advisors have nobody left to report on once the Crown is renounced. Kind 0x63
 * forces the modal flag on instead.
 *
 * The two refusals are one `if`/`else if`: the dead `JMP` after the first `return 0`'s epilogue
 * is the `else` arm's jump, which two separate `if`s would not have emitted.
 *
 * The three deletes are Borland's -- pointer to a local, a null test the source does not
 * contain, `ADD DWORD SS:[0x10],2` for SButton's two destructible sub-objects (rule 49), the
 * destructor with flag 3 -- and the three null stores come AFTER all three, not inside each
 * `if`, which is where `ReportWin_Done` puts them.
 *
 * `SButton::~SButton()` IS DEFINED HERE AND NOT MERELY DECLARED. Rule 47 covers the compiler's
 * own calls too: with the destructor in this translation unit the `delete` compiles to the
 * four-byte `PUSH CS / CALL NEAR` the shipped has; declared alone it is a five-byte call
 * and the comparator has to collapse all three.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
class SButton;

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern "C" int popm(char *msg, int n);   /* PORT: C linkage, as defined (W2) */
extern void GameWin_Redraw(GameWin *o);
extern "C" void ColChild_Center(ColChild *o);   /* PORT: C linkage, as defined (W2) */
extern void EventWin_Modal(EventWin *w);

/* same object: 4-byte `push cs; call ` */
extern "C" void MSFlushInput_1058_bba6(void);

extern "C" void ReportWin_Init(ReportWin *o, char flag, void *rec);
extern "C" void show_window_1058_bc28(GameWin *o);

extern "C" void win_set_icon_1058_b9ab(DialogWin *w, int icon);
extern "C" void *set_on_keydown_after_1058_bc70(EventTable *table, void *h);
extern "C" void win_release_1058_bca9(GameWin *o);

int ReportWin_DoReport_parent(ReportWin *o, register int kind, char flag,
                                  void *rec)
{
    if (kind == 8 && (g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
        popm("FOREIGNNOTAVAIL", 1);
        return 0;
    } else if (kind == 2 && (g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
        popm("RELIGIOUSNOTAVAIL", 1);
        return 0;
    }
    if (kind == 0x63)
        flag = 1;
    MSFlushInput_1058_bba6();
    ReportWin_Init(o, flag, rec);
    o->score = 0;
    o->kind = kind;
    if (o->prev != 0)
        delete o->prev;
    if (o->next != 0)
        delete o->next;
    if (o->extra_btn != 0)
        delete o->extra_btn;
    o->prev = 0;
    o->next = 0;
    o->extra_btn = 0;
    MSDoAllTasks_1058_bc15();
    o->page = 0;
    o->view = 0;
    GameWin_Redraw((GameWin *)o);
    ColChild_Center((ColChild *)o);
    show_window_1058_bc28(o);
    win_show_and_focus_1058_bc47(&(DialogWin &)*o);
    o->kind = kind;
    MSFlushInput_1058_bba6();
    MSDoAllTasks_1058_bc15();
    win_set_icon_1058_b9ab(&(DialogWin &)*o, 0x83);
    MSFlushInput_1058_bba6();
    set_on_keydown_after_1058_bc70(&(EventTable &)*o, (void *)ReportWin_KeyClick);
    if (o->modal != 0) {
        EventWin_Modal(&(EventWin &)*o);
        win_release_1058_bca9(o);
    }
    return o->score;
}

