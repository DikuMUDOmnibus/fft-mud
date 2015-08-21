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
*    ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
*    ROM has been brought to you by the ROM consortium                      *
*        Russ Taylor (rtaylor@hypercube.org)                                *
*        Gabrielle Taylor (gtaylor@hypercube.org)                           *
*        Brian Moore (zump@rom.org)                                         *
*    By using this code, you have agreed to follow the terms of the         *
*    ROM license, in the file Rom24/doc/rom.license                         *
****************************************************************************/

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

/*
 * Local functions.
 */
void do_autodisp (CHAR_DATA * ch, char *argument);
void check_assist args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_dodge args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_parry args ((CHAR_DATA * ch, CHAR_DATA * victim));
bool check_shield_block args ((CHAR_DATA * ch, CHAR_DATA * victim));
void dam_message args ((CHAR_DATA * ch, CHAR_DATA * victim, int dam,
                        int dt, bool immune));
void death_cry args ((CHAR_DATA * ch));
void group_gain args ((CHAR_DATA * ch, CHAR_DATA * victim));
int xp_compute args ((CHAR_DATA * gch, CHAR_DATA * victim, int total_levels));
bool is_safe args ((CHAR_DATA * ch, CHAR_DATA * victim));
void make_corpse args ((CHAR_DATA * ch));
void one_hit args ((CHAR_DATA * ch, CHAR_DATA * victim, int dt));
void mob_hit args ((CHAR_DATA * ch, CHAR_DATA * victim, int dt));
void raw_kill args ((CHAR_DATA * victim));
void set_fighting args ((CHAR_DATA * ch, CHAR_DATA * victim));
void disarm args ((CHAR_DATA * ch, CHAR_DATA * victim));


void f_multi_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
    one_hit (ch, victim, dt);//heh
    return;
}

bool check_evade(CHAR_DATA *ch, CHAR_DATA *ach)
{
    if(number_range(0,100) < 80)
    return TRUE;
    else return FALSE;
}


/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
    int number;

    one_hit (ch, victim, dt);

    /* oh boy!  Fun stuff! */

    if (ch->wait > 0)
        return;

    number = number_range (0, 2);

    if (number == 1 && IS_SET (ch->act, ACT_MAGE))
    {
        /*  { mob_cast_mage(ch,victim); return; } */ ;
    }

    if (number == 2 && IS_SET (ch->act, ACT_CLERIC))
    {
        /* { mob_cast_cleric(ch,victim); return; } */ ;
    }

    
}


/*
 * Hit one guy once.
 */
