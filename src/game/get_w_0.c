/* get_w_0: candidate for 1060:afce (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * The word at +0 of a record -- the plainest of the widget layer's accessors, and the
 * counterpart of `get_w_2_*` and `get_w_4_*`. One of SIX copies.
 *
 * 24 bytes, not the census 20: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

#include "classes.h"

int get_w_0(MenuClass *r)
{
    return r->hmenu;
}
