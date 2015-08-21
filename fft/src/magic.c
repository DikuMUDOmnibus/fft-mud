 


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
#include "recycle.h"

/*
 * Local functions.
 */
void say_spell args ((CHAR_DATA * ch, int sn));

/* imported functions */
bool remove_obj args ((CHAR_DATA * ch, int iWear, bool fReplace));
void wear_obj args ((CHAR_DATA * ch, OBJ_DATA * obj, bool fReplace));
int get_f_spell_damage(CHAR_DATA *ch, CHAR_DATA *victim, int sn);


/*
 * Lookup a skill by name.
 */
int skill_lookup (const char *name)
{
    int sn;

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL)
            break;
        if (LOWER (name[0]) == LOWER (skill_table[sn].name[0])
            && !str_prefix (name, skill_table[sn].name))
            return sn;
    }

    return -1;
}

int find_spell (CHAR_DATA * ch, const char *name)
{
    /* finds a spell the character can cast if possible */
    int sn, found = -1;

    if (IS_NPC (ch))
        return skill_lookup (name);

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL)
            break;
        if (LOWER (name[0]) == LOWER (skill_table[sn].name[0])
            && !str_prefix (name, skill_table[sn].name))
        {
            if (found == -1)
                found = sn;
            if (ch->pcdata->learned[sn] > 0)
                return sn;
        }
    }
    return found;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup (int slot)
{
    extern bool fBootDb;
    int sn;

    if (slot <= 0)
        return -1;

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (slot == skill_table[sn].slot)
            return sn;
    }

    if (fBootDb)
    {
        bug ("Slot_lookup: bad slot %d.", slot);
        abort ();
    }

    return -1;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell (CHAR_DATA * ch, int sn)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    char *pName;
    int iSyl;
    int length;

    struct syl_type {
        char *old;
        char *new;
    };

    static const struct syl_type syl_table[] = {
        {" ", " "},
        {"ar", "abra"},
        {"au", "kada"},
        {"bless", "fido"},
        {"blind", "nose"},
        {"bur", "mosa"},
        {"cu", "judi"},
        {"de", "oculo"},
        {"en", "unso"},
        {"light", "dies"},
        {"lo", "hi"},
        {"mor", "zak"},
        {"move", "sido"},
        {"ness", "lacri"},
        {"ning", "illa"},
        {"per", "duda"},
        {"ra", "gru"},
        {"fresh", "ima"},
        {"re", "candus"},
        {"son", "sabru"},
        {"tect", "infra"},
        {"tri", "cula"},
        {"ven", "nofo"},
        {"a", "a"}, {"b", "b"}, {"c", "q"}, {"d", "e"},
        {"e", "z"}, {"f", "y"}, {"g", "o"}, {"h", "p"},
        {"i", "u"}, {"j", "y"}, {"k", "t"}, {"l", "r"},
        {"m", "w"}, {"n", "i"}, {"o", "a"}, {"p", "s"},
        {"q", "d"}, {"r", "f"}, {"s", "g"}, {"t", "h"},
        {"u", "j"}, {"v", "z"}, {"w", "x"}, {"x", "n"},
        {"y", "l"}, {"z", "k"},
        {"", ""}
    };

    buf[0] = '\0';

    if (!IS_NPC (ch) && ch->pcdata->condition[COND_DRUNK] > 40)
    {
    if(strlen(skill_table[sn].d_cast_text) == 0)
    {
    sprintf(buf,"%s %s!",skill_table[sn].d_cast_text,skill_table[sn].name);
    return;
    }    
    }
    else
    {
    if(strlen(skill_table[sn].cast_text) == 0)
    {
    sprintf(buf,"%s %s!",skill_table[sn].cast_text,skill_table[sn].name);
    return;
    }    
    }

    if(strlen(buf) < 4)
    for (pName = skill_table[sn].name; *pName != '\0'; pName += length)
    {
        for (iSyl = 0; (length = strlen (syl_table[iSyl].old)) != 0; iSyl++)
        {
            if (!str_prefix (syl_table[iSyl].old, pName))
            {
                strcat (buf, syl_table[iSyl].new);
                break;
            }
        }

        if (length == 0)
            length = 1;
    }

    sprintf (buf2, "$n : '%s'\n\r {M [{R%10s{R]{x", buf,skill_table[sn].name);
    sprintf (buf, "$n : '%s'\n\r", skill_table[sn].name);

    for (rch = ch->in_room->people; rch; rch = rch->next_in_room)
    {
        if (rch != ch)
            act ((!IS_NPC (rch)) ? buf : buf2,
                 ch, NULL, rch, TO_VICT);
    }

    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell (int level, CHAR_DATA * victim, int dam_type)
{
    int save;

    save = 50 + (victim->level - level) * 5 - victim->saving_throw * 2;

    switch (check_immune (victim, dam_type))
    {
        case IS_IMMUNE:
            return TRUE;
        case IS_RESISTANT:
            save += 2;
            break;
        case IS_VULNERABLE:
            save -= 2;
            break;
    }

    if (!IS_NPC (victim) )
        save = 9 * save / 10;
    save = URANGE (5, save, 95);
    return number_percent () < save;
}

