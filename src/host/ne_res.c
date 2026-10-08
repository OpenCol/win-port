/* The NE resource reader (roadmap W4, section 1: "read directly from the NE resource DLLs").
 *
 * Win32 cannot load a 16-bit module, so the game's resources -- COLONIZE.EXE's own and the
 * COLDATA*.DLL / COLTEXT0.DLL ones -- are read from the files: the NE header's resource
 * table lists each type, and under it each resource's offset and length (in units of the
 * alignment shift) and its id, an integer (high bit set) or a name in the table.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ne_res.h"

static uint16_t rd16(const unsigned char *p) { return (uint16_t)(p[0] | p[1] << 8); }
static uint32_t rd32(const unsigned char *p) { return rd16(p) | (uint32_t)rd16(p + 2) << 16; }

NeModule *ne_open(const char *path)
{
    FILE *f = fopen(path, "rb");
    NeModule *m;
    long size;
    if (!f)
        return 0;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    m = calloc(1, sizeof *m);
    m->data = malloc((size_t)size);
    m->size = (size_t)size;
    if (fread(m->data, 1, m->size, f) != m->size || m->size < 0x40) {
        fclose(f);
        ne_close(m);
        return 0;
    }
    fclose(f);
    m->ne = rd16(m->data + 0x3c);
    if (m->ne + 0x40 > m->size || memcmp(m->data + m->ne, "NE", 2) != 0) {
        ne_close(m);
        return 0;
    }
    m->rsrc = m->ne + rd16(m->data + m->ne + 0x24);
    snprintf(m->path, sizeof m->path, "%s", path);
    return m;
}

void ne_close(NeModule *m)
{
    if (!m)
        return;
    free(m->data);
    free(m);
}

/* `want` is an integer id (< 0x10000) or a name; a name "#123" means the integer 123 */
static int matches(const NeModule *m, uint16_t id, const char *want)
{
    uintptr_t w = (uintptr_t)want;
    if (w < 0x10000)
        return (id & 0x8000) && (id & 0x7fff) == (uint16_t)w;
    if (want[0] == '#')
        return (id & 0x8000) && (id & 0x7fff) == (uint16_t)atoi(want + 1);
    if (id & 0x8000)
        return 0;
    {
        const unsigned char *s = m->data + m->rsrc + id;
        size_t n = s[0];
        return strlen(want) == n && _strnicmp((const char *)s + 1, want, n) == 0;
    }
}

static int find_ordinal(const NeModule *m, const char *type, const char *name, NeResource *out);

/* RT_NAMETABLE (type 15): Win16's KERNEL resolved a string type -- the game's "CvPc", "SPRT" --
   through each module's name table, entries of {size, type ordinal, id ordinal, type name,
   id name}: a name, case-insensitive, stands for the ordinals. */
static int by_name_table(const NeModule *m, const char *type, const char *name, NeResource *out)
{
    NeResource nt;
    uint32_t q = 0;
    uintptr_t w = (uintptr_t)name;
    if (!find_ordinal(m, (const char *)(uintptr_t)15, 0, &nt))
        return 0;
    while (q + 6 <= nt.length) {
        const unsigned char *e = nt.data + q;
        uint16_t size = rd16(e), tord = rd16(e + 2), iord = rd16(e + 4);
        const char *tname = (const char *)e + 6;
        const char *iname = tname + strlen(tname) + 1;
        if (!size)
            break;
        if (_stricmp(tname, type) == 0) {
            /* the type name names an ordinal type; an integer id is then looked up in it (any
               entry gives the type), a named one needs its own entry */
            if (w < 0x10000) {
                if (find_ordinal(m, (const char *)(uintptr_t)(tord & 0x7fff), name, out))
                    return 1;
            } else if (*iname && _stricmp(iname, name) == 0)
                return find_ordinal(m, (const char *)(uintptr_t)(tord & 0x7fff),
                                    (const char *)(uintptr_t)(iord & 0x7fff), out);
        }
        q += size;
    }
    return 0;
}

int ne_find(const NeModule *m, const char *type, const char *name, NeResource *out)
{
    if ((uintptr_t)type >= 0x10000 && type[0] != '#' && by_name_table(m, type, name, out))
        return 1;
    return find_ordinal(m, type, name, out);
}

/* name 0: the first resource of the type */
static int find_ordinal(const NeModule *m, const char *type, const char *name, NeResource *out)
{
    const unsigned char *p = m->data + m->rsrc;
    unsigned shift = rd16(p);
    p += 2;
    while (rd16(p)) {
        uint16_t tid = rd16(p), count = rd16(p + 2);
        const unsigned char *e = p + 8;
        int t_ok = matches(m, tid, type);
        unsigned i;
        for (i = 0; i < count; i++, e += 12) {
            if (t_ok && (!name || matches(m, rd16(e + 6), name))) {
                out->offset = (uint32_t)rd16(e) << shift;
                out->length = (uint32_t)rd16(e + 2) << shift;
                if (out->offset + out->length > m->size)
                    out->length = (uint32_t)(m->size - out->offset);
                out->data = m->data + out->offset;
                return 1;
            }
        }
        p = e;
    }
    return 0;
}
