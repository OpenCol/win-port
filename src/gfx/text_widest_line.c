/* PORT: this file's own declarations of MSGetTextLength, font_handle_1068_dd1e removed: the header declares them from the definition (W1 step 3). */
/* text_widest_line: candidate for 1068:cbf3 (158 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 158, not the census 149. The trailing 9 bytes are the dead second epilogue.
 *
 * Measures a multi-line label: the string is split on `_`, each piece an underscore ends is measured with
 * `MSGetTextLength` (the last piece never is), the widest goes back through the caller's `int *`, and the number of
 * pieces is returned. One underscore means two lines, so the count starts at 1.
 *
 * The pieces are copied into a 0x80-byte stack buffer as it goes, and the terminator is
 * written at the separator: `buf[i] = 0` BEFORE the measure, and `i` reset to 0 after. The
 * separator itself is then copied into the buffer too -- the store after the `if` is
 * unconditional -- so the buffer always holds the current piece preceded by the underscore
 * that ended the last one. The measure only ever reads up to the `0`, so that stray byte
 * costs nothing, and it is what the shipped code does rather than a reading of it.
 *
 * THE SEPARATOR TEST CARRIES AN EXPLICIT `(int)` AND THE LOOP'S END TEST DOES NOT, and that
 * asymmetry is in the bytes. `*p == '_'` compiles to one `CMP BYTE [bx],0x5f`; the shipped
 * has `MOV AL,[bx] / CBW / CMP AX,0x5f`, three instructions for the same answer, which is
 * what a cast to `int` costs. The end test two lines later is a plain `CMP BYTE [es:bx],0`.
 * Borland folds a promoted comparison back to a byte compare when it can, and a written-out
 * cast is what stops it -- so the cast was in the source. The `CBW` also says the character
 * is signed; `unsigned char` would give `MOV AH,0`.
 *
 * The widest-so-test is `maxw < w`, with the accumulator on the LEFT: the shipped loads
 * `[bp-6]` and compares it against `[bp-8]`.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` */

int text_widest_line(char *s, int *pw, void *font)
{
    int lines;
    int maxw;
    int w;
    char *p;
    char buf[0x80];
    register int i;

    p = s;
    lines = 1;
    i = 0;
    maxw = 0;
    while (*p != 0) {
        if ((int)*p == '_') {
            buf[i] = 0;
            w = MSGetTextLength(font_handle_1068_dd1e((Font *)font), buf);
            if (maxw < w)
                maxw = w;
            i = 0;
            lines++;
        }
        buf[i] = *p;
        p++;
        i++;
    }
    *pw = maxw;
    return lines;
}
