#include "include.h"

void calc_move(CHAR_DATA *ch);
const char gridname [27] = "0123456789abcdefghijklmnop";
void advance_turn(CHAR_DATA *ch);
extern const char echo_off_str[];
extern const char echo_on_str[];
int colorstrlen(char *txt);
void f_multi_hit( CHAR_DATA*ch, CHAR_DATA *victim, int dt );
void generate_random_mountains(ROOM_INDEX_DATA *room);
void handle_reaction(CHAR_DATA *ch, CHAR_DATA *victim);
void generate_random_hills(ROOM_INDEX_DATA *room);
void say_spell(CHAR_DATA *ch, int sn);
void check_winner(ROOM_INDEX_DATA *ro);
char * get_t_color(CHAR_DATA *ch);

int get_f_spell_damage(CHAR_DATA *ch, CHAR_DATA *victim, int sn)
{
double fVal= UMIN((ch->Faith/100) * (victim->Faith/100) * 2,1);
return (f_mag_table[sn].multiplier * ch->MagAttack * fVal);
}

void calc_move(CHAR_DATA *ch)
{
OBJ_DATA * tObj;
int total = job_table[ch->jobid].move_base;
int jtotal = job_table[ch->jobid].jump_base;
double cache;
double cache2;


cache = (ch->HPP/16384);
cache2 = (job_table[ch->jobid].classhpmult/100);
if (cache2 < 1) cache2 = 1;
if (cache < 1) cache = 1;
ch->max_hit = (cache * cache2);
cache = (ch->MPP/16384);
cache2 = (job_table[ch->jobid].classmpmult/100);
if (cache2 < 1) cache2 = 1;
if (cache < 1) cache = 1;
ch->max_mana = (cache * cache2);
cache = (ch->SpP/16384);
cache2 = (job_table[ch->jobid].classspmult/100);
if (cache2 < 1) cache2 = 1;
if (cache < 1) cache = 1;
ch->speed = (cache * cache2);
cache = (ch->PAP/16384);
cache2 = (job_table[ch->jobid].classpamult/100);
if (cache2 < 1) cache2 = 1;
if (cache < 1) cache = 1;
ch->PhysAttack = (cache * cache2);
cache = (ch->MAP/16384);
cache2 = (job_table[ch->jobid].classmamult/100);
if (cache2 < 1) cache2 = 1;
if (cache < 1) cache = 1;
ch->MagAttack = (cache * cache2);

if(IS_NPC(ch))
{
total = 3;
jtotal = 2;
ch->speed = UMAX(number_range(ch->level/5,30),5);
}

if(ch->move_num == skill_lookup("move+1"))
total += 1;
else if(ch->move_num == skill_lookup("move+2"))
total += 2;
else if(ch->move_num == skill_lookup("move+3"))
total += 3;

if(ch->move_num == skill_lookup("jump+1"))
jtotal += 1;
else if(ch->move_num == skill_lookup("jump+2"))
jtotal += 2;
else if(ch->move_num == skill_lookup("jump+3"))
jtotal += 3;

tObj = get_eq_char (ch, WEAR_RIGHT);
if(tObj != NULL)
{
if(tObj->pIndexData->tMod == TMOD_MOVE)
total += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_JUMP)
jtotal += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_SPEED)
ch->speed += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_HIT)
ch->max_hit += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_MANA)
ch->max_mana += tObj->pIndexData->tAmount;
}

tObj = get_eq_char (ch, WEAR_LEFT);
if(tObj != NULL)
{
if(tObj->pIndexData->tMod == TMOD_MOVE)
total += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_JUMP)
jtotal += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_SPEED)
ch->speed += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_HIT)
ch->max_hit += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_MANA)
ch->max_mana += tObj->pIndexData->tAmount;
}

tObj = get_eq_char (ch, WEAR_BODY);

if(tObj != NULL)
{
ch->max_hit+= tObj->value[0];
ch->max_mana+= tObj->value[1];
ch->PhysAttack += tObj->value[2];    
ch->MagAttack += tObj->value[3];    
ch->speed += tObj->value[4];   
}

