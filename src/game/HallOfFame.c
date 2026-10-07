/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 224 of the imported file. */
/* PORT: this file's own declarations of MSHideWindow_of_1050_8cd8 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GameWin_Init, file_MSOpenFile_1050_9096, font_height_1050_9106, text_search, win_center_1050_851f, win_show_and_focus_1050_911f removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 189, 236 of the imported file. */
/* HallOfFame: candidate for 1050:6f8e (2175 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: HallOfFame
 * name-source: original
 *
 * READS HALLFAME.DAT, INSERTS, SHOWS AND WRITES IT BACK -- the whole high-score table in one function,
 * called three ways: to show the table, to add an entry, or both. The file is five records of 42 bytes,
 * 0xd2 in all, read and written as one block into a six-record buffer.
 *
 * THE SIXTH RECORD IS SCRATCH. The clearing loop runs over all six but only clears a record when the
 * file did not load OR the index is 5, so a loaded file keeps its first five and always gets a blank
 * sixth. That sixth is where an entry too poor for the table lands, because the insertion test is
 * "this record's score is below the new one, OR this is index 5" -- index 5 always accepts. The
 * records below the insertion point shift down one, the new entry is copied in, and the caller's
 * pointer is cleared so the loop stops.
 *
 * THE DISPLAY is five rows of three lines each: the position, difficulty word, player name and nation;
 * then how the game ended -- independence granted, independence declared, or still a colony of its
 * mother country -- with the year and the rank; and then a "--- <score> % ---" banner centred. The
 * number shown against a row is the loop index, EXCEPT that row 4 is labelled 6 when the new entry
 * landed in the scratch record, so a player who just missed the table sees a sixth place that is not
 * really in it.
 *
 * With nothing drawn at all it says "No Entries Available" in the middle of the window.
 *
 * IT ALWAYS REWRITES THE FILE, even when it was only asked to show the table and nothing changed --
 * and if the file cannot be created it is not closed either, which is the one thing the two `open`
 * tests are for.
 *
 * WHAT THE BYTES SETTLED: the record has a ninth field at +0x20 that is only ever cleared and never
 * read, which is what makes it 42 bytes rather than 40. Both file results are tested in the
 * assignment -- `(open = MSOpenFile(...)) == 1` and `(open = MSCreateFile(...)) == 0` -- so the value
 * is tested in AL before it reaches the frame. And the create test is a `goto`, not an `if` around the
 * write: a short conditional over a short unconditional, rule 12.
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
/* PORT: this file's own Font, DiskFile, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "records.h"

#pragma option -a

#pragma option -a-


class DlgControl;



extern void * g_backdrop_win;           /* DGROUP:0xc6, declared */

   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;                 /* SEG20:0x5ca, by address */

extern void space_string(char *buf);
extern void comma_string(char *buf);
extern void colon_string(char *buf);
extern void period_string(char *buf);
extern void percent_string(char *buf);

extern char *strings(int n);

