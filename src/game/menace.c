/* menace: candidate for 1028:0d5d (151 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 151, not the census 142. The trailing nine bytes are the dead second
 * epilogue (rule 44), and the match holds over them.
 * symbol: menace
 *
 * Reports a kind back to the caller, or -1: kinds under four always, and the rest only when the caller asks and either the 78-word table clears 0x4b or the unit's own row clears 0x80.
 *
 * RENAMED 2026-09-26 from kind_if_known: the Mac build's menace(short, short, short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between find_beach and menaced, which are already bound; the arity agrees and the Mac body is 0.91x this one.
 */

#include "colonization.h"

#include "tribes.h"
#include "records.h"

extern int pissed(int tribe, int i);

int menace(int nation, int owner, int flag, int unit)
{
    int hit;
    int r = -1;
    int anger;
    int home;

    if (owner >= 4) {
        if (flag == 0)
            goto out;
        hit = 0;
        anger = pissed(owner - 4, nation);
        if (anger >= 0x4b)
            hit = 1;
        if (unit >= 0) {
            home = g_units[unit].home;
            if (g_settlements[home].anger[nation] >= SETTLEMENT_ANGER_HOSTILE)
                hit = 1;
        }
        if (hit == 0)
            goto out;
    }
    r = owner;
out:
    return r;
}
