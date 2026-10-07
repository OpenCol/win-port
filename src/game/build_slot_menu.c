/* build_slot_menu: candidate for 1028:d2c4 (2475 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P -a
 * symbol: build_slot_menu
 * name-source: descriptive
 *
 * THE SAVE-GAME SLOT MENU. Given a title and a number of slots it returns a finished `Popup` --
 * BY VALUE, which is why the tail of the function is a copy constructor rather than a `ret`: the
 * caller passes a hidden pointer at [bp+6] and the epilogue walks the whole 0x3dc-byte object into
 * it member by member. `save_game_dialog` is the caller that consumes it.
 *
 * WHAT EACH LINE SAYS. For every slot it builds the file name, reads the header with
 * `return_zero_1008_b239`, and on failure writes "(EMPTY)". On success the line is assembled from
 * six pieces in one sentence: the difficulty word, the leader's name, a joining word, the nation
 * adjective, a comma, the season word (header +0xc, the saved SEG20:0x778e) and finally the year (header +0xa,
 * the saved 0x778c). That is the one-line summary the player reads before choosing a slot --
 * "Discoverer Adam Smith of the Dutch, Spring 1592".
 *
 * WHICH LEADER IT SHOWS is the first of the four nation records whose byte at +0x31 is zero, and
 * zero if none matches. +0x31 of the 0x34-byte record at SEG20:0x7810 is CONTROL (0x7810 + 0x31 =
 * 0x7841 = g_nation_text[0].control), so this picks the first HUMAN nation -- the search
 * `save_game_dialog` makes with CONTROL_HUMAN.
 *
 * `g_slot_ok[i]` is set beside each line so the caller can tell a real save from an empty slot
 * without re-reading the header.
 *
 * THE RETRY LOOP IS DEAD. The body is wrapped in `do { stop = 0; ... } while (stop != 0 && ...)`
 * and nothing ever assigns `stop` a non-zero value, so the loop runs exactly once; the `pass`
 * counter that would have capped it at two attempts is likewise never read. The shipped code keeps
 * the whole scaffold -- the compare against `stop` guards the `for` as well -- which is why the
 * reconstruction has to keep it too.
 *
 * WHAT THE BYTES SETTLED: the `Sprite` inside the `Popup` is copied MEMBER BY MEMBER here, not with
 * `rep movsw`, and the copy maps its layout exactly -- twelve words, a byte at 0x18, a word at 0x1a
 * and a long at 0x1c. The gap at 0x19 is padding the copy skips, so this file needs `-a` (word
 * alignment) to place `handle`; without it Borland packs the word at 0x19 and each `Popup` comes out
 * two bytes short. The canonical class says the same (2026-10-02): no member at 0x19, declared
 * under `#pragma option -a` everywhere else. `matched/save_game_dialog.c` legitimately declares the same class as a plain
 * `char body[0x20]`, because there the Sprite is assigned with `rep movsw` and the members never
 * show (rule 9: one module, two declarations).
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern void space_string(char *buf);
extern void comma_string(char *buf);
extern char *strings(int n);

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void save_slot_file_name(char *dst, int slot);

Popup build_slot_menu(char *title, int count)
{
    int i;
    int nation;
    int j;
    int pass = 0;
    Popup p(0x800);
    int stop;
    char name[0x50];
    char text[0x50];
    SaveHeader hdr;
    char names[4][0x34];

    do {
        stop = 0;
        show_plain_dialog_1028_e43b(&p, title, 0);
        for (i = 0; stop == 0 && i < count; i++) {
            save_slot_file_name(name, i);
            if (return_zero_1008_b239(name, &hdr, names) != 0) {
                STRCPY(text, "(EMPTY)");
                g_slot_ok[i] = 0;
            } else {
                g_slot_ok[i] = 1;
                nation = -1;
                for (j = 0; nation < 0 && j < 4; j++)
                    if (names[j][0x31] == 0)
                        nation = j;
                if (nation < 0)
                    nation = 0;
                text[0] = 0;
                say_string(text, g_difficulty_words[hdr.level]);
                space_string(text);
                STRCAT(text, names[nation]);
                space_string(text);
                say_string(text, g_misc_words[MISC_OF_THE]);
                space_string(text);
                say_string(text, g_nation_adjective[nation]);
                comma_string(text);
                STRCAT(text, strings(g_season_word[hdr.era]));
                space_string(text);
                say_number(text, hdr.score);
            }
            Popup_item(&p, text, i + 1);
        }
        if (stop != 0)
            ;
        pass++;
    } while (stop != 0 && pass <= 1);
    return p;
}

