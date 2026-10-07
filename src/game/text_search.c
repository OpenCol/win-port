/* text_search: candidate for 1040:7087 (91 bytes, 12 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Loads a text resource and reads n + 1 lines from it; returns 1 when the load failed.
 *
 * RENAMED 2026-09-26 from skip_text_lines: the Mac build's text_search(char*, char*, int), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller DoBuildingsArticle; caller DoCargoArticle; caller DoFathersArticle; caller DoJobArticle; caller DoTerrainArticle), the runner-up gets 7, and the arity agrees (3).
 */

#include "colonization.h"

extern int text_open(void *ctx, char *name);
extern char *text_get(void);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void text_close(void);
int text_search(void *ctx, char *name, int n)
{
    int err = 1;
    int i;

    if (text_open(ctx, name))
        goto done;
    for (i = 0; i <= n; i++)
        text_get();
    err = 0;
done:
    text_close();
    return err;
}
