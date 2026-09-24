/* opening_title_draw -- shipped at 1010:0d96.
 *
 * Ported by tools/port.py from matched/game/opening_title_draw.c.
 *
 * The file's own Card, Opening dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "sounds.h"
extern void sprite_draw(RECT *out, void *sprite, void *port, col_i16 x, col_i16 y);
extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern void play_sound_effect(col_i16 n);

void opening_title_draw(Opening *w)
{
    RECT r1;
    RECT r2;
    RECT r3;
    register col_i16 d = 0;

    if (w->scroll <= 0x500) {
        d = 0x500 - w->scroll;
        SetRect(&r1, d, 0x38, d + 0x280, 0x17b);
    } else {
        SetRect(&r1, 0, 0x38, 0x280, 0x17b);
    }
    SetRect(&r2, 0, 0x38, 0x280, 0x17b);
    CopyToPort(((char *)((unsigned char *)(w) + 0xc4)), w, &r1, &r2);
    SetRect(&r2, 0, 0x17b, 0x280, 0x1e0);
    CopyToPort(((char *)((unsigned char *)(w) + 0xc4)), w, &r2, &r2);
    if ((*((void * *)((unsigned char *)(w) + 0x1e7e))) != 0)
        sprite_draw(&r3, (*((void * *)((unsigned char *)(w) + 0x1e7e))), w, (*((col_i16 *)((unsigned char *)(w) + 0x1e82))) - d, (*((col_i16 *)((unsigned char *)(w) + 0x1e84))));
    if ((*((char *)((unsigned char *)(w) + 0x1e86))) == 0)
        play_sound_effect(SOUND_MUSIC_JINE_THE_CAVALRY);
}
