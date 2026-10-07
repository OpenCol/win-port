/* Font_ctor_1008_d9c4: candidate for 1008:d9c4 (94 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Font::Font(int,int)
 *
 * Module 1008's copy of the constructor of a 4-byte font record: the font handle from
 * `MSGetTypeFont(kind, height)` and its line height from `MSGetTextHeight`.
 *
 * The parameters are two `int`s, not one `char *`, and the CALLER is what proves it.
 * This function hands `[bp+0xc]` and `[bp+0xa]` straight through, and a four-byte argument
 * passed untouched reads equally well either way -- §119's blind spot exactly. Its one caller, at 1008:b425,
 * pushes `0x10` and `0x0` as plain literals, with no relocation against either, which a pointer
 * could not be; and `MSGetTypeFont(int kind, int height)` was already reconstructed with
 * that signature. Kind 0 is Times New Roman, 1 is Arial.
 *
 * THE CLASS IS `Font` (renamed from `GameFont` 2026-09-28). The image has no `GameFont` type-name
 * string; `Font` is in the RTTI of 1008, 1050, 1060 and 1068, the modules holding Font's inline
 * members. In 1050 this constructor (94 bytes at 0x7f9b) ends exactly where Font::~Font's copy
 * starts (0x7ff9), and KingRoom's font, built by the 1008 copy, is destroyed by Font::~Font
 * (1008:d4b5). Font has two constructors: Font() (Font_ctor_*) and this Font(int, int).
 *
 * RENAMED 2026-09-27 from GameFont_ctor_1008_d9c4: the class is Font: the image holds no 'GameFont' type string; the object is destroyed by Font::~Font, and this is Font's (int, int) constructor (symbol: Font::Font(int,int)), 94 bytes ending exactly where Font's destructor starts.
 */

#include "colonization.h"

#include "classes.h"

extern int MSGetTextHeight(unsigned hfont);

Font::Font(int kind, int height)
{
    hfont = MSGetTypeFont(kind, height);
    this->height = MSGetTextHeight(hfont);
}
