/* CTable_dtor: candidate for 1068:00eb (149 bytes, 11 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable::~CTable()
 *
 * The destructor of CTable, the colour-table class whose first 0x404 bytes are a
 * 256-entry LOGPALETTE: releases the palette handle at +0x404 and disposes the three
 * memory handles at +0x418, +0x41a, +0x41c when set.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void palette_delete(unsigned hpal);   /* 1070:0471 */

CTable::~CTable()
{
    palette_delete(hpal);
    if (hsrc_a != 0)
        MSDisposeHandle(hsrc_a);
    if (hsrc_b != 0)
        MSDisposeHandle(hsrc_b);
    if (hsrc != 0)
        MSDisposeHandle(hsrc);
}
