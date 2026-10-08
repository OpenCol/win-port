/* KERNEL and USER resources on the NE reader (roadmap W4): LoadLibrary of the game's data DLLs,
 * FindResource/LoadResource/LockResource, and cursors, icons, bitmaps and menus made from
 * the NE files' bytes.
 *
 * A module handle is a token for one of ours; any other instance handle -- g_hinstance,
 * which a 32-bit HINSTANCE did not fit -- means COLONIZE.EXE itself, read from the working
 * directory (the game's). WinG.dll, which the game also loads, is emulated (w16_wing.c) and
 * gets a module with no file.
 */
#include <stdlib.h>
#include <string.h>
#include "w16.h"
#include "ne_res.h"

#define TOK(h) w16_token((HANDLE)(h))

typedef struct { uint32_t magic; NeModule *ne; } Module;
typedef struct { uint32_t magic; NeResource r; } Res;
#define MOD_MAGIC 0x4d4f4431u
#define RES_MAGIC 0x52455331u

static Module *self_module(void)
{
    static Module m = { MOD_MAGIC, 0 };
    if (!m.ne)
        m.ne = ne_open("COLONIZE.EXE");
    return &m;
}

static Module *module_of(unsigned hinst)
{
    Module *m = (Module *)w16_handle(hinst);
    if (hinst && m && m->magic == MOD_MAGIC)
        return m;
    return self_module();
}

static const char *rid(const char *p)
{
    /* a Win16 MAKEINTRESOURCE is a far pointer with segment 0: an integer under 0x10000 */
    return p;
}

unsigned w16_LoadLibrary(const char *name) W16(LoadLibrary);
unsigned w16_LoadLibrary(const char *name)
{
    Module *m = calloc(1, sizeof *m);
    m->magic = MOD_MAGIC;
    if (_stricmp(name, "WinG.dll") != 0 && _stricmp(name, "WING.DLL") != 0) {
        m->ne = ne_open(name);
        if (!m->ne) {
            w16_trace("LoadLibrary(%s): not found", name);
            free(m);
            return 2;                     /* Win16: < 32 is an error, 2 file not found */
        }
    }
    w16_trace("LoadLibrary(%s)", name);
    return TOK(m);
}

void w16_FreeLibrary(unsigned h) W16(FreeLibrary);
void w16_FreeLibrary(unsigned h)
{
    Module *m = (Module *)w16_handle(h);
    if (!m || m->magic != MOD_MAGIC || m == self_module())
        return;
    ne_close(m->ne);
    m->magic = 0;
    free(m);
    w16_forget(h);
}

unsigned w16_FindResource(unsigned hinst, const char *name, const char *type) W16(FindResource);
unsigned w16_FindResource(unsigned hinst, const char *name, const char *type)
{
    Module *m = module_of(hinst);
    Res *r;
    NeResource nr;
    if (!m->ne || !ne_find(m->ne, rid(type), rid(name), &nr))
        return 0;
    r = malloc(sizeof *r);
    r->magic = RES_MAGIC;
    r->r = nr;
    return TOK(r);
}

/* a loaded resource: its own copy, as Win16 gave each LoadResource its own segment */
unsigned w16_LoadResource(unsigned hinst, unsigned hres) W16(LoadResource);
unsigned w16_LoadResource(unsigned hinst, unsigned hres)
{
    Res *r = (Res *)w16_handle(hres);
    HGLOBAL g;
    (void)hinst;
    if (!r || r->magic != RES_MAGIC)
        return 0;
    g = GlobalAlloc(GMEM_FIXED, r->r.length ? r->r.length : 1);
    memcpy((void *)g, r->r.data, r->r.length);
    return TOK(g);
}

void *w16_LockResource(int h) W16(LockResource);
void *w16_LockResource(int h) { return h ? GlobalLock((HGLOBAL)w16_handle((unsigned)h)) : 0; }

int w16_FreeResource(int h) W16(FreeResource);
int w16_FreeResource(int h)
{
    GlobalFree((HGLOBAL)w16_handle((unsigned)h));
    w16_forget((unsigned)h);
    return 0;
}

/* ------------------------------------------------------------------ cursors, icons, bitmaps */

