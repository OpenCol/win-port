/* MapWin_PieceIconDraw: candidate for 1038:a99f (321 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_PieceIconDraw
 * name-source: sibling-build
 *
 * Draws a unit sprite's marks at tile pixel (x, y) with the sprite bank
 * switched to zoom + 8: flag 8 a coloured copy two pixels right, flag 1
 * one two pixels left (colour 0x5f with flag 4, else the byte at
 * DGROUP:0x337e), flag 2 the sprite itself; the bank goes back to 1.
 *
 * RENAMED 2026-09-27 from draw_unit_marks: CONST PROPOSAL, SIBLING RESOLVED. The witness points both draw_marker (a849) and draw_unit_marks (a99f) at MapWin::PieceIconDraw(Sprite&, short, short, short), CODE 23 0x1318, 300 bytes. The constants tie (the same 4 values and overlap 1.0 for both), so they cannot decide. Arity does: the Mac method takes a Sprite&, a99f takes (MapWin *w, void *sprite, int x, int y, int flags), and a849 takes an int sprite INDEX. Call chain: the Mac method's only caller is the free thunk PieceIconDraw(Sprite&, s, s, s) (CODE 22 0x1d2c, 66 bytes), which is called 5 times by PieceDraw(short x6). a99f's only caller is the thunk map_draw_unit_marks (1040:20fb, 43 bytes, void *sprite), which is called 5 times by PieceDraw_noport (int x6). Callees: SetGlobalSpriteScale x2, SetClip, DrawMono x2/Sprite_DrawMono x2, Draw, ResetClip on both. Size 300 against 321. Layout: the Mac has ONE method between RedrawMap and DrawCursor; Windows has two there, a849 then a99f. a849's thunk map_draw_marker (1040:20d3) has 0 callers, and the Mac has no thunk between PieceIconDraw(Sprite&) and PieceInit, so the index form is a Windows-only overload (see rejected).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MapWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void SetGlobalSpriteScale(int num, int den);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_ResetClip(GRPort *port);

void MapWin_PieceIconDraw(MapWin *w, Sprite *sprite, int x, int y, int flags)
{
    int bank;
    int eight;
    RECT r1;
    RECT r2;
    RECT r3;

    bank = w->zoom + 8;
    eight = 8;
    SetGlobalSpriteScale(bank, eight);
    GRPort_SetClip((GRPort *)w, (RECT *)&w->rect.left);
    if (flags & 8)
        Sprite_DrawMono(&r1, sprite, (GRPort *)w, w->rect.left + x + 2, w->rect.top + y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 1)
        Sprite_DrawMono(&r2, sprite, (GRPort *)w, w->rect.left + x - 2, w->rect.top + y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        Sprite_Draw(&r3, sprite, (GRPort *)w, w->rect.left + x, w->rect.top + y);
    GRPort_ResetClip((GRPort *)w);
    SetGlobalSpriteScale(1, 1);
}
