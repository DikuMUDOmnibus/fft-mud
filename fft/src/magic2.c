 

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"


extern char *target_name;

void spell_farsight (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    do_function (ch, &do_scan, target_name);
}


void spell_portal (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal;

    if ((victim = get_char_world (ch, target_name)) == NULL
        || victim == ch
        || victim->in_room == NULL
        || !can_see_room (ch, victim->in_room)
        || IS_SET (victim->in_room->room_flags, ROOM_SAFE)
        || IS_SET (victim->in_room->room_flags, ROOM_PRIVATE)
        || IS_SET (victim->in_room->room_flags, ROOM_SOLITARY)
        || IS_SET (victim->in_room->room_flags, ROOM_NO_RECALL)
        || IS_SET (ch->in_room->room_flags, ROOM_NO_RECALL)
        || victim->level >= level + 3 || (!IS_NPC (victim) && victim->level >= LEVEL_HERO)    /* NOT trust */
        || (IS_NPC (victim) && IS_SET (victim->imm_flags, IMM_SUMMON))
        || (IS_NPC (victim) && saves_spell (level, victim, ELE_NON))
        || (is_clan (victim) && !is_same_clan (ch, victim)))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }

    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 2 + level / 25;
    portal->value[3] = victim->in_room->vnum;

    obj_to_room (portal, ch->in_room);

    act ("$p rises up from the ground.", ch, portal, NULL, TO_ROOM);
    act ("$p rises up before you.", ch, portal, NULL, TO_CHAR);
}

void spell_nexus (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal;
    ROOM_INDEX_DATA *to_room, *from_room;

    from_room = ch->in_room;

    if ((victim = get_char_world (ch, target_name)) == NULL
        || victim == ch
        || (to_room = victim->in_room) == NULL
        || !can_see_room (ch, to_room) || !can_see_room (ch, from_room)
        || IS_SET (to_room->room_flags, ROOM_SAFE)
        || IS_SET (from_room->room_flags, ROOM_SAFE)
        || IS_SET (to_room->room_flags, ROOM_PRIVATE)
        || IS_SET (to_room->room_flags, ROOM_SOLITARY)
        || IS_SET (to_room->room_flags, ROOM_NO_RECALL)
        || IS_SET (from_room->room_flags, ROOM_NO_RECALL)
        || victim->level >= level + 3 || (!IS_NPC (victim) && victim->level >= LEVEL_HERO)    /* NOT trust */
        || (IS_NPC (victim) && IS_SET (victim->imm_flags, IMM_SUMMON))
        || (IS_NPC (victim) && saves_spell (level, victim, ELE_NON))
        || (is_clan (victim) && !is_same_clan (ch, victim)))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }

    /* portal one */
    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;

    obj_to_room (portal, from_room);

    act ("$p rises up from the ground.", ch, portal, NULL, TO_ROOM);
    act ("$p rises up before you.", ch, portal, NULL, TO_CHAR);

    /* no second portal if rooms are the same */
    if (to_room == from_room)
        return;

    /* portal two */
    portal = create_object (get_obj_index (OBJ_VNUM_PORTAL), 0);
    portal->timer = 1 + level / 10;
    portal->value[3] = from_room->vnum;

    obj_to_room (portal, to_room);

    if (to_room->people != NULL)
    {
        act ("$p rises up from the ground.", to_room->people, portal, NULL,
             TO_ROOM);
        act ("$p rises up from the ground.", to_room->people, portal, NULL,
             TO_CHAR);
    }
}
