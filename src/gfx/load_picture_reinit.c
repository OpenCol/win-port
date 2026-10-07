/* PORT: this file's own declarations of grport_init_wh0 removed: the header declares it from the definition (W1 step 3). */
/* load_picture_reinit: candidate for 1068:363f (60 bytes).  Verdict: RELOCATABLE.
 *
 * `GRPort_LoadPicture` with 1068:974e run on the port first -- the variant
 * `MSInitImageButton` uses for a control whose byte +0x72 is set. The answer is the loader's.
 *
 * RENAMED 2026-09-26 from load_picture_974e: GRPort_LoadPicture after grport_init_wh0 re-initialises the
 * port; MSInitImageButton's path for a control whose +0x72 byte is set.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *port, int id, int first, int count, struct logpal *pal);

/* same object: 4-byte `push cs; call ` (rule 47) */

int load_picture_reinit(void *p, int id, int first, int count, void *pal)
{
    grport_init_wh0((GRPort *)p);
    return GRPort_LoadPicture((GRPort *)p, id, first, count, (struct logpal *)pal);
}
