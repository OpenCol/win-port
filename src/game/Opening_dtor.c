/* Opening_dtor: candidate for 1010:1b29 (756 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening::~Opening()
 *
 * THE OPENING SCREEN'S DESTRUCTOR, and with it the class's layout. Almost all of the 756 bytes are the
 * compiler's: the vtable store, the null check, fifteen member destructions and the base's.
 *
 * The SOURCE statements are only these: stop the low-memory watch, clear the global at DGROUP:0x964 that
 * points at the object, unlock and then free the FOUR resource handles at +0x52e, +0x532, +0x53a and
 * +0x536 (in that order both times -- the third and fourth are the other way round from their addresses),
 * and clear three handler slots on the overlay window at DGROUP:0xc6.
 *
 * THE CLASS: two GRPorts, a CTable, the four handles, and then twelve Sprite members -- one alone and
 * eleven arrays of 0x12, 0xd, 0x36, 0x10, 0xe, 0x16, 8, 8, 0xb, 0xd and 0x15 sprites, which is the
 * opening's whole animation. The object is 0x1e88 bytes.
 *
 * `g_backdrop_win` is declared FAR here where `g_opening` is -- one module, two declarations (rule 9).
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

extern void DisableLowMemoryCheck(void);
extern EventWin * g_backdrop_win;        /* DGROUP:0xc6, declared ; its EventTable part sits at +0xd */
extern Opening *g_opening;             /* DGROUP:0x964 */

extern "C" void MSUnlockResource_1010_2d72(void *h);
extern "C" void MSReleaseResource_1010_2d8e(void *h);
extern "C" void set_on_lclick_1010_2c6b(void *table, void *fn);
extern "C" void set_on_rclick_1010_2ca4(void *table, void *fn);
extern "C" void set_on_keydown_after_1010_2cdd(void *table, void *fn);

Opening::~Opening()
{
    DisableLowMemoryCheck();
    g_opening = 0;
    MSUnlockResource_1010_2d72(ship_path_res);
    MSUnlockResource_1010_2d72(credit_times_res);
    MSUnlockResource_1010_2d72(monster_events_res);
    MSUnlockResource_1010_2d72(credit_spots_res);
    MSReleaseResource_1010_2d8e(ship_path_res);
    MSReleaseResource_1010_2d8e(credit_times_res);
    MSReleaseResource_1010_2d8e(monster_events_res);
    MSReleaseResource_1010_2d8e(credit_spots_res);
    set_on_lclick_1010_2c6b(&(EventTable &)*g_backdrop_win, 0);
    set_on_rclick_1010_2ca4(&(EventTable &)*g_backdrop_win, 0);
    set_on_keydown_after_1010_2cdd(&(EventTable &)*g_backdrop_win, 0);
}
