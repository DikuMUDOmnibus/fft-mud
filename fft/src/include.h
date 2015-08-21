#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <time.h>
#include <unistd.h>                /* OLC -- for close read write etc */
#include <stdarg.h>                /* printf_to_char */
#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"

char * fcn(CHAR_DATA *ch);
int parse_room(CHAR_DATA *ch, int x, int y,bool range);
void generate_random_hills(ROOM_INDEX_DATA *room);
void vary(ROOM_INDEX_DATA *room, int i, int j);
CHAR_DATA * find_char_map(CHAR_DATA *ch, int dir);
void fmap (CHAR_DATA * ch, bool forced, bool range, char *argument);
int find_distance(int xt,int yt,int yc,int xc);
void refresh(ROOM_INDEX_DATA *r);
void tile_spell(CHAR_DATA *ch, int sn);
int hdif(CHAR_DATA *ch, int dir);
int get_coord(char letter);
int get_char_pos(CHAR_DATA *ch, CHAR_DATA *targ,int coord);
int find_char_pos(CHAR_DATA *ch, char *arg, int coord);
CHAR_DATA * find_char_xy(CHAR_DATA *ch, int x, int y);
bool can_hit_t(CHAR_DATA *ch,int x,int y);

void ai_move(CHAR_DATA *ch);
void ai_kill(CHAR_DATA *ch);
void ai_sit(CHAR_DATA *ch);
