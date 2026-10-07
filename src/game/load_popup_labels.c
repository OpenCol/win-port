/* load_popup_labels: candidate for 1050:002f (97 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Opens the POPUPS group of the LABELS text resource and caches its first three entries:
 * the raw value from `text_string` into the word array at
 * DGROUP:0x4cf8, and the string `strings` gives for it into the -pointer array
 * at SEG31:0x98.
 *
 * Both literals were read out of DGROUP rather than guessed (§119): 0x254c is "LABELS" and
 * 0x2553 is "POPUPS", seven bytes apart because the first is six characters and a NUL.
 *
 * A failed load leaves both arrays alone -- the whole body is inside `if (load(...) == 0)`,
 * and the shipped spells that as `JZ` to the body then `JMP` past, which is rule 12's
 * invert-and-jump-over for `if (load(...) != 0) goto out;`.
 */

#include "colonization.h"

extern int text_open(void *file, char *group);
extern int text_string(void);
extern char *strings(int n);

/* DEFINED here, with its defaults: the three literals open segment 11's DGROUP pool, ahead
   of "LABELS" and "POPUPS" below (tools/check_module_data.py, 2026-10-02) */
char * g_popup_label[3] = { "~OK", "~Help", "~Cancel" };   /* SEG31:0x98 */

void load_popup_labels(void)
{
    register int i;

    if (text_open("LABELS", "POPUPS") != 0)
        goto out;
    for (i = 0; i < 3; i++) {
        g_popup_label_id[i] = text_string();
        g_popup_label[i] = strings(g_popup_label_id[i]);
    }
out:
    ;
}