tObj = get_eq_char (ch, WEAR_ACCESSORY);
if(tObj != NULL)
{
if(tObj->pIndexData->tMod == TMOD_MOVE)
total += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_JUMP)
jtotal += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_SPEED)
ch->speed += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_HIT)
ch->max_hit += tObj->pIndexData->tAmount;
if(tObj->pIndexData->tMod == TMOD_MANA)
ch->max_mana += tObj->pIndexData->tAmount;
}

ch->base_move = total;
ch->jump = jtotal;
}

void set_stats(CHAR_DATA *ch) //stub
{
calc_move(ch);
}

/* for some odd reason this function reminds me of metroids */

bool is_mastered(CHAR_DATA *ch, int job)
{
int i;
if(IS_NPC(ch))
return FALSE;
for(i=0;i<MAX_SKILL;i++)
{
if(skill_table[i].job_num == job && ch->pcdata->learned[i] == 0)
return FALSE;
}
return TRUE;
}

void do_autodisp(CHAR_DATA *ch, char *argument)
{
char buf[40];
if(IS_NPC(ch))
sprintf(buf,"%s{!%c{W{x",get_t_color(ch),ch->f_name[0]);
else
sprintf(buf,"%s{!%c{W{x",get_t_color(ch),ch->name[0]);
ch->fdisp = str_dup(buf);
}

void leave_fight(CHAR_DATA *ch) 
{ 
ch->infight = FALSE; 

  if(TURN(ch))
  advance_turn(ch);
  ch->mposx = 0; ch->jump = 0; ch->base_move = 0; ch->in_room->map.closed = TRUE;
}

void gen_random_battle(CHAR_DATA *ch)
{
int monsters[10];
int i=0;
CHAR_DATA *leader;
CHAR_DATA *crone;
CHAR_DATA *wch;
int mob_top=0;
char buf[50];
int total=0;
int charcnt = 0;
MOB_INDEX_DATA *pMobIndex;

for (wch = ch->in_room->people; wch != NULL; wch = wch->next_in_room)
charcnt++;

pMobIndex = get_mob_index (number_range(20,30));
leader = create_mobile(pMobIndex);
total = ch->level + number_range(-6,4);

if(total > 100)
total = 99;

for(i=0;i<total;i++)
advance_level(leader,FALSE);

char_to_room (leader, ch->in_room);
charcnt--;

while(charcnt!=0)
{
pMobIndex = get_mob_index (number_range(20,30));
crone = create_mobile(pMobIndex);
total = ch->level + number_range(-6,4);

if(total > 100)
total = 99;

for(i=0;i<total;i++)
advance_level(crone,FALSE);

char_to_room (crone, ch->in_room);
charcnt--;
}

send_to_char("\n\r{c******{RRANDOM BATTLE!!!!{c******{x\n\r",ch);
sprintf(buf,"%s",ch->name);

do_kill(leader,buf);

sprintf(buf,"%d",leader->team);

charcnt += number_range(-2,3);

if(charcnt<1) charcnt = 1;

for (wch = ch->in_room->people; wch != NULL; wch = wch->next_in_room)
{
switch(atoi(buf))
{
case TEAM_BLUE: do_join(wch,"red"); break;
case TEAM_RED: do_join(wch,"blue"); break;
default: do_join(wch,"none"); break;
}
}

for (i=0;i!=charcnt-1;i++)
{

CHAR_DATA *mob;

pMobIndex = get_mob_index (monsters[number_range(0,mob_top-1)]);

if(pMobIndex == NULL)
continue;

mob = create_mobile(pMobIndex);
char_to_room (mob, ch->in_room);

total = ch->level + number_range(-6,4);
for(i=0;i<total;i++)
advance_level(mob,FALSE);

switch(atoi(buf))
{
case TEAM_BLUE: do_join(mob,"blue"); break;
case TEAM_RED: do_join(mob,"red");  break;
default: do_join(mob,"none");  break;
}

mob->in_room->turn=mob;
mob->AttackLeft=1;
mob->MoveLeft=3;
}

}


int find_char_pos(CHAR_DATA *ch, char *arg, int coord)
{
CHAR_DATA *victim;
if(strlen(arg) > 2)
{
    if ((victim = get_char_room (ch, arg)) == NULL)
    {
        return -2;
    }
    if(coord == 0)
    return victim->mposx;
    else
    return victim->mposy;
}
else
{
if(coord == 0)
return get_coord(arg[0]);
else
return get_coord(arg[1]);
}
}

