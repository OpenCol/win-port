/* GRPort_Init -- shipped at 1068:352f.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_Init.c.
 *
 * The file's own GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char GRPort_IsLocked(GRPort *p);

#include "grerror.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSUnlockGraf(GRPort *p);
extern void MSLockGraf(GRPort *p);
extern void * dispose_wing_surface(void *surface);
extern col_i16 rect_width_1068_96a9(void *r);
extern col_i16 rect_height_1068_96c8(void *r);
extern void GRPort_InitStruct(GRPort *p, void *r);
extern void * MSInitGraf(void *graf);
extern col_i16 control_hwnd(void *surface);
extern void error_code_set(col_i16 code);

col_i16 GRPort_Init(GRPort *p, void *r)
{
    col_i16 ok = 1;

    if ((*((void * *)((unsigned char *)(p) + 0x22))) != 0) {
        if (GRPort_IsLocked(p))
            MSUnlockGraf(p);
        (*((void * *)((unsigned char *)(p) + 0x22))) = dispose_wing_surface((*((void * *)((unsigned char *)(p) + 0x22))));
    }
    if (r == 0 || rect_width_1068_96a9(r) == 0 || rect_height_1068_96c8(r) == 0) {
        GRPort_InitStruct(p, 0);
    } else {
        GRPort_InitStruct(p, r);
        (*((void * *)((unsigned char *)(p) + 0x22))) = MSInitGraf(((char *)((unsigned char *)(p) + 0x10)));
        if ((*((void * *)((unsigned char *)(p) + 0x22))) != 0) {
            (*((col_i16 *)((unsigned char *)(p) + 0x6))) = control_hwnd((*((void * *)((unsigned char *)(p) + 0x22))));
            MSLockGraf(p);
        } else {
            error_code_set(GRERR_NO_SURFACE);
            ok = 0;
        }
    }
    return ok;
}
