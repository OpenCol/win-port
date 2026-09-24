/* PediaWin_DrawArticle -- shipped at 1060:23e3.
 *
 * Ported by tools/port.py from matched/game/PediaWin_DrawArticle.c.
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: article -> sel
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1060_65ca(void *port);
col_i16 get_w_4_1060_65e3(void *port);
void control_hide_1060_669f(void *bar);
void copy_all_to_screen_1060_610c(PediaWin *w);
void pedia_redraw(PediaWin *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
/* same object: 4-byte `push cs; call ` (rule 47) */





extern void PediaWin_DoCargoArticle(PediaWin *w);
extern void PediaWin_DoUnitArticle(PediaWin *w);
extern void PediaWin_DoTerrainArticle(PediaWin *w);
extern void PediaWin_DoJobArticle(PediaWin *w);
extern void PediaWin_DoBuildingsArticle(PediaWin *w);
extern void PediaWin_DoFathersArticle(PediaWin *w);

void PediaWin_DrawArticle(PediaWin *w)
{
    RECT r;

    if (w->sel == -1) {
        pedia_redraw(w);
        copy_all_to_screen_1060_610c(w);
        return;
    }
    SetRect(&r, 0, 0, get_w_2_1060_65ca(((char *)((unsigned char *)(w) + 0x818))), get_w_4_1060_65e3(((char *)((unsigned char *)(w) + 0x818))));
    CopyToPort(((char *)((unsigned char *)(w) + 0x818)), w, &r, &(*((RECT *)((unsigned char *)(w) + 0x76a))));
    control_hide_1060_669f(((char *)((unsigned char *)(w) + 0x7f0)));
    switch (w->kind) {
    case 0:
        PediaWin_DoCargoArticle(w);
        break;
    case 1:
        PediaWin_DoUnitArticle(w);
        break;
    case 2:
        PediaWin_DoTerrainArticle(w);
        break;
    case 3:
        PediaWin_DoJobArticle(w);
        break;
    case 4:
        PediaWin_DoBuildingsArticle(w);
        break;
    case 5:
        PediaWin_DoFathersArticle(w);
        break;
    }
    copy_all_to_screen_1060_610c(w);
}
