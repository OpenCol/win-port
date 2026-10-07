/* king_grant_funds: candidate for 1048:694e (294 bytes).  Verdict: RELOCATABLE.
 *
 * The King demands money: nothing above 60% tax (a human player hears KINGNO);
 * otherwise the amount is 100 * max(1, 6 - (difficulty for a human) - (demands + 1) / 2),
 * stored at data segment 31 offset 0x2d4, offered to a human through KINGFUND (answer
 * 1 accepts), added to the treasury long at +0x2a of the current nation's record (DGROUP:0x4cc4), announced with
 * KINGTAX and a tax rise of difficulty / 2 + demands + 5, and the demand count (+5)
 * incremented; a human then gets europe_change_mode.
 *
 * RENAMED 2026-09-26 from king_tax_demand: Case 3 of king_meeting, the player's audience. Below 60% tax the King offers a GRANT (KINGFUND: 'we can offer you a grant of only N'); if accepted, the amount goes INTO the nation's treasury (gold += amount), and then taxes rise through KINGTAX with the grant counter incremented. Above 60% he refuses the request (KINGNO: 'we do not deign to fulfill your request'). The money flows from the Crown to the player, so 'tax demand' has the direction backwards and hides that this is a paid grant.
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern void raise_taxes(char *name, int delta);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void europe_change_mode(void);
void king_grant_funds(void)
{
    int delta;
    int human;
    int amount;

    human = g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN;
    if ((int)g_cur_nation_rec->tax >= 0x3c) {
        if (human)
            ask_text_resource("KINGNO");
        goto done;
    }
    amount = 6 - (((g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) ? g_game.difficulty : 0) + ((g_cur_nation_rec->grants_taken + 1) >> 1));
    amount = max_int_1048_dd61(1, amount);
    amount *= 100;
    g_text_args[0] = amount;
    if (human && ask_text_resource("KINGFUND") != 1)
        goto done;
    g_cur_nation_rec->gold += amount;
    delta = (g_game.difficulty >> 1) + g_cur_nation_rec->grants_taken + 5;
    raise_taxes("KINGTAX", delta);
    g_cur_nation_rec->grants_taken++;
    if (human)
        europe_change_mode();
done:
    return;
}
