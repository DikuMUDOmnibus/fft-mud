/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 **************************************************************************/

/***************************************************************************
 *   ROM 2.4 is copyright 1993-1998 Russ Taylor                            *
 *   ROM has been brought to you by the ROM consortium                     *
 *       Russ Taylor (rtaylor@hypercube.org)                               *
 *       Gabrielle Taylor (gtaylor@hypercube.org)                          *
 *       Brian Moore (zump@rom.org)                                        *
 *   By using this code, you have agreed to follow the terms of the        *
 *   ROM license, in the file Rom24/doc/rom.license                        *
 **************************************************************************/
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
#include "recycle.h"

void acid_effect (void *vo, int level, int dam, int target)
{
    if (target == TARGET_ROOM)
    {                            /* nail objects on the floor */
        ROOM_INDEX_DATA *room = (ROOM_INDEX_DATA *) vo;
        OBJ_DATA *obj, *obj_next;

        for (obj = room->contents; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            acid_effect (obj, level, dam, TARGET_OBJ);
        }
        return;
    }

    if (target == TARGET_CHAR)
    {                            /* do the effect on a victim */
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        OBJ_DATA *obj, *obj_next;

        /* let's toast some gear */
        for (obj = victim->carrying; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            acid_effect (obj, level, dam, TARGET_OBJ);
        }
        return;
    }

    if (target == TARGET_OBJ)
    {                            /* toast an object */
        OBJ_DATA *obj = (OBJ_DATA *) vo;
        OBJ_DATA *t_obj, *n_obj;
        int chance;
        char *msg;

        if (IS_OBJ_STAT (obj, ITEM_BURN_PROOF)
            || IS_OBJ_STAT (obj, ITEM_NOPURGE) || number_range (0, 4) == 0)
            return;

        chance = level / 4 + dam / 10;

        if (chance > 25)
            chance = (chance - 25) / 2 + 25;
        if (chance > 50)
            chance = (chance - 50) / 2 + 50;

        if (IS_OBJ_STAT (obj, ITEM_BLESS))
            chance -= 5;

        chance -= obj->level * 2;

        switch (obj->item_type)
        {
            default:
                return;
            case ITEM_CONTAINER:
            case ITEM_CORPSE_PC:
            case ITEM_CORPSE_NPC:
                msg = "$p fumes and dissolves.";
                break;
            case ITEM_ARMOR:
                msg = "$p is pitted and etched.";
                break;
            case ITEM_CLOTHING:
                msg = "$p is corroded into scrap.";
                break;
            case ITEM_STAFF:
            case ITEM_WAND:
                chance -= 10;
                msg = "$p corrodes and breaks.";
                break;
            case ITEM_SCROLL:
                chance += 10;
                msg = "$p is burned into waste.";
                break;
        }

        chance = URANGE (5, chance, 95);

        if (number_percent () > chance)
            return;

        if (obj->carried_by != NULL)
            act (msg, obj->carried_by, obj, NULL, TO_ALL);
        else if (obj->in_room != NULL && obj->in_room->people != NULL)
            act (msg, obj->in_room->people, obj, NULL, TO_ALL);

        if (obj->item_type == ITEM_ARMOR)
        {                        /* etch it */
            AFFECT_DATA *paf;
            bool af_found = FALSE;
            int i;

            affect_enchant (obj);

            for (paf = obj->affected; paf != NULL; paf = paf->next)
            {
                if (paf->location == APPLY_AC)
                {
                    af_found = TRUE;
                    paf->type = -1;
                    paf->modifier += 1;
                    paf->level = UMAX (paf->level, level);
                    break;
                }
            }

            if (!af_found)
                /* needs a new affect */
            {
                paf = new_affect ();

                paf->type = -1;
                paf->level = level;
                paf->duration = -1;
                paf->location = APPLY_AC;
                paf->modifier = 1;
                paf->bitvector = 0;
                paf->next = obj->affected;
                obj->affected = paf;
            }

            if (obj->carried_by != NULL && obj->wear_loc != WEAR_NONE)
                for (i = 0; i < 4; i++)
                    obj->carried_by->armor[i] += 1;
            return;
        }

        /* get rid of the object */
        if (obj->contains)
        {                        /* dump contents */
            for (t_obj = obj->contains; t_obj != NULL; t_obj = n_obj)
            {
                n_obj = t_obj->next_content;
                obj_from_obj (t_obj);
                if (obj->in_room != NULL)
                    obj_to_room (t_obj, obj->in_room);
                else if (obj->carried_by != NULL)
                    obj_to_room (t_obj, obj->carried_by->in_room);
                else
                {
                    extract_obj (t_obj);
                    continue;
                }

                acid_effect (t_obj, level / 2, dam / 2, TARGET_OBJ);
            }
        }

        extract_obj (obj);
        return;
    }
}