char * get_t_color(CHAR_DATA *ch)
{
if(ch->team == TEAM_BLUE)
return "{B";
if(ch->team == TEAM_GREEN)
return "{G";
if(ch->team == TEAM_RED)
return "{R";
if(ch->team == TEAM_NONE)
return "{W";
return "{x";
}

int get_coord(char letter)
{
char buf[5];

sprintf(buf,"%c",letter);

if (!strcmp(buf, "1") || !strcmp(buf, "2") || !strcmp(buf, "3") || !strcmp(buf, "4") || !strcmp(buf, "5") || !strcmp(buf, "6") || !strcmp(buf, "7") || !strcmp(buf, "8") || !strcmp(buf, "9") || !strcmp(buf, "0"))
return atoi(buf);
else if (!strcmp(buf, "a") || !strcmp(buf, "A"))
return 10;
else if (!strcmp(buf, "b") || !strcmp(buf, "B"))
return 11;
else if (!strcmp(buf, "c") || !strcmp(buf, "C"))
return 12;
else if (!strcmp(buf, "d") || !strcmp(buf, "D"))
return 13;
else if (!strcmp(buf, "e") || !strcmp(buf, "E"))
return 14;
else if (!strcmp(buf, "f") || !strcmp(buf, "F"))
return 15;
else if (!strcmp(buf, "g") || !strcmp(buf, "G"))
return 16;
else if (!strcmp(buf, "h") || !strcmp(buf, "H"))
return 17;
else if (!strcmp(buf, "i") || !strcmp(buf, "I"))
return 18;
else if (!strcmp(buf, "j") || !strcmp(buf, "J"))
return 19;
else if (!strcmp(buf, "k") || !strcmp(buf, "K"))
return 20;
else if (!strcmp(buf, "l") || !strcmp(buf, "L"))
return 21;
else if (!strcmp(buf, "m") || !strcmp(buf, "M"))
return 22;
else if (!strcmp(buf, "n") || !strcmp(buf, "N"))
return 23;
else if (!strcmp(buf, "o") || !strcmp(buf, "O"))
return 24;
else if (!strcmp(buf, "p") || !strcmp(buf, "P"))
return 25;
else
return -2;       
}

CHAR_DATA * find_char_xy(CHAR_DATA * ch, int x, int y)
{
CHAR_DATA * rch;
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    if(!rch->infight)
    continue;
    if(rch->mposx == x && rch->mposy == y)
    return rch;   
    }
    return NULL;
    } 

int find_distance(int xt,int yt,int yc,int xc)
{
int total = 0;
if(yt > yc) while(yt!=yc) { yt--; total++; }
if(yt < yc) while(yt!=yc) { yt++; total++; }
if(xt > xc) while(xt!=xc) { xt--; total++; }
if(xt < xc) while(xt!=xc) { xt++; total++; }
return total;
}


int hdif(CHAR_DATA *ch, int dir)
{
switch(dir)
{
case DIR_NORTH:
return ch->in_room->map.index[ch->mposy][ch->mposx].height - ch->in_room->map.index[ch->mposy-1][ch->mposx].height;
break;
case DIR_EAST:
return ch->in_room->map.index[ch->mposy][ch->mposx].height - ch->in_room->map.index[ch->mposy][ch->mposx+1].height;
break;
case DIR_WEST:
return ch->in_room->map.index[ch->mposy][ch->mposx].height - ch->in_room->map.index[ch->mposy][ch->mposx-1].height;
break;
case DIR_SOUTH:
return ch->in_room->map.index[ch->mposy][ch->mposx].height - ch->in_room->map.index[ch->mposy+1][ch->mposx].height;
break;
default: 
return 10;
break;
}
}

void do_test (CHAR_DATA * ch, char *argument)
{
printf_to_char(ch,"east : %d, south : %d, west : %d, north : %d\n",hdif(ch,DIR_EAST),hdif(ch,DIR_SOUTH),hdif(ch,DIR_WEST),hdif(ch,DIR_NORTH));
}

void do_fdisp (CHAR_DATA * ch, char *argument)
{
    if (colorstrlen (argument) > 1)
    return;
if(strlen(argument) == 0)
return;

    strcpy(ch->fdisp,argument);     
    printf_to_char(ch,"You will be seen as : [ %s ]",ch->fdisp);
}

