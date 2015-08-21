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
 ***************************************************************************/

/***************************************************************************
 *  ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@hypercube.org)                                *
 *      Gabrielle Taylor (gtaylor@hypercube.org)                           *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Rom24/doc/rom.license                         *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "music.h"

/*
 * Local functions.
 */
void mobile_update args ((void));
void char_update args ((void));
void obj_update args ((void));
void ai_move(CHAR_DATA *ch);
/* used for saving */

int save_number = 0;



/*
 * Advancement stuff.
 */
void advance_level (CHAR_DATA * ch, bool hide)
{
    double cache;
    
    float m1,m2,m3,m4,m5;

    if(ch->sex == SEX_MALE)
    { 
    m1 = 3.5;
    m2 = 3.2;
    m3 = 2.5;
    m4 = 3.2;
    m5 = 2.4;
    }
    else
    {
    m1 = 3.2;
    m2 = 3.5;
    m3 = 2.6;
    m4 = 2.4;
    m5 = 3.4;
    }

    ch->level++;
    cache = (ch->HPP / ( job_table[ch->jobid].classhpmult + ch->level));
    ch->HPP += cache*m1;
    cache = (ch->MPP / ( job_table[ch->jobid].classmpmult + ch->level));
    ch->MPP += cache*m2;
    cache = (ch->SpP / ( job_table[ch->jobid].classspmult + ch->level));
    ch->SpP += cache*m3;
    cache = (ch->PAP / ( job_table[ch->jobid].classpamult + ch->level));
    ch->PAP += cache*m4;
    cache = (ch->MAP / ( job_table[ch->jobid].classmamult + ch->level));
    ch->MAP += cache*m5;

    if(!hide)
    send_to_char("{GYou gain a level!!\n\r{x",ch);
    set_stats(ch);
    return;
}

void gain_jp (CHAR_DATA * ch, int gain)
{

    if(gain > 99)
    gain = 99;
    
    ch->job_jp[ch->jobid] += gain;
    ch->job_top_jp[ch->jobid] += gain; 
    
    if (IS_NPC (ch) || ch->job_level[ch->jobid] >= 8)
        return; 
         
    if(ch->job_top_jp[ch->jobid] > ((100 * ch->job_level[ch->jobid])+1))
    {
    send_to_char ("{BJob level up!! {x", ch);
    ch->job_level[ch->jobid]++;
    }
    
    return;
}

void gain_exp (CHAR_DATA * ch, int gain)
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC (ch) || ch->level >= LEVEL_HERO)
        return;

    if(gain > 99)
    gain = 99;
    ch->exp += gain;

    while (ch->level < LEVEL_HERO && ch->exp >=
           exp_per_level (ch, ch->pcdata->points) * (ch->level + 1))
    {
        send_to_char ("{GYou raise a level!!  {x", ch);
        ch->level += 1;
        sprintf (buf, "%s gained level %d", ch->name, ch->level);
        log_string (buf);
        sprintf (buf, "$N has attained level %d!", ch->level);
        wiznet (buf, ch, NULL, WIZ_LEVELS, 0, 0);
        advance_level (ch, FALSE);
        save_char_obj (ch);
    }

    return;
}




void gain_condition (CHAR_DATA * ch, int iCond, int value)
{
/*heh i hate this function*/        return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update (void)
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *rch;

    /* Examine all mobs. */
    for (ch = char_list; ch != NULL; ch = ch_next)
    {
        ch_next = ch->next;

        if (!IS_NPC (ch) || ch->in_room == NULL) continue;

        if (ch->in_room->area->empty && !IS_SET (ch->act, ACT_UPDATE_ALWAYS))
            continue;

        if (ch->pIndexData->pShop != NULL)    /* give him some gold */
            if ((ch->gold * 100 + ch->silver) < ch->pIndexData->wealth)
            {
                ch->gold +=
                    ch->pIndexData->wealth * number_range (1, 20) / 5000000;
                ch->silver +=
                    ch->pIndexData->wealth * number_range (1, 20) / 50000;
            }


        /* That's all for sleeping / busy monster, and empty zones */
        if (ch->position != POS_STANDING)
            continue;
            
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    if(rch->infight && !ch->infight && IS_NPC(ch) && !ch->pIndexData->pShop)
    {
    do_autodisp(ch," ");
    do_join(ch,"none");
    }
    }

            if(IS_NPC(ch) && ch->infight && TURN(ch))
            ai_move(ch);
        
    }

    return;
}



