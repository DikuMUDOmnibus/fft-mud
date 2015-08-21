#include "include.h"
bool has_learned_skill(CHAR_DATA *ch, char * name);

bool can_hit_t(CHAR_DATA *ch,int x,int y)
{
 OBJ_DATA *wield = NULL;
 wield = get_eq_char (ch, WEAR_RIGHT);

 if(wield == NULL)
 {
 if(find_distance(ch->mposy,ch->mposx,y,x) < 2)
 return TRUE;
 else
 return FALSE;
 }
 
if(find_distance(ch->mposy,ch->mposx,y,x) < (weapon_table[wield->pIndexData->value[0]].range)+1)
return TRUE;
else
return FALSE;
}



int get_w_dmg(CHAR_DATA *ch)
{
int theSpeed=ch->speed;
int PhysAttack = ch->PhysAttack;
int MagiAttack = ch->MagAttack;
int theBrave = ch->Brave;
int theFaith = ch->Faith;

    OBJ_DATA *wield = NULL;

    if(IS_NPC(ch))
    return ((((PhysAttack + theSpeed)/2) * ch->level/2+1));

    wield = get_eq_char (ch, WEAR_RIGHT);

    if(wield == NULL)
    return ((((PhysAttack + theSpeed)/2) * ch->level)/2);

{
int WeaponPower = wield->pIndexData->value[1];


    switch(wield->pIndexData->value[3])
    {
    case WEAPON_NINJA_SWORD:
    case WEAPON_DAGGER:
    return (((PhysAttack + theSpeed)/2) * WeaponPower);
    break;
    case WEAPON_SPEAR:
    case WEAPON_SWORD:
    return (PhysAttack * WeaponPower);
    break;
    case WEAPON_KATANA:
    case WEAPON_KNIGHT_SWORD:
    return (PhysAttack * (theBrave/100) * WeaponPower);
    break;
    case WEAPON_BAG:
    case WEAPON_AXE:
    return (WeaponPower * number_range(1,PhysAttack));
    break;
    case WEAPON_BOW:
    case WEAPON_ROD:
    return PhysAttack * WeaponPower;
    break;
    case WEAPON_STICK:
    case WEAPON_STAVE:
    return MagiAttack * WeaponPower;
    break;
    case WEAPON_GUN:
    return WeaponPower * WeaponPower;   
    break;
    case WEAPON_CLOTH:
    return ((PhysAttack + MagiAttack)/2) * WeaponPower;
    break;
    case WEAPON_DICT:
    case WEAPON_INST:
    return ((PhysAttack + MagiAttack) / 2) * WeaponPower;
    break;
    }
    }
    return theFaith;
}



bool verify_skill(CHAR_DATA *ch, int sn)
{

if(!ch->pcdata->learned[sn])
{
send_to_char("You don't know that skill.",ch);
return FALSE;
}

if(ch->jobid == skill_table[sn].job_num || ch->job2id == skill_table[sn].job_num)
return TRUE;

send_to_char("That skill is not in either of your job groups.",ch);
return FALSE;
}

bool is_avail_job(CHAR_DATA *ch, char *argument)
{
int i=0;
for(i=0;i<MAX_JOB;i++)
{
if (!str_prefix (job_table[i].name, argument))
{
if(ch->job_level[job_table[i].dep_1_job] >= job_table[i].dep_1_lev && 
   ch->job_level[job_table[i].dep_2_job] >= job_table[i].dep_2_lev &&
   ch->job_level[job_table[i].dep_3_job] >= job_table[i].dep_3_lev)
return TRUE;
}
}
return FALSE;
}

void do_gain(CHAR_DATA *ch, char *argument)
{
int sn = skill_lookup(argument);
if(ch->pcdata->learned[sn])
{
send_to_char("You already know that skill.\n\r",ch);
return;
}
if(skill_table[sn].job_num != ch->jobid)
{
printf_to_char(ch,"You need to be a %s to learn this.\n\r",job_table[skill_table[sn].job_num].name);
return;
}
if(ch->job_jp[skill_table[sn].job_num] < skill_table[sn].jp_cost)
{
printf_to_char(ch,"Not enough JP.\n\r");
return;
}
ch->job_jp[skill_table[sn].job_num] -= skill_table[sn].jp_cost;
ch->pcdata->learned[sn] = 1;
printf_to_char(ch,"Learned %s.\n\r",skill_table[sn].name);
}

void do_job(CHAR_DATA *ch, char *argument)
{
char arg[MSL];
int i=0;

one_argument (argument, arg);

if(strlen(argument) < 1)
{
printf_to_char(ch,"--Available Jobs--\n\r");
printf_to_char(ch,"------------------\n\r");

for(i=0;job_table[i].name != NULL;i++)
{
if(is_avail_job(ch,job_table[i].name))
printf_to_char(ch,"{y[{c%1s{r%1d{g %-10s %5d{y]{x\n\r",is_mastered(ch,i) ? "*" : " ",ch->job_level[i],job_table[i].name,ch->job_jp[i]);
}
return;
}

if(ch->infight)
{
send_to_char("Not in a fight.\n\r",ch);
return;
}

for(i=0;job_table[i].name != NULL;i++)
{
if (!str_prefix (job_table[i].name, argument) && is_avail_job(ch,job_table[i].name))
{
ch->job2id = ch->jobid;
ch->jobid = i;
send_to_char("OK.\n\r",ch);
return;
}
}

}

bool has_learned_skill(CHAR_DATA *ch, char * name)
{
int q;
for(q = 0; q != MAX_SKILL; q++) 
if(is_exact_name(name,skill_table[q].name)) 
if(ch->pcdata->learned[q])
return TRUE; 
return FALSE;
}

void equip_skill(CHAR_DATA *ch, int sn)
{

if(!ch->pcdata->learned[sn])
{
printf_to_char(ch,"You don't have that skill."); 
return;
}

switch(skill_table[sn].type)
{
case SKILL_REACT:
ch->reaction_num = sn;
break;
case SKILL_MISC:
ch->misc_num = sn;
break;
case SKILL_MOVE:
ch->move_num = sn;
break;
default:
logf("%d",skill_table[sn].type);
printf_to_char(ch,"This skill is messed up. Tell someone.\n\r");
return;
break;
}

printf_to_char(ch,"Done.\n\r");

}

