/* get_colony_name: candidate for 1028:a078 (284 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Picks the next unused colony name for a nation from the COLONY text resource: the
 * nation index selects the section (ENGLISH/FRENCH/SPANISH/DUTCH), the section is walked
 * one CSV line at a time until the "@" terminator, the first line is the fallback and the
 * line at the nation's used-name counter is the result; the counter then advances.
 * The output starts as "?" and is cut at 23 characters.
 *
 * RENAMED 2026-09-27 from next_colony_name: String witness: Mac get_colony_name(short, char*) has '?', COLONY, DUTCH, ENGLISH, FRENCH and SPANISH, and a078 has exactly the same six (score 78.26 against a runner-up of 14.57). Call graph: the Mac function's only caller is perform_orders_build, and a078's only caller is perform_orders_build (1028:ab1f). The Mac sequence text_open, text_get, text_item_get, text_close matches a078's callees text_open, text_get, text_item_get, text_close and STRCPY x7. Arity: (short, char*) against next_colony_name(int nation, char *out), where a Mac char* and a pointer are both 4 bytes. Size is 270 Mac against 284 Windows. The body picks a nation's next colony name from the COLONY text section. The old name is descriptive. Apply this only after 1028:d1ba has vacated the name.
 */

#include "colonization.h"

#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL, +2 counts colony names taken */

extern int text_open(void *type, char *section);
extern char *text_get(void);
extern char *text_item_get(void);
extern void text_close(void);

void get_colony_name(int nation, char *out)
{
    char section[20];
    char line[80];
    int i;

    STRCPY(out, "?");
    switch (nation) {
    case 0: STRCPY(section, "ENGLISH"); break;
    case 1: STRCPY(section, "FRENCH"); break;
    case 2: STRCPY(section, "SPANISH"); break;
    case 3: STRCPY(section, "DUTCH"); break;
    }
    if (text_open("COLONY", section))
        goto done;
    for (i = 0; g_nation_text[nation].colony_names_used >= i; i++) {
        text_get();
        STRCPY(line, text_item_get());
        if ((int)line[0] == '@')
            goto done;
        if (i == 0 || g_nation_text[nation].colony_names_used == i)
            STRCPY(out, line);
    }
    out[23] = 0;
    g_nation_text[nation].colony_names_used++;
done:
    text_close();
}