struct logpal;
extern int GRPort_LoadPicture(GRPort *w, int id, int a, int b, struct logpal *pal);
extern void EventWin_Modal(EventWin *w);
extern void fame_OnLMouseDown(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int read_file_1050_90bc(MSFile *file, void *buf, long n);


extern "C" void show_window_1050_8bc8(GameWin *w);
/* PORT: an in-file stub of set_on_lbutton_down_1050_8eb9 removed: src/game/set_on_lbutton_down_1050_8eb9.c defines it (W2). */
extern "C" void win_release_1050_9148(GameWin *w);

extern "C" void HallOfFame(FameEntry *entry, char show)
{
    register int i;
    register int j;
    int found;
    int idx;
    int loaded = 0;
    int x;
    int y;
    int x2;
    char open;
    char any;
    FameEntry rec[6];
    char buf[0x100];
    DiskFile f;
    GameWin w;

    any = 0;
    if ((open = file_MSOpenFile_1050_9096((MSFile *)&f, "HALLFAME.DAT")) == 1) {
        loaded = 1;
        if (read_file_1050_90bc((MSFile *)&f, rec, 0xd2L) == 0)
            loaded = 0;
        MSCloseFile_1050_90e6(&f);
        open = 0;
    }
    for (i = 0; i < 6; i++) {
        if (loaded == 0 || i == 5) {
            rec[i].name[0] = 0;
            rec[i].nation = -1;
            rec[i].raw_score = -1;
            rec[i].rating = -1;
            rec[i].difficulty = 0;
            rec[i].year = 0;
            rec[i].season = 0;
            rec[i].indep = 0;
            rec[i].granted = 0;
        }
    }
    found = -1;
    if (entry != 0) {
        for (i = 0; i < 6; i++) {
            if (rec[i].rating < entry->rating || i == 5) {
                for (j = 5; j > i; j--)
                    rec[j] = rec[j - 1];
                rec[i] = *entry;
                found = i;
                entry = 0;
                break;
            }
        }
    }
    if (show != 0 || g_backdrop_win != 0)
        GameWin_Init(&w, "", 0x800, 0, 0, 0x280, 0x1e0, &g_game_palette,
                                         (Screen *)g_backdrop_win);
    else
        GameWin_Init(&w, "", 0x800, 0, 0, 0x280, 0x1e0, &g_game_palette,
                                         (Screen *)&g_main_win);
    GRPort_LoadPicture(&w, 0xca, 0x10, 0x18, (struct logpal *)&g_game_palette);
    copy_all_to_screen_1050_843b(&w);
    win_center_1050_851f((DialogWin *)&w);
    y = 0x14;
    TextColor(0xc);
    GRPort_String(&w, (unsigned *)&g_report_font, strings(g_misc_words[MISC_COLONIZATION_HALL_OF_FAME]),
                         port_width_1050_8b8a(&w) / 2, y, 0);
    y = 0x32;
    x = 0x1e;
    for (i = 0; i < 5; i++) {
        idx = i;
        if (idx == 4 && found == 5)
            idx = 5;
        if (rec[i].nation < 0)
            continue;
        any = 1;
        y += 4;
        buf[0] = 0;
        say_number(buf, idx + 1);
        period_string(buf);
        say_string(buf, g_difficulty_words[rec[i].difficulty]);
        space_string(buf);
        STRCAT(buf, rec[i].name);
        space_string(buf);
        say_string(buf, g_misc_words[MISC_OF_THE]);
        space_string(buf);
        if (rec[i].indep != 0) {
            say_string(buf, g_misc_words[MISC_FREE]);
            space_string(buf);
        }
        say_string(buf, get_true_adjective(rec[i].nation));
        TextColor(0xfa);
        GRPort_String(&w, (unsigned *)&g_font_small, buf, x, y, 1);
        y += font_height_1050_9106(&g_font_small) + 2;
        x2 = x + 0xf;
        buf[0] = 0;
        if (rec[i].granted != 0) {
            say_string(buf, g_misc_words[MISC_PRESIDENT]);
            comma_string(buf);
            text_search((void*)("NAMES"), "INDEPENDENT", rec[i].nation);
            STRCAT(buf, g_text_buf);
        } else if (rec[i].indep != 0)
            say_string(buf, g_misc_words[MISC_GENERAL_CONTINENTAL_ARMY]);
        else {
            say_string(buf, g_misc_words[MISC_LEADER]);
            comma_string(buf);
            say_string(buf, get_true_adjective(rec[i].nation));
            space_string(buf);
            say_string(buf, g_misc_words[MISC_COLONIES]);
        }
        space_string(buf);
        say_string(buf, g_misc_words[MISC_TO]);
        space_string(buf);
        say_string(buf, g_misc_words[MISC_A_D]);
        space_string(buf);
        say_number(buf, rec[i].year);
        period_string(buf);
        say_string(buf, g_misc_words[MISC_SCORE]);
        colon_string(buf);
        say_number(buf, rec[i].raw_score);
        TextColor(0xfa);
        GRPort_String(&w, (unsigned *)&g_font_small, buf, x2, y, 1);
        y += font_height_1050_9106(&g_font_small) + 2;
        buf[0] = 0;
        STRCAT(buf, "--- ");
        say_string(buf, g_misc_words[MISC_COLONIZATION_RATING]);
        colon_string(buf);
        say_number(buf, rec[i].rating);
        percent_string(buf);
        STRCAT(buf, " ---");
        TextColor(0xfa);
        GRPort_String(&w, (unsigned *)&g_font_small, buf, port_width_1050_8b8a(&w) / 2, y, 0);
        y += font_height_1050_9106(&g_font_small) + 2;
    }
    if (any == 0) {
        TextColor(0xfa);
        GRPort_String(&w, (unsigned *)&g_font_small, "No Entries Available",
                             port_width_1050_8b8a(&w) / 2, port_height_1050_8ba9(&w) / 2, 0);
    }
    show_window_1050_8bc8(&w);
    win_show_and_focus_1050_911f((DialogWin *)&w);
    set_on_lbutton_down_1050_8eb9((EventTable *)&w, (void*)(fame_OnLMouseDown));
    copy_all_to_screen_1050_843b(&w);
    EventWin_Modal(&w);
    MSHideWindow_of_1050_8cd8((DialogWin *)&w);
    win_release_1050_9148(&w);
    if ((open = file_MSCreateFile_1050_9174(&f, (void*)("HALLFAME.DAT"))) == 0)
        goto close;
    write_file_1050_919a(&f, rec, 0xd2L);
close:
    if (open != 0)
        MSCloseFile_1050_90e6(&f);
}
