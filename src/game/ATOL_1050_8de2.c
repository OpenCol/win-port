/* ATOL_1050_8de2: candidate for 1050:8de2 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 32, not the census 28. The trailing bytes are the dead second epilogue.
 *
 * This module's -callable forwarder to `ATOL`, named for the routine it reaches, as
 * the rest of the wrapper layer is. It is one of a run of same-sized forwarders back to back
 * -- 1050:8dc2, 8de2, 8e02 and 8e1e -- which is what a module's private copy of a small resource API looks like.
 *
 * IT RETURNS AN int (2026-10-01). Its three callers -- Popup_start, expand_text_template and
 * popup_ask_number -- use the result as an int, and popup_ask_number's shipped code widens it with
 * `cwde` before storing a long: a long-returning callee would have been stored as DX:AX. The
 * forwarder's own bytes are the same either way (it leaves ATOL's AX where it is).
 *
 * RENAMED 2026-10-02 from atol: segment 11's copy of the ATOL forwarder family
 * (ATOL_1008_d87e, ...), and the bare name is the library routine its callers' ATOL is.
 */

#include "colonization.h"

int ATOL_1050_8de2(char *digits)
{
    return ATOL(digits);
}
