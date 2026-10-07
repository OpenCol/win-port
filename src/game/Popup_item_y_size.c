/* Popup_item_y_size: candidate for 1050:0ad6 (133 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 133, not the census 126. The trailing 7 bytes are the dead second epilogue.
 *
 * The list row height, clamped from both ends: start from `Popup_item_x_size` (0x20), cap it
 * at five quarters of the font's line height (`MSGetLineHeight_1050_834b`), then floor it at the line height
 * itself. A row is never shorter than a line of text and never more than 25% taller.
 *
 * The 5/4 is `IMUL AX,AX,5` then `SAR AX,2` -- an arithmetic SHIFT, so it is `(w * 5) >> 2`
 * and not `* 5 / 4`, which would carry a sign correction.
 *
 * THE CAP IS RULE 45. `min_ptr_1050_979f` compares through two pointers, so the cap needs an
 * addressable operand, and the shipped stores the scaled width and then takes its address
 * INSIDE the argument -- `LEA / PUSH SS / PUSH AX`, the comma-expression form -- where the
 * other argument uses the statement-level `PUSH SS / LEA / PUSH AX`. Written as two
 * statements both arguments take the second form and the function is four bytes shorter.
 *
 * The floor is an ordinary `max_int` on values, not pointers, which is why only one of the
 * two clamps needs the idiom.
 *
 * RENAMED 2026-09-26 from ctl_clamped_width: the Mac build's Popup::item_y_size(void): this only -- the list row height (+0x9e, row_h) derived from the font's line height, clamped between it and 0x20; a HEIGHT, not a width. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` */
extern int Popup_item_x_size(Popup *popup);
extern int MSGetLineHeight_1050_834b(Font *font);

int Popup_item_y_size(Popup *popup)
{
    int w;
    int r;
    int scaled;

    w = Popup_item_x_size(popup);
    w = *min_ptr_1050_979f(&w,
            (scaled = (MSGetLineHeight_1050_834b(popup->font) * 5) >> 2, &scaled));
    r = max_int_1050_836b(w, MSGetLineHeight_1050_834b(popup->font));
    return r;
}
