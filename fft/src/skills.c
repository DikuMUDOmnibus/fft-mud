 

#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "interp.h"
#include "magic.h"
#include "recycle.h"




void do_skills (CHAR_DATA * ch, char *argument)
{
    int sn;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    if (IS_NPC (ch))
        return;

printf_to_char(ch,"%s skills\n\r",job_table[ch->jobid].name);
printf_to_char(ch,"---------------------\n\r",ch);

for (sn = 0; sn < MAX_SKILL; sn++)
{
if(skill_table[sn].job_num == ch->jobid && skill_table[sn].desc != NULL)
{
switch(skill_table[sn].type)
{
case SKILL_REACT: sprintf(buf,"{RReaction"); break;
case SKILL_MOVE: sprintf(buf,"{GMove"); break;
case SKILL_MISC: sprintf(buf,"{BSupport"); break;
case SKILL_JOB: sprintf(buf,"{MJob"); break;
case SKILL_UNSET: sprintf(buf,"Debug"); break;
}

sprintf(buf2,"%s",skill_table[sn].name);

if(!ch->pcdata->learned[sn])
printf_to_char(ch,"{y[{C%-3d {y%-15s %-10s {W%-25s{y]{x\n\r",skill_table[sn].jp_cost,buf2,buf,skill_table[sn].desc);
else
printf_to_char(ch,"{y[{C---{y %-15s %-10s {W%-25s{y]{x\n\r",buf2,buf,skill_table[sn].desc);
}
}



}

int exp_per_level (CHAR_DATA * ch, int points)
{
    return 100; // HA!
}
