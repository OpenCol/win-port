/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* THREE NAMES THAT COULD NOT LINK, corrected 2026-09-20 by
 * tools/check_global_addresses.py --link-safety. This file used ONE name for TWO
 * addresses twice over -- `g_main_window` for both 0x25e4 and 0x05ca, and
 * `g_selected_after` for both 0x4b16 and 0x0c24 -- and called 0x25e4
 * `g_colony_window`, which the rest of the corpus places at 0x25be. A data reference's
 * offset is a masked FIXUP, so all of it compiled to the shipped bytes and graded
 * RELOCATABLE; no binding of those names could reproduce the behaviour, which is what
 * a compile-and-link would have needed. The names now follow the addresses the bytes
 * prove, each confirmed elsewhere: g_colony_win_rec (23 files), g_main_win (74),
 * g_colony_c24 (5), g_info_font_large (18).
 *
 * The font at 0x4b16 reaching Popup_set_font is odd and is left as the bytes say it:
 * that argument is spelt `ctl`, `owner` and `v` in three other files, so what it means
 * is not settled and this is not the place to settle it.
 *
 * RENAMED 2026-09-26 from clear_colonist_specialty: the Mac build's colony_select_profession(short), bound by call-graph agreement (tools/mac_callgraph.py): 12 of its Mac call edges already bound land on this function (caller KeyClick; callee grey; callee help_me; callee hilite; callee item), the runner-up gets 7, and the arity agrees (1). The old name came from the popup's LOBOTOMIZE item, one line of what the body builds: the colony screen's whole colonist-job popup.
 */

#include "colonization.h"

/* PORT: this file's own Font, Screen, GRPort, Sprite, DialogWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
/* colony_select_profession: candidate for 1010:c98c (3568 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_select_profession
 *
 *
 * A FIELD THIS FILE USED TO MISNAME. Unit+2 was declared `specialty` here, which is what
 * Unit+0x17 holds in every other file and in units.h; offset 2 is the unit TYPE. The two
 * literals settle it. `final_job == JOB_SOLDIER` stores 9 and `final_job == JOB_DRAGOON`
 * stores 7, and units.h has UNIT_CONTINENTAL_ARMY 9 and UNIT_CONTINENTAL_CAVALRY 7 -- a
 * soldier promoting to Continental Army and a dragoon to Continental Cavalry. Read as a
 * specialty the same pair would be a distiller and a silver miner, which nothing pairs.
 * The field is now `type`, and 9 and 7 are spelled as the unit types they are.
 *
 * `final_job` HOLDS A JOB, which the sweep cannot prove and a person can. It is assigned from
 * `l.job` (proven: `get_job`) on two paths and from `l.choice` on the third --
 * and `l.choice` is a POPUP RESULT whose space includes 0x61 and 0x62, the clear-specialty (lobotomise) and
 * MORE page-toggle items, which are not jobs. Both are filtered out before that assignment: 0x62
 * re-runs the popup and 0x61 takes the branch above it, so the only `l.choice` that reaches
 * `final_job` is a menu item that IS a job id. That is a control-flow argument, so
 * name_constants.py refuses the variable and these four names were written by hand.
 * `l.choice` itself is NOT named, for the reason that makes this note necessary.
 * Builds the colony's colonist-job popup.  Besides clearing a specialty, the menu
 * can move a colonist between outdoor and indoor jobs and opens the corresponding
 * Colonopedia entry on an alternate click.  It temporarily credits the current
 * job's inputs back to colony stock while computing every alternative, then removes
 * those credits before applying the choice.
 *
 * Four `asm db` statements retain Borland's exact control-flow spelling: two are
 * zero-distance jumps at structured joins and one is the pair of short exits around
 * the Colonopedia loop.  The surrounding statements express their C semantics.
 */
#include "fathers.h"
#include "goods.h"
#include "units.h"
#include "jobs.h"
#include "building.h"
#include "labels.h"
#include "misc.h"
#include "w16types.h"
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e + 58 * (sizeof(void *) - 4)) ? 1 : -1];   /* PORT: +4 a pointer before it on 64-bit (W9) */


#pragma option -a

