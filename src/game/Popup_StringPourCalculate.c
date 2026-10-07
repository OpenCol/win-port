/* Popup_StringPourCalculate: candidate for 1050:1757 (911 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 911, not the census 902. The trailing 9 bytes are the dead second epilogue.
 *
 * LAYS OUT A POPUP'S TEXT into the four line arrays that `Popup_StringPourDraw` draws (pointer
 * +0xac, length +0x1ac, x +0x22c, y +0x2ac). It walks the item chain at +0x376 (flags +0, text +2,
 * next +6) and returns the height used, one line height past the last line. The line after the last
 * gets a null pointer.
 *
 * An item with flag 1 or 2 is ONE LINE: flag 1 centres it in the width at +0x358, and otherwise it
 * starts at the left margin (+0x346, plus 6).
 *
 * ANY OTHER ITEM IS WRAPPED WORD BY WORD. After skipping spaces, the scan extends a word at a time
 * (writing a NUL over each space to measure, then putting it back) until the next word would pass
 * width - 6. The line is emitted up to the space before that word, and the next one starts at the
 * margin. The final piece of an item that fits is emitted where it stands, and x and the line width
 * advance by its width plus a space, so the next item continues on the same line.
 *
 * When even the FIRST word does not fit, the flag at [bp-0x21] makes it start a new line and try
 * again. A single word wider than the whole box would fail every time, so that loop would not end.
 *
 * The line height comes from `MSGetLineHeight_1050_834b` and the space width from measuring " ". `(int)*s` on the
 * space test is rule 51.
 *
 * RENAMED 2026-09-26 from popup_layout_text: the Mac build's Popup::StringPourCalculate(void): this only, pouring the popup's text into its line arrays; Popup_StringPourDraw draws them. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` */
extern int MSGetTextLength_1050_8391(Font *font, const char *s);
extern int MSGetLineHeight_1050_834b(Font *font);

int Popup_StringPourCalculate(Popup *w)
{
    char *s;
    char *p;
    char *prev;
    int height;
    PopupLine *item;
    int width;
    int line_h;
    int x;
    int space_w;
    int line_w;
    int y;
    char stuck;
    register int n;
    register int ww;

    stuck = 0;
    x = w->text_x + 6;
    line_w = 0;
    space_w = MSGetTextLength_1050_8391(w->font, " ");
    width = w->width;
    height = 0;
    y = w->text_y;
    line_h = MSGetLineHeight_1050_834b(w->font);
    item = w->lines;
    n = 0;
    while (item != 0) {
        if (item->flags & 3) {
            s = item->text;
            if (item->flags & 1)
                x = (width >> 1) - (MSGetTextLength_1050_8391(w->font, s) >> 1);
            else
                x = 0;
            w->line[n] = s;
            w->len[n] = STRLEN(s);
            w->line_x[n] = w->text_x + x + 6;
            w->line_y[n] = y + 3;
            n++;
            height += line_h;
            y += line_h;
            x = w->text_x + 6;
        } else {
            s = item->text;
            while (s != 0) {
                while ((int)*s == ' ')
                    s++;
                p = s;
                while (p != 0) {
                    prev = p;
                    p = STRCHR(p, ' ');
                    if (p != 0) {
                        *p = 0;
                        ww = MSGetTextLength_1050_8391(w->font, s);
                        *p = ' ';
                        p++;
                    } else {
                        ww = MSGetTextLength_1050_8391(w->font, s);
                    }
                    if (ww + line_w > width - 6) {
                        if (s == prev)
                            stuck = 1;
                        prev--;
                        p = prev;
                        break;
                    }
                }
                if (stuck) {
                    stuck = 0;
                    height += line_h;
                    y += line_h;
                    x = w->text_x + 6;
                    line_w = 0;
                    continue;
                }
                if (p != 0) {
                    w->line[n] = s;
                    w->len[n] = p - s;
                    w->line_x[n] = x;
                    w->line_y[n] = y + 3;
                    n++;
                    height += line_h;
                    y += line_h;
                    s = p;
                    x = w->text_x + 6;
                    line_w = 0;
                } else {
                    w->line[n] = s;
                    w->len[n] = STRLEN(s);
                    w->line_x[n] = x;
                    w->line_y[n] = y + 3;
                    n++;
                    line_w += ww + space_w;
                    x += ww + space_w;
                    s = p;
                }
            }
        }
        item = item->next;
    }
    height += line_h;
    w->line[n] = 0;
    return height;
}
