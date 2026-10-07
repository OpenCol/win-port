/* draw_text_resource: candidate for 1040:66e0 (147 bytes, 8 callers).  Verdict: RELOCATABLE.
 *
 * Opens the resource named `name` in the global resource context, loads it, hands the
 * data to draw_wrapped_text with the target and the rectangle, then releases and closes.
 *
 * RENAMED 2026-09-26 from load_named_resource: Copies the name, finds and locks the named resource in the global resource context, and passes the data to text_prompt (1040:62b6, i.e. ParseFancyTextAndShow(port, rect, text, 1)), which draws it word-wrapped into the caller's port and rectangle. Then it unlocks and frees the resource. The callers (the Pedia article pages, get_game_options) use it to draw article text. The job is rendering text into a rectangle. The old name describes only the lookup step and does not explain the target and rect arguments.
 */

#include "colonization.h"

/* The resource type is a 4-character code, a Mac ResType, DEFINED as `char[4]`: segment 9's
   _DATA holds 'STRN' and 'TEXT' at odd addresses (0x1cfb, 0x1cff), where Borland 4.52 would
   word-align a long or a pointer (tools/check_module_data.py, 2026-10-02). It is read whole. */

extern void *MSLockResource_1040_a99c(void *res);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void draw_wrapped_text(void *target, void *rect, void *data);
extern void MSUnlockResource_1040_a9bc(void *res);
extern void MSReleaseResource_1040_a9d8(void *res);
void draw_text_resource(void *target, char *name, void *rect)
{
    void *ctx = *(void **)g_restype_text_draw;
    void *res;
    void *data;
    char buf[0x20];

    STRCPY(buf, name);
    res = resource_find_1040_a976((long *)&ctx, buf);
    data = MSLockResource_1040_a99c(res);
    if (data != 0)
        draw_wrapped_text(target, rect, (char *)data);
    MSUnlockResource_1040_a9bc(res);
    MSReleaseResource_1040_a9d8(res);
}
