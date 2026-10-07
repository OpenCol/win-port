/* text_open: candidate for 1040:6cf4 (302 bytes, 60 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Loads the named resource into the shared text buffer, NUL-terminates it at its size,
 * points three text cursors at it, and releases the resource. Returns 0 on every path,
 * a NULL name included: that branch clears the flag too, so the text_close fallback is
 * never reached; the "loaded" local is
 * written and never read.
 *
 * RENAMED 2026-09-26 from load_text_resource: the Mac build's text_open(char*, char*), bound by call-sequence alignment: in text_search the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (2), and the body does what the Mac name says.
 */

#include "colonization.h"

/* The resource type is a 4-character code, a Mac ResType, DEFINED as `char[4]`: segment 9's
   _DATA holds 'STRN' and 'TEXT' at odd addresses (0x1cfb, 0x1cff), where Borland 4.52 would
   word-align a long or a pointer (tools/check_module_data.py, 2026-10-02). It is read whole. */

extern void *MSLockResource_1040_a99c(void *res);

extern void MSUnlockResource_1040_a9bc(void *res);
extern void MSReleaseResource_1040_a9d8(void *res);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void text_close(void);
int text_open(void *unused, char *name)
{
    int flag = 1;
    int loaded = 0;
    void *ctx = *(void **)g_restype_text_open;
    char buf[0x20];

    if (name == 0)
        flag = 0;
    else {
        STRCPY(buf, name);
        g_cur_res = resource_find_1040_a976((long *)&ctx, buf);
        if (g_cur_res == 0)
            return 0;
        g_cur_data = (char *)MSLockResource_1040_a99c(g_cur_res);
        if (g_cur_data == 0)
            return 0;
        loaded = 1;
        MSCopyMem_1068_9424(g_cur_data, g_text, block_size(g_cur_res));
        g_text[block_size(g_cur_res)] = 0;
        g_text_cursor = g_text_read_ptr = g_cur_data = g_text;
        MSUnlockResource_1040_a9bc(g_cur_res);
        MSReleaseResource_1040_a9d8(g_cur_res);
        flag = 0;
    }
    if (flag)
        text_close();
    return flag;
}
