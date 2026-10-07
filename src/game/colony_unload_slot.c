/* colony_unload_slot: candidate for 1018:88eb (72 bytes).  Verdict: RELOCATABLE.
 * symbol: colony_unload_slot
 *
 * Drops cargo slot `slot` of unit u into the current colony's stock of its
 * goods (the amount unload_cargo leaves at DGROUP:0x4bd2).  Returns
 * an uninitialised frame slot, as the shipped code does.  The helper is
 * -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int unload_cargo(int u, int slot);

int colony_unload_slot(int u, int slot)
{
    int result;
    register int goods;

    goods = unload_cargo(u, slot);
    if (goods < 0)
        goto done;
    g_cur_colony_rec->stock[goods] += g_cargo_amount;
done:
    return result;
}
