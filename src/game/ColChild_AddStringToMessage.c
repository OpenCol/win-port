/* ColChild_AddStringToMessage: candidate for 1048:10ef (60 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends s and a space to the message text at +0x149 of the object.
 *
 * RENAMED 2026-09-27 from message_append: 8 bytes, STRCAT, called by AddTo/AddNumber/AddBucks/move_unit on both sides; mac_bind votes 8/2
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ColChild_AddStringToMessage(ColChild *w, char *s)
{
    STRCAT(w->own_msg, s);
    STRCAT(w->own_msg, " ");
}
