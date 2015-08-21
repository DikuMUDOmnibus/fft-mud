#include "include.h"
CHAR_DATA * find_nearest_targ(CHAR_DATA *ch);

CHAR_DATA * find_nearest_targ(CHAR_DATA *ch)
{
int distance = 500;
CHAR_DATA *rch;
CHAR_DATA *dch = NULL; 


for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
{

if(!rch->infight)
continue;

if(ch->team == rch->team && ch->team != TEAM_NONE)
continue;

if(ch == rch || (ch->in_room != rch->in_room))
continue;

if(find_distance(ch->mposx,ch->mposy,rch->mposy,rch->mposx) < distance)
{
distance = find_distance(ch->mposx,ch->mposy,rch->mposy,rch->mposx);
dch = rch;
}

}

return dch;

}

void ai_move(CHAR_DATA *ch)
{

if(!IS_NPC(ch))
return;

switch(ch->pIndexData->ai_num)
{
case 0:
interpret(ch, "done");
break;

case 1:
ai_kill(ch);
break;

case 2:
break;

case 3:
break;

case 4:
ai_sit(ch);
break;

default:
break;
}

}

void ai_sit(CHAR_DATA *ch)
{
interpret(ch, "attack north");
interpret(ch, "attack east");
interpret(ch, "attack south");
interpret(ch, "attack west");
interpret(ch, "done");
}

void ai_kill(CHAR_DATA *ch)
{
CHAR_DATA * target;
int x;

target = find_nearest_targ(ch);

if(target == NULL)
{
logf("buhhh");
return;
}

for(x=0;x!=3;x++)
{

if(target->mposx > ch->mposx)
interpret(ch, "east");

if(target->mposx < ch->mposx)
interpret(ch, "west");

if(target->mposy > ch->mposy)
interpret(ch, "south");

if(target->mposy < ch->mposy)
interpret(ch, "north");

}

interpret(ch, "attack north");
interpret(ch, "attack east");
interpret(ch, "attack south");
interpret(ch, "attack west");
interpret(ch, "done");

}
