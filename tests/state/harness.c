/* port_state: the native side of tests/state.py (roadmap W3).
 *
 *   port_state.exe layout             each modelled global (name, address, element size, count)
 *                                     and each class's size, bases and members, as JSON
 *   port_state.exe roundtrip IN OUT   load the globals from IN, then write them all to OUT
 *   port_state.exe dump OUT           write the globals as the program starts them
 *
 * Linked with every game object (tests/state/build.py), so the globals are the game's own
 * storage. IN and OUT hold records: name, NUL, a 4-byte little-endian length, the bytes.
 * The table of globals is generated (build/state/table.c, from tests/state/build.py).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PortGlobal { const char *name; void *addr; unsigned elem; unsigned count; };
extern const struct PortGlobal port_globals[];
extern const unsigned port_global_count;
struct PortField { const char *cls; const char *what; const char *name; unsigned off; unsigned size; };
extern const struct PortField port_fields[];
extern const unsigned port_field_count;

static const struct PortGlobal *find(const char *name)
{
    unsigned i;
    for (i = 0; i < port_global_count; i++)
        if (strcmp(port_globals[i].name, name) == 0)
            return &port_globals[i];
    return 0;
}

static int load(const char *path)
{
    FILE *f = fopen(path, "rb");
    char name[256];
    unsigned n = 0;
    if (!f) {
        fprintf(stderr, "cannot open %s\n", path);
        return 1;
    }
    for (;;) {
        int c, i = 0;
        unsigned char len[4];
        unsigned size;
        const struct PortGlobal *g;
        while ((c = fgetc(f)) != EOF && c != 0 && i < 255)
            name[i++] = (char)c;
        if (c == EOF)
            break;
        name[i] = 0;
        if (fread(len, 1, 4, f) != 4)
            break;
        size = len[0] | len[1] << 8 | len[2] << 16 | (unsigned)len[3] << 24;
        g = find(name);
        if (!g || !g->count || size != g->elem * g->count) {
            fprintf(stderr, "load: %s: %s\n", name, g ? "size differs" : "unknown");
            fseek(f, size, SEEK_CUR);
            continue;
        }
        if (fread(g->addr, 1, size, f) != size)
            break;
        n++;
    }
    fclose(f);
    fprintf(stderr, "loaded %u globals\n", n);
    return 0;
}

static int dump(const char *path)
{
    FILE *f = fopen(path, "wb");
    unsigned i;
    if (!f)
        return 1;
    for (i = 0; i < port_global_count; i++) {
        const struct PortGlobal *g = &port_globals[i];
        if (!g->count)
            continue;                     /* known by its address only */
        unsigned size = g->elem * g->count;
        unsigned char len[4] = { (unsigned char)size, (unsigned char)(size >> 8),
                                 (unsigned char)(size >> 16), (unsigned char)(size >> 24) };
        fwrite(g->name, 1, strlen(g->name) + 1, f);
        fwrite(len, 1, 4, f);
        fwrite(g->addr, 1, size, f);
    }
    fclose(f);
    return 0;
}

/* MinGW runs the C++ static constructors from __main(), which GCC calls first thing in main.
   The game's constructors build windows and fonts through the platform layer (W4); the harness
   only moves bytes, so this empty __main, linked before libgcc's, keeps them from running. */
void __main(void)
{
}

int main(int argc, char **argv)
{
    unsigned i;
    if (argc >= 2 && strcmp(argv[1], "layout") == 0) {
        printf("{\"globals\": [\n");
        for (i = 0; i < port_global_count; i++)
            printf("  [\"%s\", %lu, %u, %u]%s\n", port_globals[i].name,
                   (unsigned long)port_globals[i].addr, port_globals[i].elem, port_globals[i].count,
                   i + 1 < port_global_count ? "," : "");
        printf("], \"fields\": [\n");
        for (i = 0; i < port_field_count; i++)
            printf("  [\"%s\", \"%s\", \"%s\", %u, %u]%s\n", port_fields[i].cls, port_fields[i].what,
                   port_fields[i].name, port_fields[i].off, port_fields[i].size,
                   i + 1 < port_field_count ? "," : "");
        printf("]}\n");
        return 0;
    }
    if (argc == 4 && strcmp(argv[1], "roundtrip") == 0)
        return load(argv[2]) || dump(argv[3]);
    if (argc == 3 && strcmp(argv[1], "dump") == 0)
        return dump(argv[2]);             /* the globals as the executable starts them */
    fprintf(stderr, "usage: port_state layout | roundtrip IN OUT | dump OUT\n");
    return 2;
}
