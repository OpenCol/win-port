/* MSGetResource: candidate for 1078:019c (219 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `MSGetNamedResource`'s by-NUMBER twin: the four-character type is copied
 * into a NUL-terminated local, the game's own instance (DGROUP:0x56e2) is searched first and
 * then each loaded resource library (DGROUP:0x56e6, count at 0x3c76), and the first hit is
 * loaded and its global handle returned. `PostLowMemReport` is the caller that shows
 * what the type argument is: a long holding the four characters "TEXT", copied to a local so
 * its address can be passed -- a Macintosh resource type, which is why the name is spelled
 * the Mac way.
 *
 * THE THREE BYTES THAT COST THE MOST WERE `OR EAX,0`, and they are a rule, not a quirk.
 * Every spelling of `MAKEINTRESOURCE(id)` that puts the zero on the RIGHT -- including
 * Borland's own `((LPCSTR)MAKELP(0,(i)))`, which expands to `(WORD)i | ((DWORD)(WORD)0 << 16)`
 * -- is folded away by the front end, under every flag set including `-Od`. Written with the
 * zero on the LEFT it survives verbatim. So the shipped source ORs the constant in first, the
 * way `PALETTEINDEX` does at 1078:1ef6 with `0x01000000L`, and this function is the only site
 * in the image where the constant is zero. Rule 57.
 *
 * The destination has to be a 32-bit scalar, not a pointer: assigned to an `LPCSTR`
 * Borland builds the value in two 16-bit stores and the `MOVZX EAX` never appears.
 *
 * `i` is set to -1 before the first `FindResource` and only reset to 0 if that misses, which
 * is what the `i == -1` at the end reads back -- the same idiom as the by-name twin, and the
 * reason the loop and the load can share one index.
 *
 * 219 bytes, not the census 208: two written-out `return`s make three epilogues (rule 44).
 *
 * RENAMED 2026-09-27 from MSGetNamedResource: the Mac build's MSGetResource(unsigned char*, unsigned short): the body takes (type4, int id) and hands FindResource an integer resource (0:id); the by-NAME getter, MSGetNamedResource(uchar*, uchar*), is 1078:0277 (renamed with it).
 *
 * Returns `void *` (2026-10-02), as all nine declarations of it say: the handle
 * LoadResource gives, zero-extended (`(void *)(DWORD)`, the same bytes as the `unsigned long`
 * it was spelled with).
 */

#include "colonization.h"

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef const char *LPCSTR;
#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
#define MAKELP(sel, off)    ((void *)MAKELONG((off), (sel)))
#define MAKEINTRESOURCE(i)  ((LPCSTR)MAKELP(0, (i)))

void *MSGetResource(char *type4, int id)
{
    char type[5];
    DWORD name;
    unsigned i;
    unsigned h;

    type[0] = type4[0];
    type[1] = type4[1];
    type[2] = type4[2];
    type[3] = type4[3];
    type[4] = 0;
    name = 0x00000000L | (DWORD)(WORD)(id);
    i = -1;
    if ((h = FindResource(g_hinstance, (LPCSTR)name, type)) == 0) {
        for (i = 0; i < g_resource_lib_count; i++) {
            if ((h = FindResource(g_resource_libs[i], (LPCSTR)name, type)) != 0)
                break;
        }
    }
    if (h != 0)
        return (void *)(DWORD)LoadResource(i == -1 ? g_hinstance : g_resource_libs[i], h);
    else
        return 0;
}