/*
 * Update all chars, including mobs.
*/
void char_update (void)
{
    CHAR_DATA *ch = NULL;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;

    ch_quit = NULL;

    /* update save counter */
    save_number++;

    if (save_number > 29)
        save_number = 0;

    for (ch = char_list; ch != NULL; ch = ch_next)
    {
        AFFECT_DATA *paf;
        AFFECT_DATA *paf_next;

        ch_next = ch->next;

    if(!ch->infight)
{
            ch->hit = ch->max_hit;
            ch->mana = ch->max_mana;
            ch->move = ch->max_move;
            update_pos (ch);
}
        if (ch->timer > 30)
            ch_quit = ch;

        if (ch->position == POS_STUNNED)
            update_pos (ch);

        if (!IS_NPC (ch) && ch->level < LEVEL_IMMORTAL)
        {

            if (IS_IMMORTAL (ch))
                ch->timer = 0;

            if (++ch->timer >= 12)
            {
                if (ch->was_in_room == NULL && ch->in_room != NULL)
                {
                    ch->was_in_room = ch->in_room;
                    act ("$n disappears into the void.",
                         ch, NULL, NULL, TO_ROOM);
                    send_to_char ("You disappear into the void.\n\r", ch);
                    if (ch->level > 1)
                        save_char_obj (ch);
                    char_from_room (ch);
                    char_to_room (ch, get_room_index (2));
                }
            }

            gain_condition (ch, COND_DRUNK, -1);
            gain_condition (ch, COND_FULL, ch->size > SIZE_MEDIUM ? -4 : -2);
            gain_condition (ch, COND_THIRST, -1);
            gain_condition (ch, COND_HUNGER,
                            ch->size > SIZE_MEDIUM ? -2 : -1);
        }

        for (paf = ch->affected; paf != NULL; paf = paf_next)
        {
            paf_next = paf->next;
            if (paf->duration > 0)
            {
                paf->duration--;
                if (number_range (0, 4) == 0 && paf->level > 0)
                    paf->level--;    /* spell strength fades with time */
            }
            else if (paf->duration < 0);
            else
            {
                if (paf_next == NULL
                    || paf_next->type != paf->type || paf_next->duration > 0)
                {
                    if (paf->type > 0 && skill_table[paf->type].msg_off)
                    {
                        send_to_char (skill_table[paf->type].msg_off, ch);
                        send_to_char ("\n\r", ch);
                    }
                }

                affect_remove (ch, paf);
            }
        }

        /*
         * Careful with the damages here,
         *   MUST NOT refer to ch after damage taken,
         *   as it may be lethal damage (on NPC).
         */

        if (ch->position == POS_INCAP && number_range (0, 1) == 0)
        {
            damage (ch, ch, 1, TYPE_UNDEFINED, ELE_NON, FALSE);
        }
        else if (ch->position == POS_MORTAL)
        {
            damage (ch, ch, 1, TYPE_UNDEFINED, ELE_NON, FALSE);
        }
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    for (ch = char_list; ch != NULL; ch = ch_next)
    {
    	/*
    	 * Edwin's fix for possible pet-induced problem
    	 * JR -- 10/15/00
    	 */
    	if (!IS_VALID(ch))
    	{
        	bug("update_char: Trying to work with an invalidated character.\n",0); 
        	break;
     	}

        ch_next = ch->next;

        if (ch->desc != NULL && ch->desc->descriptor % 30 == save_number)
        {
            save_char_obj (ch);
        }

        if (ch == ch_quit)
        {
            do_function (ch, &do_quit, "");
        }
    }

    return;
}




/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update (void)
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for (obj = object_list; obj != NULL; obj = obj_next)
    {
        CHAR_DATA *rch;
        char *message;

        obj_next = obj->next;

        if (obj->timer <= 0 || --obj->timer > 0)
            continue;

        switch (obj->item_type)
        {
            default:
                message = "$p crumbles into dust.";
                break;
            case ITEM_FOUNTAIN:
                message = "$p dries up.";
                break;
            case ITEM_CORPSE_NPC:
                message = "$p decays into dust.";
                break;
            case ITEM_CORPSE_PC:
                message = "$p decays into dust.";
                break;
            case ITEM_FOOD:
                message = "$p decomposes.";
                break;
            case ITEM_POTION:
            case ITEM_HEAL_FIXED:
                message = "$p has evaporated from disuse.";
                break;
            case ITEM_PORTAL:
                message = "$p fades out of existence.";
                break;
            case ITEM_CONTAINER:
                    message = "$p crumbles into dust.";
                break;
        }

        if (obj->carried_by != NULL)
        {
            if (IS_NPC (obj->carried_by)
                && obj->carried_by->pIndexData->pShop != NULL)
                obj->carried_by->silver += obj->cost / 5;
            else
            {
                act (message, obj->carried_by, obj, NULL, TO_CHAR);
            }
        }
        else if (obj->in_room != NULL && (rch = obj->in_room->people) != NULL)
        {
            if (!(obj->in_obj 
                  && !CAN_WEAR (obj->in_obj, ITEM_TAKE)))
            {
                act (message, rch, obj, NULL, TO_ROOM);
                act (message, rch, obj, NULL, TO_CHAR);
            }
        }

        if ((obj->item_type == ITEM_CORPSE_PC )
            && obj->contains)
        {                        /* save the contents */
            OBJ_DATA *t_obj, *next_obj;

            for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
            {
                next_obj = t_obj->next_content;
                obj_from_obj (t_obj);

                if (obj->in_obj)    /* in another object */
                    obj_to_obj (t_obj, obj->in_obj);

                else if (obj->carried_by)    /* carried */
                        if (obj->carried_by->in_room == NULL)
                            extract_obj (t_obj);
                        else
                            obj_to_room (t_obj, obj->carried_by->in_room);
                    else
                        obj_to_char (t_obj, obj->carried_by);

                 if (obj->in_room == NULL)    /* destroy it */
                    extract_obj (t_obj);
                else            /* to a room */
                    obj_to_room (t_obj, obj->in_room);
            }
        }

        extract_obj (obj);
    }

    return;
}



/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler (void)
{
    static int pulse_area;
    static int pulse_mobile;
    static int pulse_violence;
    static int pulse_point;
    static int pulse_music;

    if (--pulse_area <= 0)
    {
        pulse_area = PULSE_AREA;
        /* number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 ); */
        area_update ();
    }

    if (--pulse_music <= 0)
    {
        pulse_music = PULSE_MUSIC;
        song_update ();
    }

    if (--pulse_mobile <= 0)
    {
        pulse_mobile = PULSE_MOBILE;
        mobile_update ();
    }

    if (--pulse_violence <= 0)
    {
        pulse_violence = PULSE_VIOLENCE;
//        violence_update ();
    }

    if (--pulse_point <= 0)
    {
        wiznet ("TICK!", NULL, NULL, WIZ_TICKS, 0, 0);
        pulse_point = PULSE_TICK;
/* number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 ); */
        //weather_update ();
        char_update ();
        obj_update ();
    }

    tail_chain ();
    return;
}