void do_attack (CHAR_DATA * ch, char *argument)
{
char arg[MSL];
CHAR_DATA *victim;
int dir;
if(!TURN(ch))
{
printf_to_char(ch,"It's not your turn.");
return;
}

if(ch->AttackLeft < 1)
{
printf_to_char(ch,"You've already attacked.");
return;
}

argument = one_argument (argument, arg);

if (!strcmp (arg, "north"))
{
dir = DIR_NORTH;
victim = find_char_map(ch,DIR_SOUTH);
}
else if (!strcmp (arg, "east"))
{
dir = DIR_EAST;
victim = find_char_map(ch,DIR_EAST);
}
else if (!strcmp (arg, "south"))
{
dir = DIR_SOUTH;
victim = find_char_map(ch,DIR_NORTH);
}
else if (!strcmp (arg, "west"))
{
dir = DIR_WEST;
victim = find_char_map(ch,DIR_WEST);
}
else
{

victim = get_char_room (ch, arg);

if(victim == NULL)
{
printf_to_char(ch,"Not here.\n");
return;
}

if(!can_hit_t(ch,victim->mposy,victim->mposx))
{
printf_to_char(ch,"Out of range.\n");
return;
}

}

if(victim == NULL)
{
printf_to_char(ch,"Nobody there.\n");
return;
}

if(victim->team == ch->team && IS_NPC(ch) && victim->team != TEAM_NONE)
return;


  if(UMAX(ch->in_room->map.index[ch->mposx][ch->mposy].height,ch->in_room->map.index[victim->mposx][victim->mposy].height) - UMIN(ch->in_room->map.index[ch->mposx][ch->mposy].height,ch->in_room->map.index[victim->mposx][victim->mposy].height) > 1)
  {
  send_to_char("Too high/low to attack.",ch);
  return;
  }

ch->AttackLeft--;

    if(skill_lookup("hamedo") == victim->reaction_num && number_range(1,100) < 31)
    f_multi_hit( victim, ch, TYPE_UNDEFINED );
    else 
    {
    f_multi_hit( ch, victim, TYPE_UNDEFINED );
    handle_reaction(ch,victim);
    }
}

void handle_reaction(CHAR_DATA *ch, CHAR_DATA *victim)
{
    if(skill_lookup("counter") == victim->reaction_num)
    {
    if(number_range(1,100) < 60)
    f_multi_hit( victim, ch, TYPE_UNDEFINED );
    } 
}

CHAR_DATA * find_char_map(CHAR_DATA *ch, int dir)
{
CHAR_DATA *rch;

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    if(!rch->infight)
    continue;

    if(rch->mposy == (ch->mposy -1) && rch->mposx == ch->mposx && dir == DIR_SOUTH)
    return rch;
    if(rch->mposy == (ch->mposy +1) && rch->mposx == ch->mposx && dir == DIR_NORTH)
    return rch;
    if(rch->mposx == (ch->mposx -1) && rch->mposy == ch->mposy && dir == DIR_WEST)
    return rch;
    if(rch->mposx == (ch->mposx +1) && rch->mposy == ch->mposy && dir == DIR_EAST)
    return rch;
    }
    return NULL;

}

int colorstrlen(char *str)
{
 int i=0;
 char *tic;
     
 tic = str;
 while(*tic != '\0')
 {
  if(*tic == '{')
  {
   tic += 2;
   continue;
  }
  tic++;   
  i++;
 }
 return i;
} 

void generate_random_terrain(ROOM_INDEX_DATA *room)
{
switch(number_range(1,4))
{
case 1:
generate_random_rooms(room);
break;
case 2:
generate_random_mountains(room);
break;
default:
generate_random_hills(room);
break;
}
}

void generate_random_mountains(ROOM_INDEX_DATA *room)
{

int j,k,l;
for(l=0;l<9;l++)
for(j=0;j<16;j++)
for(k=0;k<26;k++)
{
l = number_range(1,100);
if(l == 6)
room->map.index[j][k].height = 7;
else
room->map.index[j][k].height = 0;
}
for(j=0;j<16;j++)
for(k=0;k<26;k++)
vary(room,j,k);
}


