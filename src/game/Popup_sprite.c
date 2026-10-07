/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 45, 60 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* Popup_sprite: candidate for 1050:107b (433 bytes).  Verdict: RELOCATABLE.
 * size-source: 433, not the census 426. The trailing seven bytes are the dead second epilogue.
 * name-source: sibling-build
 *
 * Appends a PICTURE item to a Popup -- the 0x1c-byte counterpart of Popup_item. The 32-bit value
 * stored at +0x10 is a sprite -- a unit portrait from five callers, a goods icon from
 * `colony_click_ship` -- and the width starts at 0x20 for it. It walks to the tail of the list at +0x37a, allocates the item from the block at +0x392,
 * and links it on. The list's HEAD keeps a pointer to the tail at +0x18, so appending rewrites the
 * head's +0x18 and the first item gets 0 there; the first item is also recorded at +0x36a. The
 * item takes the id at +0, and a copy of the text when there is one. The popup's width at +0x35a is
 * widened to 0x20 plus the text's extent in the font at +0x6e plus the margin at +0xa6, and the
 * count at +0x7e is incremented.
 *
 * These offsets sit 4 to 0xc bytes past the ones `Popup_item` declares for the same fields, so one
 * of the two layouts is misread; the comparator cannot tell, since every displacement here is
 * relative to a parameter.
 *
 * RENAMED 2026-09-26 from popup_image_item: the Mac build's Popup::sprite(Sprite*, int, char*): this plus a sprite, an id and a caption, in that order -- adding an image item. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;   /* 0x1c bytes */
#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetTextLength_1050_8391(Font *font, const char *s);

PopupSprite *Popup_sprite(Popup *popup, Sprite *sprite, int id, char *s)
{
    PopupSprite *tail;
    PopupSprite *it;
    i16 w;   /* PORT: i16, max_ptr_1050_97d1 takes its address */

    tail = 0;
    it = popup->sprites;
    while (it != 0) {
        tail = it;
        it = it->next;
    }
    it = (PopupSprite *)heap_get((MemBlock *)(&popup->arena), 0x1c);
    if (tail != 0) {
        tail->next = it;
        it->prev = tail;
        popup->sprites->prev = it;
    } else {
        popup->sprites = it;
        popup->first_sprite = it;
        popup->sprites->prev = 0;
    }
    it->next = 0;
    it->f2 = 0;
    it->sprite = sprite;
    w = 0x20;
    if (s != 0) {
        it->text = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(s) + 1);
        STRCPY(it->text, s);
        w += MSGetTextLength_1050_8391(popup->font, it->text) + popup->gap_x;
    } else {
        it->text = 0;
    }
    it->id = id;
    popup->sprite_w = *max_ptr_1050_97d1(&w, &popup->sprite_w);
    popup->n_sprites++;
    return it;
}