#pragma option -a-
#include "records.h"
class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern Nation g_nations[];   /* SEG20:0xc2f4 */

   /* DialogWin : EventWin, flattened for C */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;      /* SEG20:0x05ca */

extern char g_pedia_win;   /* SEG20:0x387c */

extern int get_specialty(int u);
extern void set_specialty(int c, int specialty);
extern "C" int get_job(int colonist);   /* PORT: C linkage, as defined (W2) */
extern int any_spec(int job);
extern int job_cargo(int *out, int job);
extern int nth_unit_with_job(int n);
extern int set_job(int colonist, int job);
extern int job_available(int job);
extern "C" int colonist_find(int colonist, int *x, int *y);   /* PORT: C linkage, as defined (W2) */
extern void pick_up_colonist(int id);
extern int compute_yield(int x, int y, int *kind, int flag);
extern int compute_colonist_yield(int colonist, int *kind);
extern int preassign_colonist_to_field(int colonist, int job);
extern "C" void set_field(int x, int y, int colonist);   /* PORT: C linkage, as defined (W2) */
extern int has_building(int bit);
extern int place_in_stack(int u);
extern int has_father(int nation, int bit);

extern char *strings(int n);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);
extern void colon_string(char *buf);
extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern void Popup_set_width(Popup *popup, int width);
extern void Popup_set_font(Popup *popup, Font *ctl);
extern "C" void Popup_string(Popup *popup, char *s);   /* PORT: C linkage, as defined (W2) */
extern void Popup_grey(Popup *p, int key, int on);
extern void Popup_hilite(Popup *p, int key, int on);
extern void Popup_set_active(Popup *popup, int key);
extern int Popup_help_me(Popup *popup);
extern void popup_say(int slot, int word);
extern "C" int popm(char *name, int style);   /* PORT: C linkage, as defined (W2) */
extern "C" void PediaWin_ShowModal(void *pedia, int category, int item, void *parent);   /* PORT: C linkage, as defined (W2) */

extern "C" void win_disable_1010_e459(void *p);   /* PORT: C linkage, as defined (W2) */

extern void colony_cursor_update(void);

extern "C" int try_to_set_job(int c, int job);   /* PORT: C linkage, as defined (W2) */
extern void colony_change_selected(void);
extern "C" void win_enable_1010_e4d4(void *p);   /* PORT: C linkage, as defined (W2) */

typedef struct {
    char text[0x50];                  /* BP-166 */
    unsigned char input_amount[16];  /* BP-116 */
    int stock_delta[16];             /* BP-106 */
    int job_good[25];                /* BP-e6 */
    int job_amount[19];              /* BP-b4 */
    char pad_d8[0x0c];
    int cell_amount[9];              /* BP-82 */
    Popup *popup;                /* BP-70 */
    int inputs[9];                   /* BP-6c */
    char cell_kind[2];               /* BP-5a */
    int unit;                        /* BP-58 */
    int cell_y;                      /* BP-56 */
    int cell_x;                      /* BP-54 */
    int job_is_teacher;              /* BP-52 */
    int remaining;                   /* BP-50 */
    int skip;                        /* BP-4e */
    int cursor;                      /* BP-4c */
    int page;                        /* BP-4a */
    int per_page;                    /* BP-48 */
    int selected_specialty;          /* BP-46 */
    int pages;                       /* BP-44 */
    int input_count;                 /* BP-42 */
    int old_job;                     /* BP-40 */
    int specialty;                   /* BP-3e */
    int job;                         /* BP-3c */
    int available;                   /* BP-3a */
    int range_count;                 /* BP-38 */
    int range_start;                 /* BP-36 */
    int amount;                      /* BP-34 */
    int verdict;                     /* BP-32 */
    int choice;                      /* BP-30 */
    int final_job;                   /* BP-2e */
} ClearLocals;