void one_hit (CHAR_DATA * ch, CHAR_DATA * victim, int dt)
{
    OBJ_DATA *wield;
    int dam;
    int sn; 
    int dam_type;
    bool result;

    sn = -1;


    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
        return;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
        return;

    /*
     * Figure out the type of damage message.
     */
    wield = get_eq_char (ch, WEAR_RIGHT);

    if (dt == TYPE_UNDEFINED)
    {
        dt = TYPE_HIT;
        if (wield != NULL && wield->item_type == ITEM_WEAPON)
            dt += wield->value[3];
        else
            dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
        if (wield != NULL)
            dam_type = attack_table[wield->value[3]].damage;
        else
            dam_type = attack_table[ch->dam_type].damage;
    else
        dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
        dam_type = ELE_NON;

/*if(!check_evade(victim,ch))
    {
        damage (ch, victim, 0, dt, dam_type, TRUE);
        tail_chain ();
        return;
    }
*/
    dam = get_w_dmg(ch);
    dam = dam/2;
    if(number_range(10,100) < 20)
    dam += dam/10;
    else if(number_range(10,100) < 40)
    dam -= dam/10;
    else if(number_range(10,100) < 70)
    dam -= (dam/(number_range(26,34)));
    else 
    dam += (dam/(number_range(26,34)));
    
    result = damage (ch, victim, dam, dt, dam_type, TRUE);

    tail_chain ();
    return;
}


/*
 * Inflict damage from a hit.
 */
bool damage (CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt,
             int dam_type, bool show)
{
    OBJ_DATA *corpse;
    int xp=0;
    int jp=0;
    bool immune = FALSE;
    if (victim->position == POS_DEAD)
        return FALSE;

    immune = FALSE;

    if (show)
        dam_message (ch, victim, dam, dt, immune);

    xp = xp_compute (ch, victim, 0);
    jp = jp_compute (ch, victim, 0);
        
    if (dam == 0)
        return FALSE;

{
char buf[200];             
        gain_exp (ch, xp);
        gain_jp (ch, jp);  
sprintf(buf,"$n gains {g%2d{x/{r%2d{x.\n\r",xp,jp); 
printf_to_char(ch,"You gain {g%2d{x/{r%2d{x.\n\r",xp,jp); 
act (buf, ch, NULL, NULL, TO_ROOM); 
}

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;
//    if (!IS_NPC (victim)
//        && victim->level >= LEVEL_IMMORTAL && victim->hit < 1)
//        victim->hit = 1;
    update_pos (victim);

    switch (victim->position)
    {
        case POS_MORTAL:
        case POS_INCAP:
        case POS_STUNNED:
        case POS_DEAD:
            act ("{R$n is DEAD!!{x", victim, 0, 0, TO_ROOM);
            send_to_char ("{RYou have been KILLED!!{x\n\r\n\r", victim);
            victim->hit = victim->max_hit;
            victim->mana = victim->max_mana;
            char_from_room(victim);
            victim->infight = FALSE;
    if(TURN(victim))
    advance_turn(victim);
            char_to_room(victim,get_room_index(5));
            break;

        default:
            break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if (!IS_AWAKE (victim))
        stop_fighting (victim, FALSE);

    /*
     * Payoff for killing things.
     */
    if (victim->position == POS_DEAD)
    {
     //   group_gain (ch, victim);

        sprintf (log_buf, "%s got toasted by %s at %s [room %d]",
                 (IS_NPC (victim) ? victim->short_descr : victim->name),
                 (IS_NPC (ch) ? ch->short_descr : ch->name),
                 ch->in_room->name, ch->in_room->vnum);

        if (IS_NPC (victim))
            wiznet (log_buf, NULL, NULL, WIZ_MOBDEATHS, 0, 0);
        else
            wiznet (log_buf, NULL, NULL, WIZ_DEATHS, 0, 0);

        raw_kill (victim);
        
        /* dump the flags */
        if (ch != victim && !IS_NPC (ch) && !is_same_clan (ch, victim))
        {
            if (IS_SET (victim->act, PLR_KILLER))
                REMOVE_BIT (victim->act, PLR_KILLER);
            else
                REMOVE_BIT (victim->act, PLR_THIEF);
        }

        /* RT new auto commands */

        if (!IS_NPC (ch)
            && (corpse =
                get_obj_list (ch, "corpse", ch->in_room->contents)) != NULL
            && corpse->item_type == ITEM_CORPSE_NPC
            && can_see_obj (ch, corpse))
        {
            OBJ_DATA *coins;

            corpse = get_obj_list (ch, "corpse", ch->in_room->contents);

            if (IS_SET (ch->act, PLR_AUTOLOOT) && corpse && corpse->contains)
            {                    /* exists and not empty */
                do_function (ch, &do_get, "all corpse");
            }

            if (IS_SET (ch->act, PLR_AUTOGOLD) && corpse && corpse->contains &&    /* exists and not empty */
                !IS_SET (ch->act, PLR_AUTOLOOT))
            {
                if ((coins = get_obj_list (ch, "gcash", corpse->contains))
                    != NULL)
                {
                    do_function (ch, &do_get, "all.gcash corpse");
                }
            }

            if (IS_SET (ch->act, PLR_AUTOSAC))
            {
                if (IS_SET (ch->act, PLR_AUTOLOOT) && corpse
                    && corpse->contains)
                {
                    return TRUE;    /* leave if corpse has treasure */
                }
                else
                {
                    do_function (ch, &do_sacrifice, "corpse");
                }
            }
        }

        return TRUE;
    }

    if (victim == ch)
        return TRUE;

    /*
     * Take care of link dead people.
     */
    if (!IS_NPC (victim) && victim->desc == NULL)
    {
        if (number_range (0, victim->wait) == 0)
        {
            do_function (victim, &do_recall, "");
            return TRUE;
        }
    }

    if (!IS_NPC (victim)
        && victim->hit > 0
        && victim->hit <= victim->wimpy && victim->wait < PULSE_VIOLENCE / 2)
    {
        do_function (victim, &do_flee, "");
    }

    tail_chain ();
    return TRUE;
}

bool is_safe (CHAR_DATA * ch, CHAR_DATA * victim)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (IS_IMMORTAL (ch) && ch->level > LEVEL_IMMORTAL)
        return FALSE;

    /* killing mobiles */
    if (IS_NPC (victim))
    {

        /* safe room? */
        if (IS_SET (victim->in_room->room_flags, ROOM_SAFE))
        {
            send_to_char ("Not in this room.\n\r", ch);
            return TRUE;
        }

        if (victim->pIndexData->pShop != NULL)
        {
            send_to_char ("The shopkeeper wouldn't like that.\n\r", ch);
            return TRUE;
        }

        /* no killing healers, trainers, etc */
        if (IS_SET (victim->act, ACT_TRAIN)
            || IS_SET (victim->act, ACT_PRACTICE)
            || IS_SET (victim->act, ACT_IS_HEALER)
            || IS_SET (victim->act, ACT_IS_CHANGER))
        {
            send_to_char ("I don't think Mota would approve.\n\r", ch);
            return TRUE;
        }

    }
    /* killing players */
    else
    {
        /* NPC doing the killing */
        if (IS_NPC (ch))
        {
            /* safe room check */
            if (IS_SET (victim->in_room->room_flags, ROOM_SAFE))
            {
                send_to_char ("Not in this room.\n\r", ch);
                return TRUE;
            }

        }
        /* player doing the killing */
        else
        {
/*            if (!is_clan (ch))
            {
                send_to_char ("Join a clan if you want to kill players.\n\r",
                              ch);
                return TRUE;
            }

            if (IS_SET (victim->act, PLR_KILLER)
                || IS_SET (victim->act, PLR_THIEF))
                return FALSE;

            if (!is_clan (victim))
            {
                send_to_char ("They aren't in a clan, leave them alone.\n\r",
                              ch);
                return TRUE;
            }

            if (ch->level > victim->level + 8)
            {
                send_to_char ("Pick on someone your own size.\n\r", ch);
                return TRUE;
            }
*/
        }
    }
    return FALSE;
}

bool is_safe_spell (CHAR_DATA * ch, CHAR_DATA * victim, bool area)
{

/* no-one is safe*/    return FALSE;

}



/*
 * Check for parry.
 */
bool check_parry (CHAR_DATA * ch, CHAR_DATA * victim)
{
    int chance;

    if (!IS_AWAKE (victim))
        return FALSE;

    chance = get_skill (victim, gsn_parry) / 2;

    if (get_eq_char (victim, WEAR_RIGHT) == NULL)
    {
        if (IS_NPC (victim))
            chance /= 2;
        else
            return FALSE;
    }

    if (!can_see (ch, victim))
        chance /= 2;

    if (number_percent () >= chance + victim->level - ch->level)
        return FALSE;

    act ("You parry $n's attack.", ch, NULL, victim, TO_VICT);
    act ("$N parries your attack.", ch, NULL, victim, TO_CHAR);
    return TRUE;
}

/*
 * Check for shield block.
 */

/*
 * Check for dodge.
 */
bool check_dodge (CHAR_DATA * ch, CHAR_DATA * victim)
{
    int chance;

    if (!IS_AWAKE (victim))
        return FALSE;

    chance = get_skill (victim, gsn_dodge) / 2;

    if (!can_see (victim, ch))
        chance /= 2;

    if (number_percent () >= chance + victim->level - ch->level)
        return FALSE;

    act ("You dodge $n's attack.", ch, NULL, victim, TO_VICT);
    act ("$N dodges your attack.", ch, NULL, victim, TO_CHAR);
    return TRUE;
}



/*
 * Set position of a victim.
 */
void update_pos (CHAR_DATA * victim)
{
    if (victim->hit > 0)
    {
        if (victim->position <= POS_STUNNED)
            victim->position = POS_STANDING;
        return;
    }

    if (IS_NPC (victim) && victim->hit < 1)
    {
        victim->position = POS_DEAD;
        return;
    }

    if (victim->hit <= -11)
    {
        victim->position = POS_DEAD;
        return;
    }

    if (victim->hit <= -6)
        victim->position = POS_MORTAL;
    else if (victim->hit <= -3)
        victim->position = POS_INCAP;
    else
        victim->position = POS_STUNNED;

    return;
}



/*
 * Stop fights.
 */
void stop_fighting (CHAR_DATA * ch, bool fBoth)
{
    ch->infight = FALSE;
    if(TURN(ch))
    advance_turn(ch);  
    return;
}



/*
 * Make a corpse out of a character.
 */
void make_corpse (CHAR_DATA * ch)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;

    if (IS_NPC (ch))
    {
        name = ch->short_descr;
        corpse = create_object (get_obj_index (OBJ_VNUM_CORPSE_NPC), 0);
        corpse->timer = number_range (3, 6);
        if (ch->gold > 0)
        {
            obj_to_obj (create_money (ch->gold, ch->silver), corpse);
            ch->gold = 0;
            ch->silver = 0;
        }
        corpse->cost = 0;
    }
    else
    {
        name = ch->name;
        corpse = create_object (get_obj_index (OBJ_VNUM_CORPSE_PC), 0);
        corpse->timer = number_range (25, 40);
        REMOVE_BIT (ch->act, PLR_CANLOOT);
        if (!is_clan (ch))
            corpse->owner = str_dup (ch->name);
        else
        {
            corpse->owner = NULL;
            if (ch->gold > 1 || ch->silver > 1)
            {
                obj_to_obj (create_money (ch->gold / 2, ch->silver / 2),
                            corpse);
                ch->gold -= ch->gold / 2;
                ch->silver -= ch->silver / 2;
            }
        }

        corpse->cost = 0;
    }

    corpse->level = ch->level;

    sprintf (buf, corpse->short_descr, name);
    free_string (corpse->short_descr);
    corpse->short_descr = str_dup (buf);

    sprintf (buf, corpse->description, name);
    free_string (corpse->description);
    corpse->description = str_dup (buf);

    for (obj = ch->carrying; obj != NULL; obj = obj_next)
    {
        bool floating = FALSE;

        obj_next = obj->next_content;
        obj_from_char (obj);
        if (obj->item_type == ITEM_POTION)
            obj->timer = number_range (500, 1000);
        if (obj->item_type == ITEM_SCROLL)
            obj->timer = number_range (1000, 2500);
        if (IS_SET (obj->extra_flags, ITEM_ROT_DEATH) && !floating)
        {
            obj->timer = number_range (5, 10);
            REMOVE_BIT (obj->extra_flags, ITEM_ROT_DEATH);
        }
        REMOVE_BIT (obj->extra_flags, ITEM_VIS_DEATH);

        if (IS_SET (obj->extra_flags, ITEM_INVENTORY))
            extract_obj (obj);
        else if (floating)
        {
            if (IS_OBJ_STAT (obj, ITEM_ROT_DEATH))
            {                    /* get rid of it! */
                if (obj->contains != NULL)
                {
                    OBJ_DATA *in, *in_next;

                    act ("$p evaporates,scattering its contents.",
                         ch, obj, NULL, TO_ROOM);
                    for (in = obj->contains; in != NULL; in = in_next)
                    {
                        in_next = in->next_content;
                        obj_from_obj (in);
                        obj_to_room (in, ch->in_room);
                    }
                }
                else
                    act ("$p evaporates.", ch, obj, NULL, TO_ROOM);
                extract_obj (obj);
            }
            else
            {
                act ("$p falls to the floor.", ch, obj, NULL, TO_ROOM);
                obj_to_room (obj, ch->in_room);
            }
        }
        else
            obj_to_obj (obj, corpse);
    }

    obj_to_room (corpse, ch->in_room);
    return;
}



