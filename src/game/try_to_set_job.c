/* PORT: this file's own declarations of village_bribe removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 122, 125, 218 of the imported file. */
/* try_to_set_job: candidate for 1010:c182 (1917 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: try_to_set_job
 * name-source: sibling-build
 * size-source: 1917, not the census 1000. Ghidra stopped at the switch's indirect jump; the code runs
 * to the RETF at 1010:c8fe, and the 22-entry jump table follows at 1010:c900 after one pad byte.
 *
 * Puts a colonist to work at a job -- the colony window's one routine for it, called by every panel's
 * drop handler (`ColonyWin_ColonyMouseMap`, `ColonyWin_ColonyMouseBuildings`,
 * `colony_map_click`) -- and returns 0 when it happened, 1 when it did not.
 *
 * FIRST, ONE OF TWENTY-TWO OBJECTIONS. `job_change_legal` names what is wrong with the move and a
 * switch turns the code into the game's own message: KEEPSTOCKADE, MORETHANTHREE, ONE_ENTER/ONE_LEAVE,
 * FULL, ABANDON (with a "2" variant for a nation with fewer than two colonies after 1575), GRADUATE,
 * DROPOUT/NOMATCH/NOSCHOOL, UNIV3, COLLEGE2, SCHOOL1, NOTEACHER, NEEDCOLLEGE, NEEDUNIVERSITY,
 * ASSEMBLY, TOWNHALL, MEETINGHALL, NODOCKS and SIEGE. Five of them are questions -- ABANDON, GRADUATE
 * and the three school warnings -- and answering the first choice clears the objection; the rest only
 * inform, and the move does not happen. Code 19 has no message.
 *
 * THEN THE LAND. With a pending square (DGROUP:0xa10, the cell at 0x4b9a/0x4b9c) that a tribe still
 * claims, and with nation-pair flag 0x40 set between them, the tribe speaks -- INDIANLAND, "You are
 * trespassing on {tribe land}. We patiently ask that you leave immediately." -- and offers three
 * answers: "Very well, we shall respect your wishes." (1) abandons the move; "We offer you N for this
 * land." (2) pays the tribe's price, bumps the tribe's count, clears the claim, marks the square and
 * shows INDIANBRIBE, "Very well, we withdraw our objection."; and "You are mistaken; this is OUR land
 * now!" (3) falls through and takes the square anyway -- `set_field` raises the
 * tribe's alarm when it lands. The offer line is greyed when the treasury is short of the price.
 *
 * Then the move (`set_job`), a sound when a missionary or a preacher (JOB_PREACHER) lands in the job it is
 * best at, the one-time TUTORIAL7 note once a colony without a stockade reaches three colonists, and
 * the selection updated to the moved colonist.
 *
 * Three rules shaped it: the `Popup` needs its real sub-objects (RE_STATUS 349); every scalar,
 * `r = 1` first, is declared before the `Popup`, because the shipped stores `r` before constructing
 * it (rule 72); and `(int)` on the population byte (rule 51). The case arms are written in the order
 * of their addresses: 21, 22, 2, 1, 4, 3, 5, 6/14/15, 7, 8, 9, 16, 17, 18, 12, 11, 10, 13, 20.
 *
 * RENAMED 2026-09-26 from colony_assign_job: the Mac build's try_to_set_job(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller colony_select_profession; callee grey; callee popup_say; callee sound_play), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "tutflag.h"
#include "building.h"
#include "gameflag.h"
#include "jobs.h"
#include "sounds.h"
#include "misc.h"
#include "w16types.h"
/* PORT: this file's own GRPort, Sprite, DialogWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_popup_tribe;                       /* DGROUP:0x2518, declared */

   /* DialogWin : EventWin, flattened for C */

extern int get_job(int colonist);
extern int get_specialty(int u);
extern int job_change_legal(int colonist, int job);
extern int popm(char *msg, int n);

extern char *strings(int n);
extern void popup_say(int slot, int word);
extern int get_treaty(int nation_a, int nation_b);

extern int find_close_village(int x, int y, int nation, int region);
extern void set_tribe(int tribe);
extern long check_gold(int nation);
extern void Popup_grey(Popup *p, int id, int on);
extern void subtract_gold(int nation, long amount);
extern void set_indian(int x, int y, int bits);
extern int popi(char *msg, int tribe);
extern void pick_up_colonist(int id);
extern int set_job(int colonist, int job);
extern void sound_play(int n);
extern int has_building(int bit);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_disable_1010_e459(void *o);
extern "C" void win_enable_1010_e4d4(void *o);

extern "C" void set_text_arg_i_1010_ebb0(int slot, int v);

