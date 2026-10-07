/* free_boycott: candidate for 1048:573d (279 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Parliament's boycott of a good: an AI player just clears the boycott mask (+0x20 of the
 * market record) and returns 1. A human is told (KISSUP, with the good's and the
 * nation's name words and the arrears -- 500 times the buy price) and, on answer 2, either
 * cannot pay (KISSSORRY, with the treasury) or pays: the treasury (+0x2a) drops, the
 * long at +0x22 grows, the good's bit is cleared from the mask and europe_change_mode runs.
 *
 * RENAMED 2026-09-26 from parliamentary_boycott_notice: the Mac build's free_boycott(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller EuropeMouseUnits; callee europe_ask_price; callee europe_change_mode; callee popm; callee popup_say), the runner-up gets 3, the arity agrees (1), and the Mac body is 0.99x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern void popup_say(int slot, int word);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int europe_ask_price(int g);
extern void set_text_arg_i_1048_db6a(int slot, int v);
extern int popm(char *msg, int n);

extern void europe_change_mode(void);
int free_boycott(int good)
{
    int r;
    int cost;
    int result = 0;

    if (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_nation].control != CONTROL_HUMAN) {
        g_cur_nation_rec->boycott = 0;
        result = 1;
        goto done;
    }
    popup_say(0, g_goods_name_word[good]);
    popup_say(1, g_nation_name_word[g_cur_nation]);
    cost = europe_ask_price(good) * 500;
    set_text_arg_i_1048_db6a(0, cost);
    r = popm("KISSUP", 2);
    if (r != 2)
        goto done;
    if ((long)cost > g_cur_nation_rec->gold) {
        set_text_arg_l_1048_db8e(0, g_cur_nation_rec->gold);
        popm("KISSSORRY", 2);
    } else {
        g_cur_nation_rec->gold -= cost;
        g_cur_nation_rec->royal_gold += cost;
        g_cur_nation_rec->boycott &= ~(1 << good);
        europe_change_mode();
    }
done:
    return result;
}
