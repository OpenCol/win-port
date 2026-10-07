/* GamePrefs_ctor: candidate for 1008:d16e (68 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GamePrefs::GamePrefs()
 *
 * The constructor of GamePrefs (0x19c bytes): loads the preferences. Allocation when
 * `this` is null and the constructed-object counter are the compiler's.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void GamePrefs_LoadPrefs(GamePrefs *p);

GamePrefs::GamePrefs()
{
    GamePrefs_LoadPrefs(this);
}
