/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 47 of the imported file. */
/* PediaWin_OnScroll: candidate for 1060:31fc (111 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Colonopedia's scroll handler: turn to `page`, unless that is the page already showing.
 * `PediaWin_Show` and `PediaWin_ShowModal` both install this address into the window's
 * scrollbar at +0x7f0 -- the only two references to it are those two `PUSH SEG / PUSH
 * OFFSET` pairs, which is what says it is a callback and not a function anyone calls.
 *
 * The page number is kept twice, in the window at +0x852 and in DGROUP:0x28a6, and the
 * global is what the early-out compares against. `p->page = g_pedia_page = page` is a
 * CHAINED assignment: the shipped code loads the window pointer, then stores `SI` to the
 * global and to the field from the same `AX`. Written as two statements the pointer would
 * be reloaded in between.
 *
 * The `- 0x26` is the C++ base-class conversion of docs/findings/cpp-base-offset.md: -38 is
 * the one delta in the binary that is null-guarded at all 62 of its sites, and
 * `GetEventWindow` is the accessor all 62 of them call. The ternary is how that reaches C.
 *
 * `GetEventWindow` is a FAR call even though it lives in this same segment, so it is
 * `extern`; the two redraw helpers are reached by the four-byte `PUSH CS / CALL NEAR` form
 * and are stubs here (rule 47). The first of them checks the article id at +0x84c and
 * reaches `PediaWin_DrawArticle`; the second is `copy_rect_to_screen` over the whole window.
 *
 * RENAMED 2026-09-26 from pedia_set_page: the Mac build's PediaWin::OnScroll(short): the action PediaWin_Show installs on the scrollbar, taking the page. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void *GetEventWindow(void);     /* 1060:9fcd */

/* same object: 4-byte `push cs; call ` */
extern void PediaWin_DrawListArea(PediaWin *p);

void PediaWin_OnScroll(int page)
{
    PediaWin *p;
    void *w;

    w = GetEventWindow();
    p = (PediaWin *)(w != 0 ? (char *)w - PORT_DIALOG_BASE : (char *)w);
    if (page != g_pedia_page) {
        p->page = g_pedia_page = page;
        PediaWin_DrawListArea(p);
        copy_all_to_screen_1060_610c((GameWin *)(p));
    }
}
