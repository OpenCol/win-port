/* clear_colony_flag_1c -- shipped at 1018:0438, 38 bytes.
 *
 * Clears bit 7 of the current colony's status byte and returns its argument plus 0x1f.
 *
 * Ported from matched/game/clear_colony_flag_1c.c in the byte-exact repo. What changed,
 * and why each change is allowed here: the per-file `typedef struct { char pad[0x1c];
 * unsigned char status; } Colony;` is gone, because col/records.h has the merged Colony
 * and the field is `status` there too; the `far` on the extern is gone, because
 * col/globals.h declares the object once; the `far` on the RETURN type is gone, because a
 * far return is an addressing mode and `-ml` makes every function far anyway.
 */
#include "col/globals.h"

char clear_colony_flag_1c(char n)
{
    g_cur_colony_rec->status &= 0x7f;
    return (char)(n + 0x1f);
}
