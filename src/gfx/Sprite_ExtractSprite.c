/* PORT: the row header's words written as i16, not int (4 bytes here) (W4). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 105, 145 of the imported file. */
/* PORT: this file's own declarations of GRPort_PixelMap removed: the header declares it from the definition (W1 step 3). */
/* Sprite_ExtractSprite: candidate for 1068:7dd3 (1813 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Sprite_ExtractSprite
 * name-source: sibling-build
 * size-source: 1813, not the census 1804. The last nine bytes are a dead second epilogue.
 *
 * TURNS A RECTANGLE OF A GRAPHICS PORT INTO A SPRITE -- the compressor behind every piece of art the
 * game draws with transparency. It finds the bounding box of the pixels that are not the key colour,
 * builds a run per row, and packs them into one handle.
 *
 * THE KEY COLOUR has three forms. A key of -1 means "whatever the top-left pixel is", which is how
 * most art declares its own transparency; any other value is the colour itself; and -2 means NO key at
 * all, which is spelled as `|| key == -2` beside every pixel comparison, so the test can never fail
 * and the whole rectangle is copied.
 *
 * FOUR PASSES over the rectangle. The first walks down from the top until it finds a row with any
 * non-key pixel, which gives the top edge; the second walks up from the bottom for the bottom edge.
 * The third pass -- the expensive one -- walks each surviving row from the left for the first non-key
 * pixel and from the right for the last, recording a pointer, a start and a length per row, and
 * keeping the smallest start and the largest end as the left and right edges. The fourth copies the
 * runs out.
 *
 * THE SPRITE FORMAT falls out of that: per row a two-byte start, a two-byte length, then that many
 * pixels, and a zero DWORD at the end. A row with nothing on it stores a start and a length of zero
 * and no pixels, so an empty band costs four bytes.
 *
 * THE SCAN TABLES are three arrays of 640 entries allocated with `new[]` -- one of pointers, one of
 * starts, one of lengths -- and the success path and the out-of-memory path after them delete all three; the path that finds one of them null returns without deleting the others. An image that is
 * entirely key colour never reaches them: it gets an empty rectangle, a null data pointer and a zero
 * handle, which is a success and not a failure.
 *
 * IT LOCKS THE PORT IF IT HAS TO, with a warning, and unlocks it again on the normal exits and on two of
 * the three error paths -- the scan-table allocation failure returns without MSUnlockGraf -- which is why
 * the flag is a local and not a return value.
 *
 * WHAT THE BYTES SETTLED: the row step is in each `for` loop's INCREMENT, not at the end of its body;
 * the inner loop's `break` jumps to the step, which nothing else would produce. The sprite's +0x1c is a
 * HUGE POINTER -- the shipped tests it by ORing its halves (rule 75), where a `long` would compare a
 * dword. The final copy loop reuses the outer scan cursor rather than the inner one. And the size in
 * the out-of-memory message is `(long)(int)total`, narrowed and widened again, where the same variable
 * reaches `MSAllocateHandle` as a plain long.
 *
 * RENAMED 2026-09-27 from ExtractSprite: CONST PROPOSAL. A spelling change to the Class_Method convention; the binding is unchanged. The name came from the error string 'Error: Unable to allocate scan line tables in ExtractSprite' (name_source original). Mac Sprite::ExtractSprite(GRPort&, short, Rect*), CODE 2 0x0a9e, 1196 bytes. 7dd3 (Sprite *s, GRPort *port, int key, RECT *src) is this plus the same three. Callees: GRPort Lock/Unlock, PixelMap x2, Prev/NextScanLine x2, Sprite::Unlock, MSDisposeHandle, MSAllocateHandle, MSHLock/HUnlock, new[]/delete[] x3 on both. Layout: directly after ChangeColor on both (Windows has one extra ChangeColor variant between). Size 1196 against 1813. DO NOT change the string literal. Update tools/subsystems.py NAME_SEEDS, which is keyed by the name 'ExtractSprite', and the comment in tools/mac_const_witness.py:954. ColChild_ExtractSprite contains the old name as a suffix, so rename by exact word.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
typedef unsigned char *Row;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" char GRPort_IsLocked(GRPort *p);
extern "C" void MSLockGraf(GRPort *port);
extern "C" void MSUnlockGraf(GRPort *port);
extern "C" void Sprite_Unlock(Sprite *s);
extern "C" unsigned char *GRPort_NextScanLine(GRPort *p, unsigned char *row);
extern "C" unsigned char *GRPort_PrevScanLine(GRPort *port, unsigned char *row);

extern "C" void report_error_1068_963d(void *ctx, char *msg);
extern "C" void report_error_1068_98bc(void *ctx, char *msg, int x, int y);
extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

int Sprite_ExtractSprite(Sprite *s, GRPort *port, int key, RECT *src)
{
    register int x;
    register int y;
    int h;
    unsigned char *p;
    unsigned char *p2;
    unsigned char *q;
    unsigned char *out;
    RECT r;
    int bot;
    int right;
    int top;
    int left;
    unsigned char key_color;
    int w;
    int ht;
    long total;
    int locked;
    int blank;
    Row *rows;
    int *starts;
    int *lens;

    locked = 1;
    blank = 1;
    SetRect(&r, src->left, src->top, src->right, src->bottom);
    IntersectRect(&r, &r, (RECT *)((char *)port + 8));
    left = 0x2710;
    right = 0;
    total = 0;
    w = r.right - r.left;
    ht = r.bottom - r.top;
    if (!GRPort_IsLocked(port)) {
        report_error_1068_963d(&g_error_log, "Warning: Port not locked on Sprite Extract");
        locked = 0;
        MSLockGraf(port);
    }
    p = (unsigned char*)(GRPort_PixelMap(port, r.left, r.top));
    if (p == 0) {
        report_error_1068_98bc(&g_error_log,
                        "Error: Sprite Extract Illegal Pixel Mapping (x,y)",
                        r.left, r.top);
        if (locked == 0)
            MSUnlockGraf(port);
        return 0;
    }
    if (key == -1)
        key_color = *p;
    else if (key != -2)
        key_color = (unsigned char)key;
    for (y = 0; y < ht; y++, p = GRPort_NextScanLine(port, p)) {
        for (x = 0, q = p; x < w; x++) {
            if (*q++ != key_color || key == -2) {
                top = y;
                p = GRPort_PrevScanLine(port, p);
                y = ht;
                blank = 0;
                break;
            }
        }
    }
    if (blank != 0) {
        SetRect((RECT *)((char *)s + 8), 0, 0, 0, 0);
        SetRect((RECT *)s, 0, 0, rect_width_1068_96a9(&r),
                rect_height_1068_96c8(&r));
        if (key != -2)
            s->key = key_color;
        else
            s->key = 0;
        s->hx = 0;
        s->hy = 0;
        if (locked == 0)
            MSUnlockGraf(port);
        h = 0;
        s->bits = 0;
        return h;
    }
    p2 = (unsigned char*)(GRPort_PixelMap(port, r.left, ht + r.top - 1));
    for (y = 0; y < ht; y++, p2 = GRPort_PrevScanLine(port, p2)) {
        for (x = 0, q = p2; x < w; x++) {
            if (*q++ != key_color || key == -2) {
                bot = ht - y;
                y = ht;
                break;
            }
        }
    }
    rows = new Row[0x280];
    starts = new int[0x280];
    lens = new int[0x280];
    if (rows == 0 || starts == 0 || lens == 0) {
        report_error_1068_963d(&g_error_log,
                        "Error: Unable to allocate scan line tables in ExtractSprite");
        return 0;
    }
    for (y = 0; bot - top > y; y++, p = GRPort_NextScanLine(port, p)) {
        total += 4;
        starts[y] = 0;
        lens[y] = 0;
        for (x = 0, q = p; x < w; x++, q++) {
            if (*q != key_color || key == -2) {
                rows[y] = q;
                starts[y] = x;
                break;
            }
        }
        if (x != w) {
            if (starts[y] < left)
                left = starts[y];
        }
        for (x = 0, q = p + w - 1; x < w; x++, q--) {
            if (*q != key_color || key == -2) {
                lens[y] = w - x - starts[y];
                total += lens[y];
                break;
            }
        }
        if (x != w) {
            if (lens[y] + starts[y] > right)
                right = lens[y] + starts[y];
        }
    }
    if (s->bits != 0)
        Sprite_Unlock(s);
    if (s->handle != 0)
        s->handle = MSDisposeHandle(s->handle);
    total += 4;
    h = MSAllocateHandle(total);
    if (h == 0) {
        report_error_1068_94ea(&g_error_log,
                        "Error: Couldn't allocate memory for sprite data of size",
                        (long)(int)total);
        if (locked == 0)
            MSUnlockGraf(port);
        delete rows;
        delete starts;
        delete lens;
        return h;
    }
    SetRect((RECT *)((char *)s + 8), left, top, right, bot);
    SetRect((RECT *)s, 0, 0, rect_width_1068_96a9(&r), rect_height_1068_96c8(&r));
    if (key != -2)
        s->key = key_color;
    else
        s->key = 0;
    s->hx = 0;
    s->hy = 0;
    out = (unsigned char *)MSHLock(h);
    for (y = 0; bot - top > y; y++) {
        *(i16 *)out = starts[y];
        out += 2;
        *(i16 *)out = lens[y];
        out += 2;
        for (x = lens[y], p = rows[y]; x > 0; x--) {
            *out = *p;
            p++;
            out++;
        }
    }
    *(long *)out = 0;
    out = (unsigned char *)MSHUnlock(h);
    if (locked == 0)
        MSUnlockGraf(port);
    delete rows;
    delete starts;
    delete lens;
    return h;
}