static HANDLE group_resource(unsigned hinst, const char *name, int cursor)
{
    Module *m = module_of(hinst);
    NeResource dir, one;
    int id;
    if (!m->ne || !ne_find(m->ne, MAKEINTRESOURCEA(cursor ? 12 : 14), name, &dir))
        return 0;
    id = LookupIconIdFromDirectoryEx((PBYTE)dir.data, !cursor, 0, 0, LR_DEFAULTCOLOR);
    if (!id || !ne_find(m->ne, MAKEINTRESOURCEA(cursor ? 1 : 3), MAKEINTRESOURCEA(id), &one))
        return 0;
    return CreateIconFromResourceEx((PBYTE)one.data, one.length, !cursor, 0x00030000, 0, 0, LR_DEFAULTCOLOR);
}

unsigned w16_LoadCursor(unsigned hinst, const char *name) W16(LoadCursor);
unsigned w16_LoadCursor(unsigned hinst, const char *name)
{
    if (!hinst)
        return TOK(LoadCursorA(0, name));  /* IDC_ARROW and the other system cursors */
    return TOK(group_resource(hinst, name, 1));
}

unsigned w16_LoadIcon(unsigned hinst, const char *name) W16(LoadIcon);
unsigned w16_LoadIcon(unsigned hinst, const char *name)
{
    if (!hinst)
        return TOK(LoadIconA(0, name));
    return TOK(group_resource(hinst, name, 0));
}

unsigned w16_LoadBitmap(unsigned hinst, const char *name) W16(LoadBitmap);
unsigned w16_LoadBitmap(unsigned hinst, const char *name)
{
    Module *m = module_of(hinst);
    NeResource r;
    const BITMAPINFOHEADER *bh;
    unsigned colors;
    HDC dc;
    HBITMAP b;
    if (!m->ne || !ne_find(m->ne, MAKEINTRESOURCEA(2), name, &r))
        return 0;
    bh = (const BITMAPINFOHEADER *)r.data;
    if (bh->biSize != sizeof(BITMAPINFOHEADER))
        return 0;                         /* an OS/2 core header: not in this game */
    colors = bh->biClrUsed ? bh->biClrUsed : (bh->biBitCount <= 8 ? 1u << bh->biBitCount : 0);
    dc = GetDC(0);
    b = CreateDIBitmap(dc, bh, CBM_INIT, (const char *)bh + bh->biSize + colors * 4, (const BITMAPINFO *)bh,
                       DIB_RGB_COLORS);
    ReleaseDC(0, dc);
    return TOK(b);
}

/* ------------------------------------------------------------------ menus */

/* A Win16 menu template: a header word pair, then items -- flags, an id unless MF_POPUP,
   a string; MF_END (0x80) closes a level. */
static const unsigned char *build_menu(HMENU menu, const unsigned char *p, const unsigned char *end)
{
    while (p < end) {
        uint16_t flags = (uint16_t)(p[0] | p[1] << 8);
        uint16_t id = 0;
        const char *s;
        p += 2;
        if (!(flags & MF_POPUP)) {
            id = (uint16_t)(p[0] | p[1] << 8);
            p += 2;
        }
        s = (const char *)p;
        p += strlen(s) + 1;
        if (flags & MF_POPUP) {
            HMENU sub = CreatePopupMenu();
            p = build_menu(sub, p, end);
            AppendMenuA(menu, (flags & ~(MF_END | MF_POPUP)) | MF_POPUP, (UINT_PTR)sub, s);
        } else if (!*s && !id && !(flags & ~MF_END)) {
            AppendMenuA(menu, MF_SEPARATOR, 0, 0);
        } else {
            AppendMenuA(menu, flags & ~MF_END, id, s);
        }
        if (flags & MF_END)
            break;
    }
    return p;
}

unsigned w16_LoadMenu(unsigned hinst, const char *name) W16(LoadMenu);
unsigned w16_LoadMenu(unsigned hinst, const char *name)
{
    Module *m = module_of(hinst);
    NeResource r;
    HMENU menu;
    if (!m->ne || !ne_find(m->ne, MAKEINTRESOURCEA(4), name, &r))
        return 0;
    menu = CreateMenu();
    build_menu(menu, r.data + 4, r.data + r.length);
    return TOK(menu);
}
