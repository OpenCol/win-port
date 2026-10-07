/* UnitIcon: candidate for 1040:1de3 (358 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Picks the picture for a unit. The default is the unit type's own icon, the icon byte of the 14-byte
 * record at SEG20:0x7ac8; a plain colonist (type 0) goes to `ColonistIcon`
 * instead; and then six OVERRIDES fire, one per type, each of them "this type, but NOT its
 * expected speciality".
 *
 * Type 2 without speciality 0x14 draws 0x4a, type 1 without 0x15 draws 0x4b, type 4 without
 * 0x15 draws 0x4d, type 5 without 0x16 draws 0x4c, type 3 without 0x18 draws 0x4e. So each of
 * those five types has one speciality that keeps the type's own icon and every other
 * speciality gets a distinct substitute -- a colonist carrying the type's equipment, drawn
 * differently from the specialist who owns it.
 *
 * The sixth is not about specialities at all: type 0xb with bit 7 of +4 set draws 0x42.
 *
 * SIX SEPARATE `if`s AND NOT AN `else if` CHAIN. Every one is tested even after an earlier one
 * has matched, which is visible as six independent forward jumps to six different labels
 * rather than one join.
 *
 * `icon` IS NOT `register` AND THE PARAMETER IS. Both end up in registers -- the function has
 * no frame -- but the parameter takes SI and the local DI, which is the opposite of what two
 * `register` declarations give. A promoted plain local is allocated after a register parameter;
 * see the note under rule 53.
 *
 * Every speciality comparison carries rule 51's written-out `(int)` cast, `CBW` then `CMP AX`;
 * the type comparisons, against a plain `unsigned char`, do not.
 *
 * RENAMED 2026-09-26 from unit_portrait: the Mac build's UnitIcon(short), bound by call-sequence alignment (7.0 aligned gaps in ColonyWin_ColonyClickUnits, ColonyWin_DrawColonyUnits, ReportWin_Score leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.96x this one.
 */

#include "colonization.h"

#include "jobs.h"
#include "units.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern int ColonistIcon(int spec);

int UnitIcon(register int u)
{
    int icon;

    icon = g_unit_types[g_units[u].type].icon;
    if (g_units[u].type == UNIT_COLONISTS)
        icon = ColonistIcon((int)g_units[u].spec);
    if (g_units[u].type == UNIT_PIONEERS && (int)g_units[u].spec != JOB_PIONEER)
        icon = 0x4a;
    if (g_units[u].type == UNIT_SOLDIERS && (int)g_units[u].spec != JOB_SOLDIER)
        icon = 0x4b;
    if (g_units[u].type == UNIT_DRAGOONS && (int)g_units[u].spec != JOB_SOLDIER)
        icon = 0x4d;
    if (g_units[u].type == UNIT_SCOUTS && (int)g_units[u].spec != JOB_SCOUT)
        icon = 0x4c;
    if (g_units[u].type == UNIT_MISSIONARIES && (int)g_units[u].spec != JOB_MISSIONARY)
        icon = 0x4e;
    if (g_units[u].type == UNIT_ARTILLERY && (g_units[u].flags2 & 0x80))
        icon = 0x42;
    return icon;
}
