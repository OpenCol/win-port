/* GamePrefs_dtor: candidate for 1008:d1b2 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GamePrefs::~GamePrefs()
 *
 * The destructor of GamePrefs (0x19c bytes): saves the preferences. The null check, the
 * constructed-object counter and the conditional delete are the compiler's.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void GamePrefs_SavePrefs(GamePrefs *p);

GamePrefs::~GamePrefs()
{
    GamePrefs_SavePrefs(this);
}
