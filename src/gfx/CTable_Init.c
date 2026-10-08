/* PORT: the game's 16-bit words read and written as i16, not int (4 bytes here) (W4). */
/* CTable_Init: candidate for 1068:0180 (227 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: CTable_Init
 *
 * Loads a colour-table resource (type at DGROUP:0x2af2) into the palette record: the
 * resource is found (a miss is reported: "Error: Color resource not found - ", id) and
 * locked, its first two words give the first entry and the count, the RGB triples that
 * follow are set through CTable_Fill (a call TLINK converted), and the resource is
 * unlocked and freed. The module's resource wrappers are defined here because the shipped
 * calls to them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from load_color_resource: the Mac build's CTable::Init(unsigned short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between CTable_UpdatePal and CTable_SetAnimate, where the Mac gap holds 4 functions and the Windows one 8, so the pairing inside it is by size and arity (the Mac body is 0.69x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "classes.h"

extern void *MSGetResource(char *type4, int id);
extern void *MSLockResource(void *h);
extern void MSUnlockResource(void *h);
extern void MSReleaseResource(void *h);
struct logpal { char pad[1]; };
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern void *MSGetResource_1068_94c7(void *type, int id);

extern void report_error_1068_94ea(void *ctx, char *msg, long v);

extern void *MSLockResource_1068_94f8(void *h);

extern void MSUnlockResource_1068_952d(void *res);

extern void MSReleaseResource_1068_9549(void *res);

void CTable_Init(CTable *pal, int id)
{
    void *res;
    void *type;
    char *p;
    int first;
    int count;

    type = g_color_resource_type;
    res = MSGetResource_1068_94c7(&type, id);
    if (res == 0) {
        report_error_1068_94ea(&g_error_log, "Error: Color resource not found - ", (long)(unsigned)id);
        return;
    }
    p = (char *)MSLockResource_1068_94f8(res);
    first = swap_word_identity_1068_9518(*(i16 *)p);
    p += 2;
    count = swap_word_identity_1068_9518(*(i16 *)p);
    p += 2;
    CTable_Fill(pal, first, count, p);
    MSUnlockResource_1068_952d(res);
    MSReleaseResource_1068_9549(res);
}
