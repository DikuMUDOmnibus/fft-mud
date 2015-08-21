#include "include.h"

bool validate_attack(CHAR_DATA *ch,int sn);
CHAR_DATA * find_char_xy(CHAR_DATA *ch, int x, int y);

bool validate_attack(CHAR_DATA *ch,int sn)
{

if(!ch->infight)
{
printf_to_char(ch,"You aren't fighting.\n\r");
return FALSE;
}

if(!ch->pcdata->learned[sn])
{
printf_to_char(ch,"Huh?\n\r");
return FALSE;
}

if(!TURN(ch))
{
printf_to_char(ch,"It's not your turn.\n\r");
return FALSE;
}

if(ch->AttackLeft < 1)
{
printf_to_char(ch,"You've already attacked.\n\r");
return FALSE;
}
return TRUE;
}

void throw_stone (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int tY,tX,dam;
    one_argument (argument, arg);

   if(!validate_attack(ch,skill_lookup("throw stone"))) return; 

    if (arg[0] == '\0')
    {
        send_to_char ("I NEED COORDINATES!! \n\r", ch);
        return;
    }
    
tX = find_char_pos(ch,arg,0);
tY = find_char_pos(ch,arg,1);
if(tX == -2 || tY == -2) { send_to_char ("Bad XY (or victim not found). Try again.", ch); return; }

    if(find_distance(tX,tY,ch->mposy,ch->mposx) > 3)
    {
        send_to_char ("Out of range.\n\r", ch);
        return;
    }

ch->AttackLeft--;

if(ch->MoveLeft != ch->base_move)
ch->MoveLeft = 0;

       dam = ch->PhysAttack * number_range(1,2);

victim = find_char_xy(ch,tX,tY);

if(victim == NULL)
    return;

act ("$n throws a rock..", ch, NULL, NULL, TO_ROOM);
send_to_char("You throw a rock..\n\r",ch);

        if(number_range(1,300)>200)
        {
        act (".. and misses $n.", victim, NULL, NULL, TO_ROOM);
        send_to_char(".. and misses you.\n\r",victim);
        }
        else
        {
        act (".. and hits $n.", victim, NULL, NULL, TO_ROOM);
        send_to_char(".. and it hits you.\n\r",victim);
        damage (ch, victim, dam, gsn_throw_stone, ELE_NON, TRUE);
		}
    return;
}



void xmeteor (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int tY,tX;
    one_argument (argument, arg);

   if(!validate_attack(ch,skill_lookup("xmeteor"))) return; 

    if (arg[0] == '\0')
    {
        send_to_char ("I NEED COORDINATES!! \n\r", ch);
        return;
    }
    
tX = find_char_pos(ch,arg,0);
tY = find_char_pos(ch,arg,1);

if(tX == -2 || tY == -2) { send_to_char ("Bad XY (or victim not found). Try again.", ch); return; }

    if(find_distance(tX,tY,ch->mposy,ch->mposx) > 5)
    {
        send_to_char ("Out of range.\n\r", ch);
        return;
    }

ch->AttackLeft--;

if(ch->MoveLeft != ch->base_move)
ch->MoveLeft = 0;

act ("ONWARD CAME THE METEORS!!",ch,NULL,NULL,TO_ROOM);
send_to_char("ONWARD CAME THE METEORS!!\n\r",ch);

{
int c;
int x;
for(c=tX - 2;c<tX + 2;c++)
{
for(x=tY - 2;x<tY + 2;x++)
{
if(c<0 || c>26 || x < 0 || x > 15)
continue;

ch->in_room->map.index[c][x].height -= 2;
}
}
}
    return;
}


