/* SetCDCallback: candidate for 1068:a62f (22 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores the function pointer at DGROUP:0x569c -- the CD callback
 * `sound_init` registers.
 *
 * RENAMED 2026-09-26 from set_cd_done_cb: the Mac build's SetCDCallback(void(*)(void)*), bound by call-sequence alignment (1.0 aligned gaps in sound_init leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.82x this one.
 */

#include "colonization.h"

extern void (*g_cd_done_cb)(void);                /* DGROUP:0x569c */

void SetCDCallback(void *fn)
{
    g_cd_done_cb = (void (*)(void))fn;
}