extern "C" int try_to_set_job(register int colonist, register int job)
{
    int r = 1;
    int newc;
    int warn;
    int spec;
    int claim;
    int jobname;
    int answer;
    int price;
    int tx;
    int ty;
    int oldjob;
    char buf[0x50];
    Popup popup(0x800);

    win_disable_1010_e459(&g_colony_win_rec);
    oldjob = get_job(colonist);
    spec = get_specialty(colonist);
    jobname = job;
    if (jobname == 0x17)
        jobname = 0x15;
    warn = job_change_legal(colonist, job);
    switch (warn) {
    case 21:
        popm("KEEPSTOCKADE", 5);
        break;
    case 22:
        popm("MORETHANTHREE", 5);
        break;
    case 2:
        show_text_dialog_1010_e528((void*)("ONE_ENTER"));
        break;
    case 1:
        show_text_dialog_1010_e528((void*)("ONE_LEAVE"));
        break;
    case 4:
        set_name_slot_1010_e658(0, g_cur_colony_rec->name);
        popm("FULL", 5);
        break;
    case 3:
        STRCPY(g_name_slots[0], g_cur_colony_rec->name);
        STRCPY(buf, "ABANDON");
        if (g_nation_colony_count[g_cur_colony_rec->nation] < 2 && g_game.year > 0x627)
            STRCAT(buf, "2");
        if (popm(buf, 5) == 1)
            warn = 0;
        break;
    case 5:
        spec = get_specialty(colonist);
        if (spec == JOB_COLONIST_ALT)
            STRCPY(buf, strings(g_misc_words[MISC_NONE]));
        else
            STRCPY(buf, strings(g_spec_recs[spec].name));
        STRUPR(buf);
        STRCPY(g_name_slots[0], buf);
        STRCPY(buf, strings(g_spec_recs[jobname].name));
        STRUPR(buf);
        STRCPY(g_name_slots[1], buf);
        if (popm("GRADUATE", 5) == 1)
            warn = 0;
        break;
    case 6:
    case 14:
    case 15:
        STRCPY(buf, strings(g_spec_recs[jobname].name));
        STRUPR(buf);
        STRCPY(g_name_slots[0], buf);
        STRCPY(buf, "DROPOUT");
        if (warn == 14)
            STRCPY(buf, "NOMATCH");
        if (warn == 15)
            STRCPY(buf, "NOSCHOOL");
        if (popm(buf, 5) == 1)
            warn = 0;
        break;
    case 7:
        popm("UNIV3", 5);
        break;
    case 8:
        popm("COLLEGE2", 5);
        break;
    case 9:
        popm("SCHOOL1", 5);
        break;
    case 16:
        popm("NOTEACHER", 5);
        break;
    case 17:
        spec = get_specialty(colonist);
        popup_say(0, g_spec_recs[spec].name);
        popm("NEEDCOLLEGE", 5);
        break;
    case 18:
        spec = get_specialty(colonist);
        popup_say(0, g_spec_recs[spec].name);
        popm("NEEDUNIVERSITY", 5);
        break;
    case 12:
        popm("ASSEMBLY", 5);
        break;
    case 11:
        popm("TOWNHALL", 5);
        break;
    case 10:
        popm("MEETINGHALL", 5);
        break;
    case 13:
        popm("NODOCKS", 0);
        break;
    case 20:
        popm("SIEGE", 1);
        break;
    }
    if (warn != 0)
        goto done;
    if (g_pending_flag != 0) {
        claim = g_tile_tribe[g_pending_col][g_pending_row];
        if (claim >= 0 && (get_treaty(claim, g_cur_colony_rec->nation) & 0x40)) {
            popup_say(0, get_adjective(claim));
            tx = g_cur_colony_rec->x + g_pending_col - 2;
            ty = g_cur_colony_rec->y + g_pending_row - 2;
            find_close_village(tx, ty, -1, -1);
            set_tribe(claim - 4);
            price = village_bribe(g_cur_settlement, g_cur_colony_rec->nation, tx, ty);
            set_text_arg_i_1010_ebb0(1, price);
            g_popup_tribe = g_cur_tribe;
            show_plain_dialog_1010_ec06(&popup, (void*)("INDIANLAND"), 0);
            if (check_gold(g_cur_colony_rec->nation) < (long)price)
                Popup_grey(&popup, 1, 1);
            answer = popup_run_thunk_1010_e4b1(&popup);
            if (answer == 1)
                goto done;
            if (answer == 2) {
                subtract_gold(g_cur_colony_rec->nation, (long)price);
                g_cur_tribe_rec->count++;
                g_tile_tribe[g_pending_col][g_pending_row] = -1;
                set_indian(g_pending_col, g_pending_row, 1);
                popi("INDIANBRIBE", g_cur_tribe);
            }
        }
    }
    if (job >= 9 && g_cur_colony_rec->pop > colonist)
        pick_up_colonist(colonist);
    newc = set_job(colonist, job);
    if ((job == JOB_MISSIONARY || job == JOB_PREACHER) && job == spec)
        sound_play(SOUND_AMEN);
    if ((g_game.game_flags & GAMEFLAG_TUTORIAL) && !(g_game.hint_flags & TUT_SHOWN_7) && (int)g_cur_colony_rec->pop >= 3
        && has_building(BUILDING_STOCKADE) == 0) {
        set_name_slot_1010_e658(0, g_cur_colony_rec->name);
        popm("TUTORIAL7", 1);
        g_game.hint_flags |= TUT_SHOWN_7;
    }
    if (oldjob >= JOB_COLONIST)
        g_colony_c24 = newc;
    else if (job >= 0x13)
        g_colony_c24 = -1;
    g_row_colonist = g_selected_colonist = newc;
    if (g_cur_colony_rec->pop == 0)
        g_open_flag = 0;
    r = 0;
done:
    g_pending_flag = 0;
    win_enable_1010_e4d4(&g_colony_win_rec);
    return r;
}
