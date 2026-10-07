/* text_dialog_1050_8e85: candidate for 1050:8e85 (48 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Segment 1050's copy of the TEXT-dialog tail: it reaches `run_text_dialog` with the three
 * long arguments zero, exactly as `text_dialog_1008_ebd0` and `text_dialog_1038_8c3c` do in
 * their own modules. One small function compiled into every module that shows a TEXT dialog.
 *
 * The difference from those two is the CALL FORM, and it decides the source. They reach
 * `run_text_dialog` across a segment boundary, so it is `extern` and the call is the
 * five-byte one. Here `run_text_dialog` is at 1050:5f98 -- the SAME segment -- and the
 * shipped bytes are the four-byte `push cs; call `, so rule 47 applies and the callee
 * has to be DEFINED in this file rather than declared.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int text_dialog_1050_8e85(void *file, void *name, int sel, int kind)
{
    return run_text_dialog(file, name, sel, 0, 0, 0, kind);
}
