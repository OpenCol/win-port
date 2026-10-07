/* MagicWin_load_lbm: candidate for 1068:ad96 (202 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_load_lbm
 * name-source: descriptive
 *
 * Loads a picture into the sprite's port and remaps it if the palette has moved: the palette
 * range (first, count) goes into +0xf6/+0xf8 and the palette pointer into +0xea, `load_lbm_resource` fills the
 * GRPort at +0xc4, the port is selected against the object at +0x26's bits, and the sprite is
 * remapped only when those bits differ from the palette just stored. The port's rectangle is
 * copied out to +0xee at the end.
 *
 * ONE OF FOUR IDENTICAL 202-BYTE FUNCTIONS back to back at 1068:ac02, accc, ad96 and ae60,
 * differing ONLY in which loader they call first -- `GRPort_LoadPicture`, `load_gif_resource`,
 * `load_lbm_resource` and `load_pcx_resource`. Every other instruction is the same, including the
 * two separate `get_ctable_1068_d77f` calls: the result is not kept across `GRPort_ColorSync`,
 * because `-Od` keeps nothing across a call.
 *
 * The comparison against +0xea is done as two 16-bit halves, high first, which is what a 
 * pointer compared for inequality gives when the branch is written the short way round.
 *
 * `load_lbm_resource` and `GRPort_ColorSync` are reached by the five-byte NOP form and are
 * `extern`; `get_ctable_1068_d77f`, `MagicWin_remap_to_palette` and `port_bounds` are reached by
 * the four-byte form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-27 from sprite_load_lbm: the receiver is a MagicWin, not a Sprite: port at +0xc4, palette at +0xea, and every caller sits in MagicWin's module 1068:a698..b188 (521cb981e). The Mac build has no such method; the name stays descriptive.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char pad[1]; } logpal;
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









extern void load_lbm_resource(void *p, int res, int w, int h, void *pal);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_ctable_1068_d77f(void *p);
extern "C" void MagicWin_remap_to_palette(MagicWin *sprite, short first, short count);
extern "C" RECT *port_bounds(void *p);

void MagicWin_load_lbm(MagicWin *o, int res_id, register int first, register int count, void *pal)
{
    o->first = first;
    o->count = count;
    o->pal = pal;
    load_lbm_resource(&o->port, res_id, first, count, (logpal *)pal);
    GRPort_ColorSync(&o->port, (CTable *)get_ctable_1068_d77f(&(DialogWin &)*o));
    if (get_ctable_1068_d77f(&(DialogWin &)*o) != o->pal)
        MagicWin_remap_to_palette(o, first, count);
    CopyRect(&o->rect, port_bounds(&o->port));
}
