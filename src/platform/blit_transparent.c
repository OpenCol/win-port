/* blit_transparent: candidate for 1070:ce4f (411 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Draws the (sx, sy) w-by-h part of bitmap `hbm` onto `hdc` at (x, y) with teal as the
 * transparent colour, entirely through GDI. The source is selected into its own DC with its
 * background colour set to RGB(0, 128, 128), so a SRCCOPY into a monochrome bitmap turns every
 * teal pixel white and the rest black: that is the mask. The composite is built off screen in a
 * third DC: the destination copied in, the source XORed on (SRCINVERT), the mask ANDed (SRCAND),
 * the source XORed on again, and the result copied back. Every object is deselected and
 * deleted afterwards.
 *
 * The XOR-AND-XOR form is the flicker-free transparent blit of the Windows 3.1 era, and 0x808000
 * is teal written as a COLORREF, 0x00BBGGRR.
 */

#include "colonization.h"

void blit_transparent(unsigned hdc, unsigned hbm, int x, int y, register int w,
                                    int h, int sx, int sy)
{
    unsigned src_dc;
    unsigned mask_dc;
    unsigned old_src;
    unsigned mask_bm;
    unsigned old_mask;
    unsigned mem_bm;
    unsigned old_mem;
    register unsigned mem_dc;

    src_dc = CreateCompatibleDC(hdc);
    old_src = SelectObject(src_dc, hbm);
    mask_dc = CreateCompatibleDC(hdc);
    mem_dc = CreateCompatibleDC(hdc);
    mem_bm = CreateCompatibleBitmap(hdc, w, h);
    old_mem = SelectObject(mem_dc, mem_bm);
    mask_bm = CreateBitmap(w, h, 1, 1, 0);
    old_mask = SelectObject(mask_dc, mask_bm);
    SetBkColor(mem_dc, 0xffffffL);
    SetTextColor(mem_dc, 0L);
    SetBkColor(src_dc, 0x808000L);
    BitBlt(mask_dc, 0, 0, w, h, src_dc, sx, sy, 0xcc0020L);
    BitBlt(mem_dc, 0, 0, w, h, hdc, x, y, 0xcc0020L);
    BitBlt(mem_dc, 0, 0, w, h, src_dc, sx, sy, 0x660046L);
    BitBlt(mem_dc, 0, 0, w, h, mask_dc, 0, 0, 0x8800c6L);
    BitBlt(mem_dc, 0, 0, w, h, src_dc, sx, sy, 0x660046L);
    BitBlt(hdc, x, y, w, h, mem_dc, 0, 0, 0xcc0020L);
    SelectObject(mask_dc, old_mask);
    DeleteObject(mask_bm);
    SelectObject(mem_dc, old_mem);
    DeleteObject(mem_bm);
    DeleteDC(mask_dc);
    DeleteDC(mem_dc);
    SelectObject(src_dc, old_src);
    DeleteDC(src_dc);
}
