/* CursorClass_dtor: candidate for 1008:d436 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CursorClass::~CursorClass()
 *
 * The destructor of CursorClass: releases the cursor handle at +0 through destroy_cursor.
 */

#include "colonization.h"

#include "classes.h"

CursorClass::~CursorClass()
{
    destroy_cursor(hcursor);
}