void generate_random_rooms(ROOM_INDEX_DATA *room)
{
int ran1,ran2;
int k;

for(k=1;k<8;k++)
{

ran1 = 0;
ran2 = number_range(0,15);

while(ran1 != 26)
{
if(number_range(1,2) == 2)
ran2++;
else
ran2--;
if(ran2 < 0)
ran2 = 0;
if(ran2 > 15)
ran2 = 15;
if(room->map.index[ran2][ran1].height < k)
room->map.index[ran2][ran1].height = k-2;
else
room->map.index[ran2][ran1].height = k-2;
ran1++;
if(room->map.index[ran2][ran1].height < k)
room->map.index[ran2][ran1].height = k-2;
else
room->map.index[ran2][ran1].height = k-2;
}
}

for(k=1;k<8;k++)
{

ran1 = 0;
ran2 = number_range(0,15);

while(ran1 != 26)
{
if(number_range(1,2) == 2)
ran2++;
else
ran2--;
if(ran2 < 0)
ran2 = 0;
if(ran2 > 15)
ran2 = 15;
if(room->map.index[ran1][ran1].height < k)
room->map.index[ran1][ran2].height = k-2;
else
room->map.index[ran1][ran2].height = k-2;
ran1++;
if(room->map.index[ran1][ran1].height < k)
room->map.index[ran1][ran2].height = k-2;
else
room->map.index[ran1][ran2].height = k-2;
}
}

//for(j=0;j<16;j++)
//for(k=0;k<26;k++)
//vary(room,j,k);
}



int hl()
{
//if(number_range(1,10) == 4)
return 7;
//else
return number_range(0,2);
}



void generate_random_hills(ROOM_INDEX_DATA *room)
{
int i,j,k,l;

for(i=0;i<number_range(2,6);i++)
room->map.index[number_range(0,15)][number_range(0,25)].height = number_range(3,7);

for(l=0;l<9;l++)
for(j=0;j<16;j++)
for(k=0;k<26;k++)
vary(room,j,k);

}

void vary(ROOM_INDEX_DATA *room, int i, int j)
{
int orig = room->map.index[i][j].height;

room->map.index[i][j].occupied = FALSE;
if(room->map.index[i][j-1].height < orig && orig != 0)
room->map.index[i][j-1].height = orig - 1;
if(room->map.index[i][j+1].height < orig && orig != 0)
room->map.index[i][j+1].height = orig - 1;
if(room->map.index[i-1][j].height < orig && orig != 0)
room->map.index[i-1][j].height = orig - 1;
if(room->map.index[i+1][j].height < orig && orig != 0)
room->map.index[i+1][j].height = orig - 1;
 if(room->map.index[i-1][j-1].height < orig && orig != 0)
room->map.index[i-1][j-1].height = orig - 1;
if(room->map.index[i+1][j+1].height < orig && orig != 0)
room->map.index[i+1][j+1].height = orig - 1;
if(room->map.index[i-1][j+1].height < orig && orig != 0)
room->map.index[i-1][j+1].height = orig - 1;
if(room->map.index[i+1][j-1].height < orig && orig != 0)
room->map.index[i+1][j-1].height = orig - 1;
}

void do_join (CHAR_DATA * ch, char *argument)
{
int i,j;

char arg[MSL];
argument = one_argument (argument, arg);

if(ch->infight)
{
send_to_char("You are already fighting.\n\r",ch);
return;
}

if(ch->in_room->map.closed)
{
send_to_char("Sorry, this fight is closed, but you can still watch.\n\r",ch);
return;
}

if(strlen(ch->fdisp)<1)
do_autodisp(ch," ");

if (!strcmp (arg, "blue"))
{
ch->team = TEAM_BLUE;
}
else if (!strcmp (arg, "red"))
{
ch->team = TEAM_RED;
}
else if (!strcmp (arg, "green"))
{
ch->team = TEAM_GREEN;
}
else if (!strcmp (arg, "none"))
{
ch->team = TEAM_NONE;
}
else
{
send_to_char("You need to specify a side to join (red,green,blue,none)\n\r",ch);
return;
}
calc_move(ch);

if(IS_SET (ch->in_room->room_flags, ROOM_SAFE))
DIE(ch,"No fighting here.");

if(!ch->in_room->map.init)
{
ch->in_room->map.init = TRUE;
generate_random_terrain(ch->in_room);
}


act ("$n joins the fight!", ch, NULL, NULL, TO_ROOM);
send_to_char("You join the fight!\n\r",ch);

j = number_range(1,25);
i = number_range(1,15);

while(ch->in_room->map.index[i][j].occupied == TRUE)
{
j = number_range(1,25);
i = number_range(1,15);
}

ch->AT = number_range(1,97);

ch->mposy = i;
ch->mposx = j;
ch->in_room->map.index[i][j].occupied = TRUE;
ch->infight = TRUE;
}

