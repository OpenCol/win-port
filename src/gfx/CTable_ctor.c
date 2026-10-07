/* CTable_ctor: candidate for 1068:004e (157 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable::CTable()
 *
 * The constructor of CTable (0x41e bytes): palette_init on the object, the GDI palette
 * handle from create_palette stored at +0x404, the random seed, three bytes and three words
 * zeroed.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void palette_init(Palette *pal);

extern "C" void CTable_Reseed(CTable *t);

CTable::CTable()
{
    palette_init((Palette *)this);
    hpal = create_palette(this);
    CTable_Reseed(this);
    r = 0;
    g = 0;
    b = 0;
    hsrc_a = hsrc_b = hsrc = 0;
}