/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry (CHAR_DATA * ch)
{
    char *msg;
    int vnum;

    vnum = 0;
    msg = "You hear $n's death cry.";

    switch (number_bits (4))
    {
        case 0:
            msg = "$n hits the ground ... DEAD.";
            break;
        case 1:
            if (ch->material == 0)
            {
                msg = "$n splatters blood on your armor.";
                break;
            }
        case 2:
            if (IS_SET (ch->parts, PART_GUTS))
            {
                msg = "$n spills $s guts all over the floor.";
                vnum = OBJ_VNUM_GUTS;
            }
            break;
        case 3:
            if (IS_SET (ch->parts, PART_HEAD))
            {
                msg = "$n's severed head plops on the ground.";
                vnum = OBJ_VNUM_SEVERED_HEAD;
            }
            break;
        case 4:
            if (IS_SET (ch->parts, PART_HEART))
            {
                msg = "$n's heart is torn from $s chest.";
                vnum = OBJ_VNUM_TORN_HEART;
            }
            break;
        case 5:
            if (IS_SET (ch->parts, PART_ARMS))
            {
                msg = "$n's arm is sliced from $s dead body.";
                vnum = OBJ_VNUM_SLICED_ARM;
            }
            break;
        case 6:
            if (IS_SET (ch->parts, PART_LEGS))
            {
                msg = "$n's leg is sliced from $s dead body.";
                vnum = OBJ_VNUM_SLICED_LEG;
            }
            break;
        case 7:
            if (IS_SET (ch->parts, PART_BRAINS))
            {
                msg =
                    "$n's head is shattered, and $s brains splash all over you.";
                vnum = OBJ_VNUM_BRAINS;
            }
    }

    act (msg, ch, NULL, NULL, TO_ROOM);

    if (vnum != 0)
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name = IS_NPC (ch) ? ch->short_descr : ch->name;
        obj = create_object (get_obj_index (vnum), 0);
        obj->timer = number_range (4, 7);

        sprintf (buf, obj->short_descr, name);
        free_string (obj->short_descr);
        obj->short_descr = str_dup (buf);

        sprintf (buf, obj->description, name);
        free_string (obj->description);
        obj->description = str_dup (buf);

        if (obj->item_type == ITEM_FOOD)
        {
            if (IS_SET (ch->form, FORM_POISON))
                obj->value[3] = 1;
            else if (!IS_SET (ch->form, FORM_EDIBLE))
                obj->item_type = ITEM_TRASH;
        }

        obj_to_room (obj, ch->in_room);
    }

    if (IS_NPC (ch))
        msg = "You hear something's death cry.";
    else
        msg = "You hear someone's death cry.";

    return;
}