void do_map (CHAR_DATA * ch, char *argument)
{
char arg[MSL];

refresh(ch->in_room);

argument = one_argument (argument, arg);

if (!strcmp (arg, "forced"))
fmap(ch,TRUE,FALSE," ");
else if (!strcmp (arg, "range"))
fmap(ch,FALSE,TRUE," ");
else
fmap(ch,FALSE,FALSE," ");
}

void fmap (CHAR_DATA * ch, bool forced, bool range, char *argument)
{
char buf1[70];
char buf2[70];
char buf3[70];
int i,j;
int total = 0;
CHAR_DATA * fighters[16];
int fightcount = 0;
CHAR_DATA *rch;
int theturn = 0;

if(!ch->in_room->map.init)
{
ch->in_room->map.init = TRUE;
generate_random_terrain(ch->in_room);
}   

if (IS_SET (ch->comm, COMM_NOMAP) && forced == TRUE)
return;

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    if(rch->infight)
        {
        if(TURN(rch))
        theturn = 1;
        fighters[fightcount] = rch;
        fightcount++;
        }
     }

printf_to_char(ch,"\n\r%s",echo_off_str);

printf_to_char(ch," {y 0123456789abcdefghijklmnop{x\n\r");
printf_to_char(ch," ----------------------------  |-----------------------------------------------|\n\r");
for(i=0;i<16;i++)
{
char buf2[MSL];
printf_to_char(ch,"{W%c|{x",gridname[i]);
for(j=0;j<26;j++)
{
total += parse_room(ch,i,j,range);
}
if(i < fightcount)
{

sprintf(buf2," %s{x{w{W - %s%-19s {C %3dAT %5dHP %5dMP{W",fighters[i]->fdisp,get_t_color(fighters[i]),fcn(fighters[i]),fighters[i]->AT,fighters[i]->hit,fighters[i]->mana);
}
else
sprintf(buf2,"{x%47s{x"," ");
printf_to_char(ch,"{W|%c |%s|\n\r",gridname[i],buf2);
}

if(ch->in_room->turn != NULL)
{
char buf7[40];
CHAR_DATA *rch = ch->in_room->turn;

if(rch->level > 99)
sprintf(buf7,"{gL{W**{c ---{bexp{W             | PA MA BR FA MV JM SP |");
else
sprintf(buf7,"{gL{R%2d{c %3d{bexp{W             | PA MA BR FA MV JM SP |",rch->level,((rch->level + 1) * 100) - rch->exp);
 
sprintf(buf1," %s%-23s{W|{r%4d{c/{r%4d{RHP {g%3d{c/{g%3d{GMP{W |",get_t_color(rch),fcn(rch),rch->hit,rch->max_hit,rch->mana,rch->max_mana);
sprintf(buf2," %s",buf7);
sprintf(buf3,"                        | %2d %2d %2d %2d %2d %2d %2d {W|",rch->PhysAttack,rch->MagAttack,
rch->Brave,rch->Faith,rch->MoveLeft,rch->jump,rch->speed);
printf_to_char(ch," {W----------------------------  |-----------------------------------------------|\n\r");
printf_to_char(ch," |{y0123456789abcdefghijklmnop{W|  |%-44s",buf1);
//parse_room(ch,(rch->mposy - 1),(rch->mposx - 1),FALSE);
//parse_room(ch,(rch->mposy - 1),rch->mposx,FALSE);
//parse_room(ch,(rch->mposy - 1),(rch->mposx + 1),FALSE);
printf_to_char(ch,"{W\n\r");
printf_to_char(ch," {W----------------------------  |%-44s",buf2);
//parse_room(ch,rch->mposy,(rch->mposx - 1),FALSE);
//parse_room(ch,rch->mposy,rch->mposx,FALSE);
//parse_room(ch,rch->mposy,(rch->mposx + 1),FALSE);
printf_to_char(ch,"{W\n\r");
printf_to_char(ch," {W|%-26s|  |%-44s",ch->in_room->name,buf3);
//parse_room(ch,(rch->mposy + 1),(rch->mposx - 1),FALSE);
//parse_room(ch,(rch->mposy + 1),rch->mposx,FALSE);
//parse_room(ch,(rch->mposy + 1),(rch->mposx + 1),FALSE);
printf_to_char(ch,"{W\n\r");
}


