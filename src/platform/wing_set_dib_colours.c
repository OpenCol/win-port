/* wing_set_dib_colours: candidate for 1070:a93c (132 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Builds an RGBQUAD table from the palette -- one `palette_entry_rgb` per entry
 * filling red, green and blue through three out-pointers, then the fourth byte zeroed -- and
 * hands the table to `WinGSetDIBColorTable`.
 *
 * The three pointers are +2, +1 and +0 of a four-byte entry and the shipped code pushes the
 * +0 one FIRST, so blue is the last parameter and the lowest byte: the Windows RGBQUAD order.
 * The table is indexed by the ABSOLUTE palette index while the API is handed `&tbl[0]`
 * together with `start`, so the entries it reads are not the ones the loop wrote unless
 * `start` is 0. That is the shipped arithmetic, not a reading of it.
 *
 * PARKED FOR THREE SESSIONS OVER ONE INSTRUCTION, and the lever turned out to be the
 * PARAMETER TYPE, not the argument expression. Every byte matched except where `PUSH SS` sat:
 *
 *     shipped   PUSH SS / MOV AX,SI / SHL AX,2 / LEA DX,[bp-0x402] / ADD AX,DX / PUSH AX
 *     compiled  MOV AX,SI / SHL AX,2 / LEA DX,[bp-0x402] / ADD AX,DX / PUSH SS / PUSH AX
 *
 * Nine spellings of the ADDRESS were tried across two sessions -- `&tbl[i].rgbRed`,
 * `&tbl[i * 4 + 2]`, `&tbl[i][2]`, `tbl + i * 4 + 2`, `i * 4 + tbl + 2`, the same with an
 * explicit cast, with the cast on the base, `&*(...)` and `&(...)[2]` -- and every one
 * of them computes the offset first. Declaring the callee's three out-parameters
 * `void *` instead of `unsigned char *` moves the `PUSH SS` and the function
 * matches. See rule 56.
 *
 * The two halves have to agree, and one of them is already fixed: `i * 4 + tbl + 2` with the
 * INDEX on the left is what puts the index in AX and the base in DX (`LEA DX` / `ADD AX,DX`);
 * `tbl + i * 4 + 2` puts the base in AX. So the shipped source wrote the scaled index first
 * AND the prototype generic.
 */

#include "colonization.h"

typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;
#include "records.h"
extern int WinGSetDIBColorTable(unsigned hdc, unsigned start, unsigned n,
                                           RGBQUAD *t);

/* same object: 4-byte `push cs; call ` */

void wing_set_dib_colours(GrafRec *graf, void *pal, int start, int count)
{
    unsigned char tbl[0x400];
    register int i;

    for (i = start; start + count > i; i++) {
        palette_entry_rgb(pal, i, i * 4 + tbl + 2, i * 4 + tbl + 1, i * 4 + tbl);
        *(i * 4 + tbl + 3) = 0;
    }
    WinGSetDIBColorTable(graf->hdc, start, count, (RGBQUAD *)tbl);
}
