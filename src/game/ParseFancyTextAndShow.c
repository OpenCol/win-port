/* PORT: this file's own declarations of MSGetTextLength_1040_a8dd, font_height_1040_a8c4 removed: the header declares them from the definition (W1 step 3). */
/* ParseFancyTextAndShow: candidate for 1040:62e0 (661 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 661, not the census 652. The trailing 9 bytes are the dead second epilogue.
 *
 * Lays text into a rectangle a word at a time, drawing it when `draw` is set and returning the
 * baseline it ended on either way -- so the same call measures and paints. It starts half a line
 * below the rectangle's top in colour 0xfa, with the font at DGROUP:0x1ce4, and reads the text a
 * character at a time:
 *
 *   '^'   ends the line (the end of the text counts as one), and a space after it is skipped
 *   0xF9  a tab: five spaces into the word and four into the line
 *   '{'   switches to colour 0xc
 *   '}'   acts as a space and switches back to 0xfa once that word is drawn
 *
 * Every other character goes on the end of both the word and the line. When the line grows wider
 * than the rectangle the baseline moves down and the line restarts from the current word, and a
 * space draws the word and advances x past it.
 *
 * The line buffer only exists to be measured: nothing is ever drawn from it.
 *
 * RENAMED 2026-09-27 from wrap_text: CONST PROPOSAL. Mac ParseFancyTextAndShow(GRPort&, Rect*, char*, short), CODE 28 0x20f6, 626 bytes. 62e0 (void *port, RECT *r, char *text, int draw) has the same four arguments. Callees: TextColor x3, String x2, text-length x3 (font_text_extent), string copy/cat on both. Caller: Mac (bsr) the 3-argument overload ParseFancyTextAndShow(GRPort&, Rect*, char*) at 0x20ac (74 bytes). Windows: draw_wrapped_text (1040:62b6, 42 bytes), its only caller. Layout: Mac 3-arg overload, 4-arg, DoFancyText; Windows draw_wrapped_text, 62e0, DoFancyText (6592). Size 626 against 661. The name goes to the body. The 3-argument thunk would carry the same identifier, so it keeps draw_wrapped_text (see chains). (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_String(GRPort *port, unsigned *hf, const char *s, int x, int y, int align);

/* same object: 4-byte `push cs; call ` (rule 47) */

int ParseFancyTextAndShow(GRPort *port, RECT *r, char *text, int draw)
{
    char *p;
    int x;
    int y;
    int w;
    char ch;
    char hilite;
    char done;
    char line[0x100];
    char word[0x50];
    register int n;
    register int word_len;

    hilite = 0;
    done = 0;
    p = text;
    x = r->left;
    y = font_height_1040_a8c4((Font *)g_wrap_font) / 2 + r->top;
    TextColor(0xfa);
    line[0] = word[0] = 0;
    w = 0;
    n = word_len = 0;
    while (done == 0) {
        if (*p == 0) {
            ch = '^';
            done = 1;
        } else {
            ch = *p;
            p++;
        }
        if ((int)ch == '^') {
            if (draw != 0)
                GRPort_String(port, (unsigned *)g_wrap_font, word, x, y, 1);
            y += font_height_1040_a8c4((Font *)g_wrap_font) + 2;
            x = r->left;
            n = word_len = 0;
            line[0] = word[0] = 0;
            if ((int)*p == ' ')
                p++;
        } else if ((int)ch == -7) {
            STRCAT(word, "     ");
            word_len = STRLEN(word);
            STRCAT(line, "    ");
            n = STRLEN(line);
        } else if ((int)ch == '{') {
            TextColor(0xc);
        } else if ((int)ch == '}') {
            hilite = 1;
            ch = ' ';
        } else {
            line[n] = ch;
            line[n + 1] = 0;
            w = MSGetTextLength_1040_a8dd((Font *)g_wrap_font, line);
            word[word_len] = ch;
            word[word_len + 1] = 0;
            n++;
            word_len++;
        }
        if (rect_width_1040_a903(r) < w) {
            y += font_height_1040_a8c4((Font *)g_wrap_font) + 2;
            x = r->left;
            STRCPY(line, word);
            w = MSGetTextLength_1040_a8dd((Font *)g_wrap_font, line);
            n = STRLEN(line);
        }
        if ((int)ch == ' ') {
            if (draw != 0)
                GRPort_String(port, (unsigned *)g_wrap_font, word, x, y, 1);
            x += MSGetTextLength_1040_a8dd((Font *)g_wrap_font, word);
            word[0] = 0;
            word_len = 0;
            if (hilite != 0) {
                TextColor(0xfa);
                hilite = 0;
            }
        }
    }
    return y;
}