void raw_kill (CHAR_DATA * victim)
{
    int i;

    victim->infight = FALSE;

    if(TURN(victim))
    advance_turn(victim);

    death_cry (victim);
    make_corpse (victim);

    if (IS_NPC (victim))
    {
        extract_char (victim, TRUE);
        return;
    }

    victim->in_room->map.closed = TRUE;

    extract_char (victim, FALSE);
    while (victim->affected)
        affect_remove (victim, victim->affected);

    STR_COPY_STR( victim->affected_by, race_table[victim->race].aff, AFF_FLAGS );
    for (i = 0; i < 4; i++)
        victim->armor[i] = 100;
    victim->position = POS_RESTING;
    victim->mposy = 0;
    victim->mposx = 0;
    victim->infight = FALSE;
    victim->AT = 0;
    victim->casting = FALSE;
    victim->SnToCast = 0;
    victim->CastTargX = 0;
    victim->CastTargY = 0;
    char_to_room (victim, get_room_index (101));
    do_look(victim,"auto");

    save_char_obj( victim ); 
    return;
}



void group_gain (CHAR_DATA * ch, CHAR_DATA * victim)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    int xp;
    int members;
    int group_levels;

    /*
     * Monsters don't get kill xp's or alignment changes.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if (victim == ch)
        return;

    members = 0;
    group_levels = 0;
    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        if (is_same_group (gch, ch))
        {
            members++;
            group_levels += IS_NPC (gch) ? gch->level / 2 : gch->level;
        }
    }

    if (members == 0)
    {
        bug ("Group_gain: members.", members);
        members = 1;
        group_levels = ch->level;
    }

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;

        if (!is_same_group (gch, ch) || IS_NPC (gch))
            continue;

        xp = xp_compute (gch, victim, group_levels);
        sprintf (buf, "You receive %d experience points.\n\r", xp);
        send_to_char (buf, gch);
        gain_exp (gch, xp);

        for (obj = ch->carrying; obj != NULL; obj = obj_next)
        {
            obj_next = obj->next_content;
            if (obj->wear_loc == WEAR_NONE)
                continue;

            if ((IS_OBJ_STAT (obj, ITEM_ANTI_EVIL) && IS_EVIL (ch))
                || (IS_OBJ_STAT (obj, ITEM_ANTI_GOOD) && IS_GOOD (ch))
                || (IS_OBJ_STAT (obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL (ch)))
            {
                act ("You are zapped by $p.", ch, obj, NULL, TO_CHAR);
                act ("$n is zapped by $p.", ch, obj, NULL, TO_ROOM);
                obj_from_char (obj);
                obj_to_room (obj, ch->in_room);
            }
        }
    }

    return;
}

int jp_compute (CHAR_DATA * gch, CHAR_DATA * victim, int total_levels)
{
return (8 + (gch->job_level[gch->jobid] * 2) + gch->level/4);
}

int xp_compute (CHAR_DATA * gch, CHAR_DATA * victim, int total_levels)
{
int b=0; 
if(victim->hit < 0) 
b = 10; 
return UMAX((10 + (victim->level - gch->level) + b), 1);
}

void dam_message (CHAR_DATA * ch, CHAR_DATA * victim, int dam, int dt,
                  bool immune)
{
    char buf1[256], buf2[256], buf3[256];
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;
    int dam_percent = ((100 * dam) / victim->max_hit);

    if (ch == NULL || victim == NULL)
        return;


    if (dam == 0)
    {
        vs = "miss";
        vp = "misses";
    }
    else if (dam_percent <= 5)
    {
        vs = "scratch";
        vp = "scratches";
    }
    else if (dam_percent <= 10)
    {
        vs = "graze";
        vp = "grazes";
    }
    else if (dam_percent <= 15)
    {
        vs = "hit";
        vp = "hits";
    }
    else if (dam_percent <= 20)
    {
        vs = "injure";
        vp = "injures";
    }
    else if (dam_percent <= 25)
    {
        vs = "wound";
        vp = "wounds";
    }
    else if (dam_percent <= 30)
    {
        vs = "maul";
        vp = "mauls";
    }
    else if (dam_percent <= 35)
    {
        vs = "decimate";
        vp = "decimates";
    }
    else if (dam_percent <= 40)
    {
        vs = "devastate";
        vp = "devastates";
    }
    else if (dam_percent <= 45)
    {
        vs = "maim";
        vp = "maims";
    }
    else if (dam_percent <= 50)
    {
        vs = "MUTILATE";
        vp = "MUTILATES";
    }
    else if (dam_percent <= 55)
    {
        vs = "DISEMBOWEL";
        vp = "DISEMBOWELS";
    }
    else if (dam_percent <= 60)
    {
        vs = "DISMEMBER";
        vp = "DISMEMBERS";
    }
    else if (dam_percent <= 65)
    {
        vs = "MASSACRE";
        vp = "MASSACRES";
    }
    else if (dam_percent <= 70)
    {
        vs = "MANGLE";
        vp = "MANGLES";
    }
    else if (dam_percent <= 75)
    {
        vs = "*** DEMOLISH ***";
        vp = "*** DEMOLISHES ***";
    }
    else if (dam_percent <= 80)
    {
        vs = "*** DEVASTATE ***";
        vp = "*** DEVASTATES ***";
    }
    else if (dam_percent <= 85)
    {
        vs = "=== OBLITERATE ===";
        vp = "=== OBLITERATES ===";
    }
    else if (dam_percent <= 90)
    {
        vs = ">>> ANNIHILATE <<<";
        vp = ">>> ANNIHILATES <<<";
    }
    else if (dam_percent <= 95)
    {
        vs = "<<< ERADICATE >>>";
        vp = "<<< ERADICATES >>>";
    }
    else
    {
        vs = "do UNSPEAKABLE things to";
        vp = "does UNSPEAKABLE things to";
    }

    punct = (dam_percent <= 45) ? '.' : '!';

    if (dt == TYPE_HIT)
    {
        if (ch == victim)
        {
            sprintf (buf1, "{3$n %s $melf%c{x", vp, punct);
            sprintf (buf2, "{2You %s yourself%c{x", vs, punct);
        }
        else
        {
            sprintf (buf1, "{3$n %s $N%c{x", vp, punct);
            sprintf (buf2, "{2You %s $N%c{x", vs, punct);
            sprintf (buf3, "{4$n %s you%c{x", vp, punct);
        }
    }
    else
    {
        if (dt >= 0 && dt < MAX_SKILL)
{
	    if(strlen(skill_table[dt].noun_damage) > 2)
            attack = skill_table[dt].noun_damage;
 	    else
            attack = skill_table[dt].name;
}
        else if (dt >= TYPE_HIT && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
            attack = attack_table[dt - TYPE_HIT].noun;
        else
        {
            bug ("Dam_message: bad dt %d.", dt);
            dt = TYPE_HIT;
            attack = attack_table[0].name;
        }

        if (immune)
        {
            if (ch == victim)
            {
                sprintf (buf1, "{3$n is unaffected by $s own %s.{x", attack);
                sprintf (buf2, "{2Luckily, you are immune to that.{x");
            }
            else
            {
                sprintf (buf1, "{3$N is unaffected by $n's %s!{x", attack);
                sprintf (buf2, "{2$N is unaffected by your %s!{x", attack);
                sprintf (buf3, "{4$n's %s is powerless against you.{x",
                         attack);
            }
        }
        else
        {
            if (ch == victim)
            {
                sprintf (buf1, "{3$n's %s %s $m%c (%d){x", attack, vp, punct,dam);
                sprintf (buf2, "{2Your %s %s you%c (%d){x", attack, vp, punct,dam);
            }
            else
            {
                sprintf (buf1, "{3$n's %s %s $N%c (%d){x", attack, vp, punct,dam);
                sprintf (buf2, "{2Your %s %s $N%c (%d){x", attack, vp, punct,dam);
                sprintf (buf3, "{4$n's %s %s you%c (%d){x", attack, vp, punct,dam);
            }
        }
    }

    if (ch == victim)
    {
        act (buf1, ch, NULL, NULL, TO_ROOM);
        act (buf2, ch, NULL, NULL, TO_CHAR);
    }
    else
    {
        act (buf1, ch, NULL, victim, TO_NOTVICT);
        act (buf2, ch, NULL, victim, TO_CHAR);
        act (buf3, ch, NULL, victim, TO_VICT);
    }
    return;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm (CHAR_DATA * ch, CHAR_DATA * victim)
{
    OBJ_DATA *obj;

    if ((obj = get_eq_char (victim, WEAR_RIGHT)) == NULL)
        return;

    if (IS_OBJ_STAT (obj, ITEM_NOREMOVE))
    {
        act ("{5$S weapon won't budge!{x", ch, NULL, victim, TO_CHAR);
        act ("{5$n tries to disarm you, but your weapon won't budge!{x",
             ch, NULL, victim, TO_VICT);
        act ("{5$n tries to disarm $N, but fails.{x", ch, NULL, victim,
             TO_NOTVICT);
        return;
    }

    act ("{5$n DISARMS you and sends your weapon flying!{x",
         ch, NULL, victim, TO_VICT);
    act ("{5You disarm $N!{x", ch, NULL, victim, TO_CHAR);
    act ("{5$n disarms $N!{x", ch, NULL, victim, TO_NOTVICT);

    obj_from_char (obj);
    if (IS_OBJ_STAT (obj, ITEM_NODROP) || IS_OBJ_STAT (obj, ITEM_INVENTORY))
        obj_to_char (obj, victim);
    else
    {
        obj_to_room (obj, victim->in_room);
        if (IS_NPC (victim) && victim->wait == 0 && can_see_obj (victim, obj))
            get_obj (victim, obj, NULL);
    }

    return;
}

void do_kill (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument (argument, arg);


if(ch->infight)
{
        send_to_char ("You are already fighting.\n\r", ch);
        return;
}


    if (arg[0] == '\0')
    {
        send_to_char ("Kill whom?\n\r", ch);
        return;
    }

    
    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

 if(victim->infight)
{
        send_to_char ("They are already fighting. Use the JOIN command.\n\r", ch);
        return;
}

  if(IS_NPC(victim))
  if(victim->pIndexData->pShop)
{
        send_to_char ("Not on shopkeepers.\n\r", ch);
        return;
}



    ch->in_room->map.closed = FALSE;
    printf_to_char(ch,"You start a fight.\n");
    printf_to_char(victim,"You've been pulled into a fight.\n");

    
    do_join(ch,"blue");
    do_join(victim,"red");

if(IS_NPC(victim))
do_autodisp(victim," ");
if(IS_NPC(ch))
do_autodisp(ch," ");

    if(victim->AT > ch->AT)
    {
    printf_to_char(ch,"%s goes first.",victim->name); 
    printf_to_char(victim,"%s goes first.",victim->name);
    victim->in_room->turn = victim;
    victim->MoveLeft = victim->base_move;
    victim->AttackLeft = 1;
    }
    else
    {
    printf_to_char(ch,"%s goes first.",ch->name); 
    printf_to_char(victim,"%s goes first.",ch->name);  
    ch->in_room->turn = ch;
    ch->MoveLeft = ch->base_move;
    ch->AttackLeft = 1;
    }

    return;
}



void do_flee (CHAR_DATA * ch, char *argument)
{
    if (ch->infight == FALSE)
    {
        send_to_char ("You aren't fighting anyone.\n\r", ch);
        return;
    }

    if(ch->mposx == 0 || ch->mposy == 0 || ch->mposx == 25 || ch->mposy == 15)
    {
    }
    else
    {
        send_to_char ("You need to be on the edge of the map to flee.\n\r", ch);
        return;
    }
     
    if(!TURN(ch))
    {
        send_to_char ("It's not your turn.\n\r", ch);
        return;
    }

    if(number_range(1,10) != 10)
    {
  
        act ("$n has fled!", ch, NULL, NULL, TO_ROOM);

        if (!IS_NPC (ch))
        {
            send_to_char ("You flee from combat!\n\r", ch);
        }

        do_done(ch," ");
        stop_fighting (ch, TRUE);
        ch->infight = FALSE;
        ch->mposx = 0;
        ch->mposy = 0;
	return;
    }

    send_to_char ("PANIC! You couldn't escape!\n\r", ch);
    ch->MoveLeft = 0;
    return;
}



void do_sla (CHAR_DATA * ch, char *argument)
{
    send_to_char ("If you want to SLAY, spell it out.\n\r", ch);
    return;
}



void do_slay (CHAR_DATA * ch, char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument (argument, arg);
    if (arg[0] == '\0')
    {
        send_to_char ("Slay whom?\n\r", ch);
        return;
    }

    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        send_to_char ("They aren't here.\n\r", ch);
        return;
    }

    if (ch == victim)
    {
        send_to_char ("Suicide is a mortal sin.\n\r", ch);
        return;
    }

    if (!IS_NPC (victim) && victim->level >= get_trust (ch))
    {
        send_to_char ("You failed.\n\r", ch);
        return;
    }

    act ("{1You slay $M in cold blood!{x", ch, NULL, victim, TO_CHAR);
    act ("{1$n slays you in cold blood!{x", ch, NULL, victim, TO_VICT);
    act ("{1$n slays $N in cold blood!{x", ch, NULL, victim, TO_NOTVICT);
    raw_kill (victim);
    return;
}
