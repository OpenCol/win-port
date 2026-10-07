/* text_dialog_1048_e911: candidate for 1048:e911 (42 bytes).  Verdict: RELOCATABLE.
 * symbol: text_dialog_1048_e911
 *
 * text_dialog_1048_ecef with a zero inserted before the last argument.  The callee
 * is -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

int text_dialog_1048_e911(void *file, void *name, int kind)
{
    return text_dialog_1048_ecef(file, name, 0, kind);
}