/* RT save for dispels */

bool saves_dispel (int dis_level, int spell_level, int duration)
{
    int save;

    if (duration == -1)
        spell_level += 5;
    /* very hard to dispel permanent effects */

    save = 50 + (spell_level - dis_level) * 5;
    save = URANGE (5, save, 95);
    return number_percent () < save;
}

/* co-routine for dispel magic and cancellation */

bool check_dispel (int dis_level, CHAR_DATA * victim, int sn)
{
    AFFECT_DATA *af;

    if (is_affected (victim, sn))
    {
        for (af = victim->affected; af != NULL; af = af->next)
        {
            if (af->type == sn)
            {
                if (!saves_dispel (dis_level, af->level, af->duration))
                {
                    affect_strip (victim, sn);
                    if (skill_table[sn].msg_off)
                    {
                        send_to_char (skill_table[sn].msg_off, victim);
                        send_to_char ("\n\r", victim);
                    }
                    return TRUE;
                }
                else
                    af->level--;
            }
        }
    }
    return FALSE;
}

/* for finding mana costs -- temporary version */
int mana_cost (CHAR_DATA * ch, int min_mana, int level)
{
    if (ch->level + 2 == level)
        return 1000;
    return UMAX (min_mana, (100 / (2 + ch->level - level)));
}



/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast (CHAR_DATA * ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;
    int tX=0;
    int tY=0;

    target_name = one_argument (argument, arg1);
    one_argument (target_name, arg2);

    if(!ch->infight)
    DIE(ch,"You're not in a fight.\n\r");

    if(ch->AttackLeft < 1)
    DIE(ch,"You don't have any attacks left.\n\r");
    
        if (arg1[0] == '\0')
    {
        send_to_char ("Cast <spell> <coordinates>\n\r", ch);
        return;
    }

    if ((sn = find_spell (ch, arg1)) < 1
        || skill_table[sn].spell_fun == spell_null || (!IS_NPC (ch)
                                                       &&  ch->
                                                           pcdata->learned[sn]
                                                           == 0))
    {
        send_to_char ("You don't know any spells of that name.\n\r", ch);
        return;
    }
   
    if(skill_table[sn].job_num != ch->jobid && skill_table[sn].job_num != ch->job2id)
    DIE(ch,"You aren't using the skill group this spell comes from.");

         mana = skill_table[sn].min_mana;

    /*
     * Locate targets.
     */
    victim = NULL;
    obj = NULL;
    vo = NULL;
    target = TARGET_NONE;

    switch (skill_table[sn].target)
    {
        default:
            bug ("Do_cast: bad target for sn %d.", sn);
            return;

        case TAR_IGNORE:
            break;
            
        case TAR_CHAR_DEFENSIVE:    
        case TAR_CHAR_OFFENSIVE:
            if (arg2[0] == '\0')
            {
                    send_to_char ("Need coordinates or target name.\n\r", ch);
            }

tX = find_char_pos(ch,target_name,0);
tY = find_char_pos(ch,target_name,1);

if(tX == -2 || tY == -2)
{
send_to_char ("Bad coordinates (or victim not found). Try again.", ch);
            return;
            }
         

            vo = (void *) victim;
            target = TARGET_CHAR;
            break;

        case TAR_CHAR_SELF:
            if (arg2[0] != '\0' && !is_name (target_name, ch->name))
            {
                send_to_char ("You cannot cast this spell on another.\n\r",
                              ch);
                return;
            }

            vo = (void *) ch;
            target = TARGET_CHAR;
            break;

        case TAR_OBJ_INV:
            if (arg2[0] == '\0')
            {
                send_to_char ("What should the spell be cast upon?\n\r", ch);
                return;
            }

            if ((obj = get_obj_carry (ch, target_name, ch)) == NULL)
            {
                send_to_char ("You are not carrying that.\n\r", ch);
                return;
            }

            vo = (void *) obj;
            target = TARGET_OBJ;
            break;

        case TAR_OBJ_CHAR_OFF:
            if (arg2[0] == '\0')
            {
                    send_to_char ("Cast the spell on whom or what?\n\r", ch);
                    return;

                target = TARGET_CHAR;
            }
            else if ((obj = get_obj_here (ch, target_name)) != NULL)
            {
                vo = (void *) obj;
                target = TARGET_OBJ;
            }
            else
            {
                send_to_char ("You don't see that here.\n\r", ch);
                return;
            }
            break;

        case TAR_OBJ_CHAR_DEF:
            if (arg2[0] == '\0')
            {
                vo = (void *) ch;
                target = TARGET_CHAR;
            }
            else if ((obj = get_obj_carry (ch, target_name, ch)) != NULL)
            {
                vo = (void *) obj;
                target = TARGET_OBJ;
            }
            else
            {
                send_to_char ("You don't see that here.\n\r", ch);
                return;
            }
            break;
    }

    if (!IS_NPC (ch) && ch->mana < mana)
    {
        send_to_char ("You don't have enough mana.\n\r", ch);
        return;
    }
    
    if(find_distance(tX,tY,ch->mposy,ch->mposx) > 3)
    {
        send_to_char ("Out of range.\n\r", ch);
        return;
    }
    
if(ch->MoveLeft != ch->base_move)
ch->MoveLeft = 0;

        ch->AttackLeft = 0;
	    ch->casting = TRUE;
	    ch->ATToCast = skill_table[sn].beats;
	    ch->mana -= mana;
	    ch->SnToCast = sn;
	    ch->CastTargX = tX;
	    ch->CastTargY = tY;
        printf_to_char(ch, "You attempt to cast at %s.\n\r", target_name);
        {
        char dbuf[MSL];
        sprintf(dbuf,"$n starts to cast a spell at %2s.",target_name);
        act (dbuf, ch, NULL, NULL, TO_ROOM);
        }

    return;
}