extern "C" void colony_select_profession(int unit_roles_only)
{
    ClearLocals l;
    register int i;
    register int j;

    l.inputs[6] = 0;
    l.inputs[4] = 0;
    l.inputs[3] = 0;
    l.popup = 0;
    for (;;) {
outer_top:
        win_disable_1010_e459(&g_colony_win_rec);
        MSDoAllTasks_1010_e5bd();
        if (g_cur_colony_rec->pop <= g_selected_colonist &&
            (0x20 <= *(char *)((char *)g_cur_colony_rec + 0x1f))) {
            unit_roles_only = 1;
            l.inputs[6] = 1;
        }
        l.inputs[7] = 1;
        if (any_spec(get_specialty(g_selected_colonist)) == 0)
            l.inputs[7] = 0;
        g_panel_toggle = 0;
        colony_cursor_update();
        if (unit_roles_only != 0) {
            l.range_start = 0x13;
            l.range_count = 6;
        } else {
            l.range_start = 0;
            l.range_count = 0x19;
        }
        l.job = get_job(g_selected_colonist);
        l.job_is_teacher = (l.job == JOB_TEACHER);
        memset(l.stock_delta, 0, 0x20);
        l.input_count = job_cargo(l.inputs, l.job);
        for (i = 0; i < l.input_count; i++) {
            if (l.inputs[i] == 0xe) {
                l.unit = nth_unit_with_job(g_selected_colonist - g_cur_colony_rec->pop);
                l.stock_delta[14] += (unsigned char)g_units[l.unit].hold.w.tools;
            } else {
                if (l.inputs[i] == 0)
                    continue;
                l.stock_delta[l.inputs[i]] += 0x32;
            }
        }
        for (i = 0; i < 0x10; i++)
            g_cur_colony_rec->stock[i] += l.stock_delta[i];

        memset(l.input_amount, 0, 0x10);
        l.amount = g_cur_colony_rec->stock[GOODS_TOOLS] / 0x14;
        l.input_amount[14] = minimax(l.amount * 0x14, 0x14, 100);
        l.input_amount[8] = 0x32;
        l.input_amount[15] = 0x32;
        if (unit_roles_only != 0 && l.job < l.range_start)
            l.available++;
        l.available = 0;
        for (i = l.range_start; i < l.range_start + l.range_count; i++) {
            if (job_available(i) != 0 || i == l.job)
                l.available++;
        }
        l.inputs[8] = 0;
        if (g_cur_colony_rec->pop <= g_selected_colonist &&
            (l.job == JOB_SOLDIER || l.job == JOB_DRAGOON)) {
            l.unit = nth_unit_with_job(g_selected_colonist - g_cur_colony_rec->pop);
            if (g_units[l.unit].type == UNIT_CONTINENTAL_ARMY || g_units[l.unit].type == UNIT_CONTINENTAL_CAVALRY)
                l.inputs[8] = 1;
        }
        g_menu_active = 1;
        l.inputs[5] = -1;
        if (g_cur_colony_rec->pop <= g_selected_colonist) {
            l.inputs[5] = g_selected_colonist - g_cur_colony_rec->pop;
            l.unit = nth_unit_with_job(l.inputs[5]);
            l.inputs[5] = place_in_stack(l.unit);
        }

        if (l.inputs[6] == 0) {
            if (colonist_find(g_selected_colonist, &l.cell_x, &l.cell_y) != 0) {
                l.inputs[4] = 1;
                for (i = 0; i <= 8; i++) {
                    g_selected_colonist = set_job(g_selected_colonist, i);
                    l.cell_amount[i] = compute_yield(l.cell_x, l.cell_y, (int *)l.cell_kind, 0);
                }
            }
            for (i = 0; i < 0x13; i++) {
                pick_up_colonist(g_selected_colonist);
                g_selected_colonist = set_job(g_selected_colonist, i);
                if (i < 9) {
                    l.job_good[i] = i < 8 ? i : 0;
                    if (preassign_colonist_to_field(g_selected_colonist, i) != 0)
                        l.job_amount[i] = 0;
                    else
                        l.job_amount[i] = g_best_job_amount;
                } else {
                    l.job_amount[i] = compute_colonist_yield(g_selected_colonist, &l.job_good[i]);
                    if (i == JOB_STATESMAN) {
                        if (has_father(g_cur_colony_rec->nation, FATHER_THOMAS_JEFFERSON) != 0)
                            l.job_amount[i] += l.job_amount[i] >> 1;
                        if (has_father(g_cur_colony_rec->nation, FATHER_THOMAS_PAINE) != 0)
                            l.job_amount[i] += g_nations[g_cur_colony_rec->nation].tax * l.job_amount[i] / 100;
                        if (has_building(BUILDING_NEWSPAPER) != 0)
                            l.job_amount[i] <<= 1;
                        else if (has_building(BUILDING_PRINTING_PRESS) != 0)
                            l.job_amount[i] += l.job_amount[i] >> 1;
                    }
                }
            }
        }
        if (l.inputs[4] != 0)
            set_field(l.cell_x, l.cell_y, g_selected_colonist);
        if (l.job == JOB_PIONEER) {
            g_cur_colony_rec->stock[GOODS_TOOLS] -= l.stock_delta[14];
            l.stock_delta[14] = 0;
        }
        g_job_popup_active = 1;
        g_selected_colonist = set_job(g_selected_colonist, l.job);
        g_job_popup_active = 0;

        l.page = 0;
        l.pages = 1;
        l.per_page = 0x19;
        if (l.available > 0xd) {
            if (l.available > 0x10) {
                l.pages = 2;
                l.per_page = 0x10;
            }
            ; /* PORT: `asm db 0xeb, 0x00` (jmp $+2, a no-op) removed (W1) */
        }
        do {
            if (l.popup != 0)
                delete l.popup;
            l.popup = new Popup(0x800);
            l.popup->flags |= 1;
            Popup_set_width(l.popup, 300);
            Popup_set_font(l.popup, &g_info_font_large);

            l.text[0] = 0;
            say_string(l.text, g_ctitle_word[CTITLE_SELECT_A_PROFESSION_FOR]);
            l.specialty = get_specialty(g_selected_colonist);
            space_string(l.text);
            if (l.specialty == JOB_COLONIST_ALT)
                l.specialty = 0x13;
            say_string(l.text, g_spec_recs[l.specialty].expert);
            l.old_job = get_job(g_selected_colonist);
            if (l.old_job != l.specialty && l.old_job != JOB_COLONIST) {
                open_string(l.text);
                say_string(l.text, g_spec_recs[l.old_job].name);
                close_string(l.text);
            }
            colon_string(l.text);
            Popup_string(l.popup, l.text);

            l.skip = l.page * l.per_page;
            l.cursor = l.range_start;
            while (l.skip > 0) {
                while (job_available(l.cursor) == 0 && l.cursor != l.job)
                    l.cursor++;
                l.cursor++;
                l.skip--;
            }
            while (job_available(l.cursor) == 0 && l.cursor != l.job)
                l.cursor++;
            i = l.cursor;
            l.remaining = l.per_page;

            if (l.pages > 1 && l.page > 0)
                Popup_item(l.popup, strings(g_ctitle_word[CTITLE_MORE]), 0x62);
            if (l.inputs[7] != 0 && l.page == 0 && unit_roles_only == 0)
                Popup_item(l.popup, strings(g_misc_words[MISC_CLEAR_SPECIALTY]), 0x61);
            if (unit_roles_only != 0 && l.page == 0 && l.job < l.range_start) {
                l.text[0] = 0;
                say_string(l.text, g_spec_recs[l.job].name);
                Popup_item(l.popup, l.text, l.job);
                if (l.job == l.selected_specialty)
                    Popup_hilite(l.popup, l.job, 1);
                Popup_set_active(l.popup, l.job);
            }

            do {
                l.verdict = job_available(i);
                if (i == l.job)
                    l.verdict = -2;
                if (l.verdict != 0) {
                    l.text[0] = 0;
                    say_string(l.text, g_spec_recs[i].name);
                    strcat(l.text, "   ");
                    if (i < 0x13 && i != 0x12) {
                        strcat(l.text, " = ");
                        if (l.inputs[4] != 0 && i <= 8) {
                            say_number(l.text, l.cell_amount[i]);
                            strcat(l.text, "/");
                        }
                        say_number(l.text, l.job_amount[i]);
                        space_string(l.text);
                        say_string(l.text, g_goods_name_word[l.job_good[i]]);
                    }
                    l.input_count = job_cargo(l.inputs, i);
                    if (l.input_count >= 0 && i != l.job) {
                        for (j = 0; j < l.input_count; j++) {
                            l.amount = l.input_amount[l.inputs[j]];
                            space_string(l.text);
                            open_string(l.text);
                            say_number(l.text, l.amount);
                            space_string(l.text);
                            strcat(l.text, strings(g_goods_name_word[l.inputs[j]]));
                            close_string(l.text);
                        }
                    }
                    Popup_item(l.popup, l.text, i);
                    l.selected_specialty = get_specialty(g_selected_colonist);
                    if (l.verdict == -1)
                        Popup_grey(l.popup, i, 1);
                    else if (i == l.selected_specialty || (i == 0x17 && l.selected_specialty == JOB_SOLDIER)) {
                        Popup_hilite(l.popup, i, 1);
                        ; /* PORT: `asm db 0xeb, 0x00` (jmp $+2, a no-op) removed (W1) */
                    }
                    if (i == l.specialty && l.verdict != -1)
                        Popup_set_active(l.popup, i);
                    l.remaining--;
                }
                i++;
            } while (l.remaining > 0 && i < 0x19);

            if (l.pages > 1 && l.page == 0)
                Popup_item(l.popup, strings(g_ctitle_word[CTITLE_MORE]), 0x62);
            l.popup->flags |= 0x40;
            l.choice = popup_run_thunk_1010_e4b1(l.popup);
            l.choice--;
            if (l.choice == 0x62)
                l.page = 1 - l.page;
            if (Popup_help_me(l.popup) != 0 && l.choice == 0x61)
                l.choice = 0x62;
        } while (l.choice == 0x62);

        /* PORT: the two exits were `asm db` bytes -- 0b c0 74 58 (or ax,ax; jz) and
           83 7e d0 00 7c 48 (cmp [bp-0x30],0; jl), both to 1010:d57b, the first instruction
           after this for (;;), read from the shipped code. In C they are breaks (W1). */
        if (Popup_help_me(l.popup) == 0)
            break;
        l.popup->flags &= 0xff7f;
        if (l.choice < 0)
            break;
        for (i = 0; i < 0x10; i++)
            g_cur_colony_rec->stock[i] -= l.stock_delta[i];
        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, l.choice, &g_colony_win_rec);
    }

    for (i = 0; i < 0x10; i++)
        g_cur_colony_rec->stock[i] -= l.stock_delta[i];
    g_menu_active = 0;
    if (l.choice == 0x61) {
        popup_say(0, g_spec_recs[l.selected_specialty].expert);
        if (popm("LOBOTOMIZE", 5) == 1)
            set_specialty(g_selected_colonist, JOB_COLONIST_ALT);
        l.final_job = l.job;
    } else if (l.choice >= 0) {
        l.final_job = l.choice;
        if (try_to_set_job(g_selected_colonist, l.final_job) == 0 &&
            l.job >= JOB_INDOOR_FIRST && l.final_job < JOB_INDOOR_FIRST)
            preassign_colonist_to_field(g_selected_colonist, l.final_job);
    } else {
        l.final_job = l.job;
        g_selected_colonist = set_job(g_selected_colonist, l.job);
    }
    if (l.inputs[8] != 0 && (l.final_job == JOB_SOLDIER || l.final_job == JOB_DRAGOON)) {
        l.unit = nth_unit_with_job(g_selected_colonist - g_cur_colony_rec->pop);
        if (l.final_job == JOB_SOLDIER)
            g_units[l.unit].type = UNIT_CONTINENTAL_ARMY;
        else
            g_units[l.unit].type = UNIT_CONTINENTAL_CAVALRY;
    }
    colony_change_selected();
    if (l.inputs[3] != 0)
        g_colony_c24 = g_selected_colonist;
    else if (l.final_job >= JOB_COLONIST)
        g_colony_c24 = -1;
    win_disable_1010_e459((Screen *)&g_main_win);
    win_enable_1010_e4d4(&g_colony_win_rec);
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
    g_menu_active = 0;
    if (l.popup != 0)
        delete l.popup;
}
