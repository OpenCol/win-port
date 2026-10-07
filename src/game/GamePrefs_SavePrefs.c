/* GamePrefs_SavePrefs: candidate for 1040:70fe (127 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GamePrefs_SavePrefs
 *
 * Writes the 0x19c-byte preferences record to COLWIN.PRF through a stack DiskFile (0x8c
 * bytes; constructed by 1068:9285, destroyed by DiskFile_dtor) after clearing the word at
 * +0xa. The exception frame around the local object is the compiler's.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
class DiskFile { public: MSFile msfile; DiskFile(); DiskFile(char *name, int mode); ~DiskFile(); };
typedef struct { long f0; short w; short h; short count; short use_default_layout; PlacementRec windows[32]; short slots[8]; } GamePrefs;
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void GamePrefs_SavePrefs(GamePrefs *p)
{
    DiskFile f;

    p->use_default_layout = 0;
    MSCreateFile_1040_aa10(&f, "COLWIN.PRF");
    write_file_1040_aa36(&f, p, 0x19cL);
}
