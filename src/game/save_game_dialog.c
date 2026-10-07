/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 103, 105, 107 of the imported file. */
/* save_game_dialog: candidate for 1028:dca2 (1622 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: save_game_dialog
 * name-source: descriptive
 *
 * THE SAVE-GAME SLOT PICKER AND ITS ANSWER. It opens the menu that was already built into the
 * `Popup` at style 0x800, reads the slot the player chose, writes the game out with
 * `save_game`, and then says how it went -- SAVEGOOD, SAVEMEM (out of memory) or
 * SAVEERROR. On a successful save it also builds the one-line description the load menu will show
 * for that slot: difficulty word, leader's name, the word at SEG20:0xdcee, the nation's own word,
 * and in brackets the season word and the year, all left in name slot 1.
 *
 * WHICH NATION that line describes is not the player's: it is the FIRST of the four whose byte at
 * SEG20:0x7841 is zero, and 0 when there is none. On a normal game that is the human, but in a
 * game where nation 0 is a computer player the save is labelled with the wrong leader.
 *
 * THE POPUP IS RESET BY ASSIGNMENT. `p = Popup(0)` builds a second, empty popup and copies it over
 * the first one -- and because the class has no `operator=` of its own, Borland writes the copy out
 * member by member: 1,120 bytes of `MOV` pairs, three `REP MOVSW` runs for the sixteen-bit arrays,
 * one `F_SCOPY` for the 256-byte pointer array, and the GRPort sub-object copied LAST because it is
 * the only member with a generated assignment of its own. That one sequence is what fixes the whole
 * class layout, and it is why this function was parked until now.
 *
 * WHAT THE LAYOUT SAYS: the class is two longs, a GRPort at +8, two Sprites at +0x2e and +0x4e, a
 * long, twenty-nine words, the four sixty-four-entry line arrays at +0xac, +0x1ac, +0x22c and
 * +0x2ac, twenty-seven more words, eight longs, two words, three longs, seven words, six longs, six
 * words and twelve words -- 0x3dc bytes in all. The GRPort's own first two bytes are NOT copied,
 * which is how we know it has a virtual function and a two-byte vtable pointer at offset 0.
 *
 * WHAT THE BYTES SETTLED: the exit is `goto out;` with an explicit `return;` at the label. Falling
 * off the end of a void function restores SS:0x14 through AX (`36 A3`), while an exit that is a
 * RETURN statement restores it through DX (`36 89 16`) -- one byte longer, and the only thing that
 * distinguishes the two spellings.
 *
 * RENAMED 2026-09-26 from report_save_outcome: The body is the whole Save Game command. It runs the slot menu popup, takes the chosen slot's name, writes the game with save_game, builds the load-menu description line for that slot, and then shows SAVEGOOD, SAVEMEM or SAVEERROR. The old name describes only the last step, as if the save had already happened elsewhere. The function actually picks the slot and performs the save.
 */

#include "colonization.h"

#include "control.h"
#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern GameState g_game;   /* SEG20:0x7782 */

extern int save_game(char *path);

extern void space_string(char *buf);
extern void open_string(char *s);
extern char *strings(int n);

extern void close_string(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void save_slot_file_name(char *dst, int slot);

void save_game_dialog(void)
{
    int answer;
    int result;
    int nation;
    int i;
    char name[0x50];
    char path[0x100];
    Popup p(0x800);

    answer = popup_run_thunk_1028_e46b(&p) - 1;
    if (answer < 0)
        goto out;
    {
        Popup t(0);
        p = t;
    }
    save_slot_file_name(name, answer);
    STRCPY(path, name);
    result = save_game(path);
    STRCPY(g_name_slots[0], name);
    if (result == 0) {
        nation = -1;
        for (i = 0; nation < 0 && i < 4; i++)
            if (g_nation_text[i].control == CONTROL_HUMAN)
                nation = i;
        if (nation < 0)
            nation = 0;
        name[0] = 0;
        say_string(name, g_difficulty_words[g_game.difficulty]);
        space_string(name);
        STRCAT(name, g_nation_text[nation].leader);
        space_string(name);
        say_string(name, g_misc_words[MISC_OF_THE]);
        space_string(name);
        say_string(name, g_nation_adjective[nation]);
        space_string(name);
        open_string(name);
        STRCAT(name, strings(g_season_word[g_game.season]));
        space_string(name);
        say_number(name, g_game.year);
        close_string(name);
        set_name_slot_1028_e412(1, name);
        show_text_dialog_1028_e48e((void*)("SAVEGOOD"));
    } else if (result == 2)
        show_text_dialog_1028_e48e((void*)("SAVEMEM"));
    else
        show_text_dialog_1028_e48e((void*)("SAVEERROR"));
out:
    return;
}