printf_to_char(ch," ----------------------------  |-----------------------------------------------|\n\r");



check_winner(ch->in_room);
printf_to_char(ch,"%s", echo_on_str);

}



void check_winner(ROOM_INDEX_DATA *ro)
{
bool found=FALSE;
int redct = 0;
int bluect = 0;
int greenct = 0;
int nonect = 0;
int total = 0;
CHAR_DATA *rch;

    for (rch = ro->people; rch != NULL; rch = rch->next_in_room)
    {
    if(rch == NULL)
    continue;

    if(rch->infight)
    {
if(TURN(rch))
found=TRUE;    
    total++;
    if(rch->team == TEAM_BLUE) 
    bluect++;
    else if(rch->team == TEAM_RED) 
    redct++;
    else if(rch->team == TEAM_GREEN) 
    greenct++;
    else if(rch->team == TEAM_NONE) 
    nonect++;
    }
    }

if(total == 1
|| bluect == total || greenct == total || redct == total)
{
    for (rch = ro->people; rch != NULL; rch = rch->next_in_room)
    {
    if(rch->infight)
    {
    printf_to_char(rch,"\n\r{Y* * * B A T T L E  C O M P L E T E * * *\n\r{x");
    printf_to_char(rch,"{B      Uh, you won... and stuff.         \n\r{x");
    rch->infight = FALSE;
    rch->mposy = 0;
    rch->team = 0;
    rch->in_room->turn=NULL;
    rch->mposx = 0;
    }
    }
}
}   



int parse_room(CHAR_DATA *ch, int x, int y,bool range)
{
CHAR_DATA *rch;
    
char rang[3];

    if(can_hit_t(ch,x,y) && range) 
    sprintf(rang,"{)");
    else    
    strcpy(rang,"");

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
 	if(rch->mposy == x && rch->mposx == y && rch->infight)
	{
	 printf_to_char(ch,"{!%s%s{x",rang,rch->fdisp);
	 return 1; 
	}

    }

    
switch(ch->in_room->map.index[x][y].height)
{
case -99:
printf_to_char(ch," ");
break;
case 0:
printf_to_char(ch,"%s.",rang);
break;
case 1:
printf_to_char(ch,"{w%s1{x",rang);
break;
case 2:
printf_to_char(ch,"{b%s2{x",rang);
break;
case 3:
printf_to_char(ch,"{c%s3{x",rang);
break;
case 4:
printf_to_char(ch,"{g%s4{x",rang);
break;
case 5:
printf_to_char(ch,"{m%s5{x",rang);
break;
case 6:
printf_to_char(ch,"{y%s6{x",rang);
break;
case 7:
default:
printf_to_char(ch,"{r%s7{x",rang);
break;

}
printf_to_char(ch,"{x");
return 0;
}


void announce_move(CHAR_DATA * ch, char * thedir)
{
CHAR_DATA *rch;

ch->MoveLeft--;
   
     for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {   
         printf_to_char(rch,"%s moves %s.{x\n",fcn(ch),thedir);
//	 do_map(rch,"forced");
    }
}

void do_done(CHAR_DATA * ch, char * thedir)
{
CHAR_DATA *rch;
if(!ch->infight)
{
printf_to_char(ch,"You're not in a fight.");
return;
}

if(!TURN(ch))
{
printf_to_char(ch,"It's not your turn.");
return;
}
   
    check_winner(ch->in_room);
    
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {  
         printf_to_char(rch,"%s ends %s turn.\n",fcn(ch), ch->sex ? "his" : "her");
         do_map(rch,"forced");
         rch->AT++;
    }
       ch->AT = 0;

advance_turn(ch); 
}

    