void obj_cast_spell (int sn, int level, CHAR_DATA * ch, CHAR_DATA * victim, OBJ_DATA * obj) 
{
    return;
}



/*
 * Spell functions.
 */

void spell_fspell (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
CHAR_DATA *victim = (CHAR_DATA *) vo;
int i = get_f_spell_damage(ch, victim, sn);
damage (ch, victim, i, sn, f_mag_table[sn].element, TRUE);
return;
}

void spell_armor (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected (victim, sn))
    {
        if (victim == ch)
            send_to_char ("You are already armored.\n\r", ch);
        else
            act ("$N is already armored.", ch, NULL, victim, TO_CHAR);
        return;
    }
    af.where = TO_AFFECTS;
    af.type = sn;
    af.level = level;
    af.duration = 24;
    af.modifier = -20;
    af.location = APPLY_AC;
    af.bitvector = 0;
    affect_to_char (victim, &af);
    send_to_char ("You feel someone protecting you.\n\r", victim);
    if (ch != victim)
        act ("$N is protected by your magic.", ch, NULL, victim, TO_CHAR);
    return;
}


void spell_bless (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
        obj = (OBJ_DATA *) vo;
        if (IS_OBJ_STAT (obj, ITEM_BLESS))
        {
            act ("$p is already blessed.", ch, obj, NULL, TO_CHAR);
            return;
        }

        if (IS_OBJ_STAT (obj, ITEM_EVIL))
        {
            AFFECT_DATA *paf;

            paf = affect_find (obj->affected, gsn_curse);
            if (!saves_dispel
                (level, paf != NULL ? paf->level : obj->level, 0))
            {
                if (paf != NULL)
                    affect_remove_obj (obj, paf);
                act ("$p glows a pale blue.", ch, obj, NULL, TO_ALL);
                REMOVE_BIT (obj->extra_flags, ITEM_EVIL);
                return;
            }
            else
            {
                act ("The evil of $p is too powerful for you to overcome.",
                     ch, obj, NULL, TO_CHAR);
                return;
            }
        }

        af.where = TO_OBJECT;
        af.type = sn;
        af.level = level;
        af.duration = 6 + level;
        af.location = APPLY_SAVES;
        af.modifier = -1;
        af.bitvector = ITEM_BLESS;
        affect_to_obj (obj, &af);

        act ("$p glows with a holy aura.", ch, obj, NULL, TO_ALL);

        if (obj->wear_loc != WEAR_NONE)
            ch->saving_throw -= 1;
        return;
    }

    /* character target */
    victim = (CHAR_DATA *) vo;


    if (victim->position == POS_FIGHTING || is_affected (victim, sn))
    {
        if (victim == ch)
            send_to_char ("You are already blessed.\n\r", ch);
        else
            act ("$N already has divine favor.", ch, NULL, victim, TO_CHAR);
        return;
    }

    af.where = TO_AFFECTS;
    af.type = sn;
    af.level = level;
    af.duration = 6 + level;
    af.location = APPLY_HITROLL;
    af.modifier = level / 8;
    af.bitvector = 0;
    affect_to_char (victim, &af);

    af.location = APPLY_SAVING_SPELL;
    af.modifier = 0 - level / 8;
    affect_to_char (victim, &af);
    send_to_char ("You feel righteous.\n\r", victim);
    if (ch != victim)
        act ("You grant $N the favor of your god.", ch, NULL, victim,
             TO_CHAR);
    return;
}



void spell_null (int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
    send_to_char ("That's not a spell!\n\r", ch);
    return;
}

