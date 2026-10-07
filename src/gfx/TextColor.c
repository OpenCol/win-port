/* TextColor: candidate for 1068:616a (55 bytes, 290 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets the drawing colour and returns the one it replaced.
 *
 *   MOV AX,0x1108 / MOV ES,AX / MOV AL,ES:[0x2ada]   -> read the colour byte
 *   MOV byte [BP-3],AL                                  save the old value
 *   MOV AX,0x1108 / MOV DL,[BP+6] / MOV ES,AX
 *   MOV ES:[0x2ada],DL                                -> store the argument
 *   MOV AL,[BP-3]                                     -> return the old value
 *
 * The byte at DGROUP:0x2ada is `g_line_color_index`, the palette index every one of
 * `MSDrawLine`, `MSDrawText`, `MSDrawString_rect` and `MSDrawString` reads back through
 * the WinG DIB colour table before it draws. This function is the only thing that writes it,
 * and the callers use it in matched pairs -- `GRPort_DrawInsetRect` and
 * `GRPort_DrawRaisedRect` set 0xef, 0xf7 and 0xff for the three edges of a bevel and put the
 * saved value back at the end -- so it is the foreground colour of the whole widget layer.
 *
 * The name is the Macintosh build's: `macui`'s `TextColor(unsigned char)` is the only
 * unbound colour-setter in that library's symbol table, it takes the same single byte, and
 * `macui` is where the `MS*` drawing routines this one feeds also live. Adopted, not derived
 * -- the Mac table gives no return type, and the Windows build's returning the previous
 * colour is the one thing the name does not predict.
 *
 * 55 bytes, not the census 48: the trailing seven are a second, unreachable epilogue, which
 * is what the explicit `return old;` costs (rule 44).
 *
 * The explicit segment load (ES := 0x1108) rather than plain DS addressing is what a
 * ``-qualified global produces: in a -WS build DS == SS, so a object is reached
 * through its own segment. Both the 0x1108 and the 0x2ada are relocation sites.
 */

#include "colonization.h"

unsigned char TextColor(unsigned char colour)
{
    unsigned char old = g_line_color_index;
    g_line_color_index = colour;
    return old;
}