void tile_spell(CHAR_DATA *ch, int sn)
{
void * vo;
CHAR_DATA *rch;
bool hit;
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    say_spell(ch,sn);
    if((rch->mposx == ch->CastTargX && rch->mposy == ch->CastTargY))
    {
    vo = (CHAR_DATA *) rch;
    if (IS_NPC (ch) )
    (*skill_table[sn].spell_fun) (ch->SnToCast, ch->level, ch, vo, 0);
    else
    (*skill_table[sn].spell_fun) (ch->SnToCast, 3 * ch->level / 4, ch, vo, 0);
    hit = TRUE;
    }
 
    if((rch->mposx == ch->CastTargX && rch->mposy == ch->CastTargY-1))
    {
    vo = (CHAR_DATA *) rch;
    if (IS_NPC (ch) )
    (*skill_table[sn].spell_fun) (ch->SnToCast, ch->level, ch, vo, 0);
    else
    (*skill_table[sn].spell_fun) (ch->SnToCast, 3 * ch->level / 4, ch, vo, 0);
    hit = TRUE;
    }

    if((rch->mposx == ch->CastTargX-1 && rch->mposy == ch->CastTargY))
    {
    vo = (CHAR_DATA *) rch;
    if (IS_NPC (ch) )
    (*skill_table[sn].spell_fun) (ch->SnToCast, ch->level, ch, vo, 0);
    else
    (*skill_table[sn].spell_fun) (ch->SnToCast, 3 * ch->level / 4, ch, vo, 0);
    hit = TRUE;
    }

    if((rch->mposx == ch->CastTargX+1 && rch->mposy == ch->CastTargY))
    {
    vo = (CHAR_DATA *) rch;
    if (IS_NPC (ch) )
    (*skill_table[sn].spell_fun) (ch->SnToCast, ch->level, ch, vo, 0);
    else
    (*skill_table[sn].spell_fun) (ch->SnToCast, 3 * ch->level / 4, ch, vo, 0);
    hit = TRUE;
    }

    if((rch->mposx == ch->CastTargX && rch->mposy == ch->CastTargY+1))
    {
    vo = (CHAR_DATA *) rch;
    if (IS_NPC (ch) )
    (*skill_table[sn].spell_fun) (ch->SnToCast, ch->level, ch, vo, 0);
    else
    (*skill_table[sn].spell_fun) (ch->SnToCast, 3 * ch->level / 4, ch, vo, 0);
    hit = TRUE;
    }

}    

}

void refresh(ROOM_INDEX_DATA *r)
{
CHAR_DATA *rch;    
int y;
int x;
for(y=0;y<16;y++)
for(x=0;x<26;x++)
for (rch = r->people; rch != NULL; rch = rch->next_in_room)
    {
        if(rch->mposx == x && rch->mposy == y && rch->infight)
        r->map.index[y][x].occupied = TRUE;
        else
        r->map.index[y][x].occupied = FALSE;
    }
if(r->turn == NULL)
advance_turn( r->people);
}



void advance_turn(CHAR_DATA * ch)
{
CHAR_DATA *rch;
int q=0;

for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
if(rch->infight)
q++;    
}
if(q<1);
return;

    check_winner(ch->in_room);

q = 0;
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {  
    if(rch == NULL || !rch->infight)
    continue;
    do_map(rch,"forced");
    }

ch->in_room->next = NULL;
while(ch->in_room->next == NULL)
{
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
    if(rch == NULL || !rch->infight)
    continue;
    
    if(rch->ATToCast < rch->AT)
    {
    say_spell (rch, rch->SnToCast);
    tile_spell(rch, rch->SnToCast);
    rch->ATToCast = 0;
    rch->SnToCast = 0;
    rch->casting = FALSE;
    rch->CastTargY = 0;
    rch->CastTargX = 0;   
    }

    rch->AT += rch->speed;
    if(rch->AT >= 100)
    {
    rch->AT = 100;
    ch->in_room->turn = rch;
    break;
    }
    }
}

    ch->in_room->turn->MoveLeft = ch->in_room->turn->base_move;
    ch->in_room->turn->AttackLeft = 1;

    printf_to_char(rch,"%s's turn.\n",fcn(ch->in_room->turn));

}

char * fcn(CHAR_DATA *ch)
{
char buf[200];
char * buf2;
if(IS_NPC(ch))
{
sprintf(buf,"%s (%s)",ch->f_name,ch->short_descr);
buf2 = str_dup(buf);
return buf2;
}
else
return ch->name;
}

