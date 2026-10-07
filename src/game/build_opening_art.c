/* PORT: this file's own declarations of load_gif_file removed: the header declares it from the definition (W1 step 3). */
/* build_opening_art: candidate for 1060:7fb2 (1111 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: build_opening_art
 *
 * A DEVELOPMENT TOOL left in the shipped image: it rebuilds the opening credits' sprite files from the
 * loose GIFs beside the sources. Nothing calls it.
 *
 * It opens `coldata5.dll`, loads `..\resource\logo.gif` into a port, remaps it onto the game palette
 * (SEG20:0x1ac), takes the pixel at (0, 0) as the transparent colour and cuts the whole port out as the
 * logo sprite, centred on its own width.
 *
 * Then 21 banners: `opencrd1.gif` for banners 0..6, `opencrd2.gif` for 7..13 and `opencrd3.gif` for
 * 14..20. Each banner is cut from the position in the 6-byte records of resource 0x251f, with 0xf9 as the
 * transparent colour and 0xf3 as the key -- except banner 2, which takes its transparent colour from the
 * pixel at (105, 426). Every banner is remapped and centred on both axes.
 *
 * Finally the sprites are written out: the logo to `..\resource\ablogo.spr` and the banners to
 * `..\resource\abban<N>.spr`, the name built in the module's message buffer.
 *
 * `GetPixel` is a FAR call here where the other helpers of its module are (rule 47), and the
 * palette type ids at DGROUP:0x29c4 and 0x29c8 are both copied to the stack though only the second is
 * used.
 * ResourceFile's constructor and destructor are defined before the function, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
/* the About box: its logo at +0x8bc and the 21 letter banners after it (AboutWin.letters) */
#include "classes.h"

struct logpal;

extern void MSLockGraf(GRPort *port);
extern void palette_remap_table(struct logpal *pal, unsigned char *out, struct logpal *ct, short first, short n);
#include "records.h"
extern void port_remap_pixels(Port *p, unsigned char *table, short base, short count);
extern char *GRPort_PixelMap(GRPort *port, int x, int y);
extern void Sprite_Extract_key_rect(Sprite *sprite, void *src, unsigned char transparent, RECT *r);
extern void Sprite_SetHotSpot(Sprite *s, int x, int y);
extern void sprite_extract_bounds(Sprite *s, GRPort *p,
                                                unsigned char transparent, unsigned char key,
                                                int x, int y);
extern void Sprite_ChangeColor_1068_7c97(Sprite *s, unsigned char *map, int first,
                                             int count);
extern void sprite_write_file(Sprite *spr, char *name);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *MSGetResource_1060_8884(void *type, int id);
extern "C" void *MSLockResource_1060_88a7(void *p);
extern "C" int port_width_1060_86c9(GRPort *port);
extern "C" int port_height_1060_86e2(GRPort *port);

void build_opening_art(AboutWin *o)
{
    ResourceFile rf("coldata5.dll");
    CTable ct;
    GRPort port;
    RECT rc;
    unsigned char transparent;
    unsigned char *pix;
    void *res;
    Spot *spots;
    long type_a;
    long type_b;
    unsigned char remap[0x100];
    register int i;

    type_a = g_restype_cred;
    type_b = g_restype_crds;
    res = MSGetResource_1060_8884(&type_b, 0x251f);
    spots = (Spot *)MSLockResource_1060_88a7(res);
    load_gif_file(&port, "..\\resource\\logo.gif", 10, 0xec, &ct);
    MSLockGraf(&port);
    palette_remap_table((struct logpal *)&g_game_palette, remap, (struct logpal *)&ct, 10, 0xec);
    port_remap_pixels((Port *)&port, remap, 10, 0xec);
    pix = (unsigned char *)GRPort_PixelMap(&port, 0, 0);
    transparent = *pix;
    SetRect(&rc, 0, 0, port_width_1060_86c9(&port), port_height_1060_86e2(&port));
    Sprite_Extract_key_rect(&o->logo, &port, transparent, &rc);
    Sprite_SetHotSpot(&o->logo, -(port_width_1060_86c9(&port) >> 1), 0);
    GRPort_Init_wh_1060_88c7(&port);
    for (i = 0; i < 0x15; i++) {
        if (i == 0) {
            load_gif_file(&port, "..\\resource\\opencrd1.gif", 10, 0xec, &ct);
            palette_remap_table((struct logpal *)&g_game_palette, remap, (struct logpal *)&ct, 10, 0xec);
            MSLockGraf(&port);
        } else if (i == 7) {
            GRPort_Init_wh_1060_88c7(&port);
            load_gif_file(&port, "..\\resource\\opencrd2.gif", 10, 0xec, &ct);
            palette_remap_table((struct logpal *)&g_game_palette, remap, (struct logpal *)&ct, 10, 0xec);
            MSLockGraf(&port);
        } else if (i == 0xe) {
            GRPort_Init_wh_1060_88c7(&port);
            load_gif_file(&port, "..\\resource\\opencrd3.gif", 10, 0xec, &ct);
            palette_remap_table((struct logpal *)&g_game_palette, remap, (struct logpal *)&ct, 10, 0xec);
            MSLockGraf(&port);
        }
        if (i == 2)
            sprite_extract_bounds(&o->letters[i], &port,
                                            GetPixel(&port, 0x69, 0x1aa), 0xf3, 0x69, 0x1aa);
        else
            sprite_extract_bounds(&o->letters[i], &port, 0xf9, 0xf3,
                                            spots[i].x, spots[i].y);
        Sprite_ChangeColor_1068_7c97(&o->letters[i], remap, 10, 0xec);
        Sprite_SetHotSpot(&o->letters[i], -(rect_width_1060_88e7(&o->letters[i]) >> 1),
                          -(rect_height_1060_8906(&o->letters[i]) >> 1));
    }
    GRPort_Init_wh_1060_88c7(&port);
    sprite_write_file(&o->logo, "..\\resource\\ablogo.spr");
    for (i = 0; i < 0x15; i++) {
        message_buf_set_1060_8925("..\\resource\\abban");
        say_number_1060_8947(i);
        message_buf_cat_1060_897c(".spr");
        sprite_write_file(&o->letters[i], message_buf_1060_899e());
    }
}

