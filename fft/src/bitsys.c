/*******************************************************************\
|* This file is support functions for Ammaross Danan's Unlimited   *|
|* Bit System. This header is to remain intact.                    *|
|*  - Kirk Johnson aka Ammaross Danan (ammaross@ardenmore.com)     *|
|* http://www.ardenmore.com  ARDENMOREDOI   mud.ardenmore.com:5150 *|
\*******************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"
#include "lookup.h"
#include "olc.h"


/* Moved to here from macro form to solve a read-in problem. */
void STR_COPY_STR( FLAG *a, const FLAG *b, const int size )
{
	int i;

	for( i = 0; i < size; i++ )
	{
		a[i] = b[i];
	}
}

bool STR_IS_ZERO( FLAG * var, int size )
{
	int i;

	for( i = 0; i < size; i++ )
	{
		if( var[i] != 0 )
			return FALSE;
	}
	return TRUE;
}

bool STR_AND( FLAG * var1, FLAG * var2, int size )
{
	int i;

	for( i = 0; i < size; i++ )
	{
		if( var1[i] & var2[i] )
			return TRUE;
	}
	return FALSE;
}


/*
 * Purpose:	Returns the value of the flags entered.
 */
int str_flag_value( const struct flag_type *flag_table, char *argument)
{
    char name[MAX_INPUT_LENGTH];
    int  flag;

	argument = one_argument( argument, name );

    if ( name[0] == '\0' )
	    return NO_FLAG;

  	for (flag = 0; flag_table[flag].name != NULL; flag++)
   	{
		if (LOWER(name[0]) == LOWER(flag_table[flag].name[0])
		&&  !str_prefix(name,flag_table[flag].name))
		    return flag_table[flag].bit;
   	}

	return NO_FLAG;
}


/*
 * Purpose:	Returns string with name(s) of the flags or stat entered.
 */
char *str_flag_string( const struct flag_type *flag_table, FLAG *bits )
{
static char buf[10][MIL];
    int  flag;
static int toggle;

	toggle = (++toggle) % 10;

    buf[toggle][0] = '\0';

    for (flag = 0; !IS_NULLSTR( flag_table[flag].name ); flag++)
    {
		if ( STR_IS_SET(bits, flag_table[flag].bit) )
		{
		    strcat( buf[toggle], " " );
		    strcat( buf[toggle], flag_table[flag].name );
		}
    }
    
    return (buf[toggle][0] != '\0') ? buf[toggle] + 1 : "none";
}


/*
 * Purpose: Used to read in bit arrays.
 */
FLAG *str_fread_flag( FILE *fp, const int size )
{
	int i = 0;
	int number;
static FLAG flag[MAX_FLAGS];

	STR_ZERO_STR( flag, MAX_FLAGS );

	while ( (number = fread_number(fp)) != -1 )
	{
		if ( i < size )
			flag[i++] = number;
	}

	return flag;
}


/*
 * Purpose: Used to write flags in the unlimited bit system.
 * This setup allows the saved array to expand OR contract
 * and still be able to load properly.
 */
char *str_fwrite_flag( FLAG *flags, const int size, char *out )
{
	char buf[MIL];
	int i;

	out[0] = '\0';

	for ( i = 0; i < size; i++ )
	{
		sprintf( buf, "%d ", flags[i] );
		strcat( out, buf );
	}

	sprintf( buf, "-1 " );
	strcat( out, buf );

	return out;
}


/*
 * Purpose: Used to write flags to file in the unlimited bit system.
 * This setup allows the saved array to expand OR contract
 * and still be able to load properly.
 */
char *str_print_flags( FLAG *flags, const int size )
{
static char buf[MIL];
	char buf2[MIL];
	int i;

	buf[0] = '\0';

	for ( i = 0; i < size; i++ )
	{
		sprintf( buf2, "%d ", flags[i] );
		strcat( buf, buf2 );
	}

	sprintf( buf2, "-1" );
	strcat( buf, buf2 );

	return buf;
}


/*
 * Used to get non-converted flags to work well under the new bit system
 */
void do_flag_norm( CHAR_DATA *ch, CHAR_DATA *victim, char *argument,
					char type, long *flag, const struct flag_type *flag_table )
{
	char word[MIL];
	long old = 0, new = 0, marked = 0, pos;

	old = *flag;
	victim->zone = NULL;

	if ( type != '=' )
		new = old;

	/* mark the words */
	for (;;)
	{
		argument = one_argument( argument,word );

		if ( IS_NULLSTR( word ) )
			break;

		pos = flag_lookup(word,flag_table);
		if (pos == 0)
		{
			send_to_char("That flag doesn't exist!\n\r",ch);
			return;
		}
		else
			SET_BIT(marked,pos);
	}

	for (pos = 0; flag_table[pos].name != NULL; pos++)
	{
	    if (!flag_table[pos].settable && IS_SET(old,flag_table[pos].bit))
	    {
			SET_BIT(new,flag_table[pos].bit);
			continue;
	    }

	    if (IS_SET(marked,flag_table[pos].bit))
	    {
			switch(type)
			{
			    case '=':
			    case '+':
					SET_BIT(new,flag_table[pos].bit);
					break;
			    case '-':
					REMOVE_BIT(new,flag_table[pos].bit);
					break;
			    default:
					if (IS_SET(new,flag_table[pos].bit))
					    REMOVE_BIT(new,flag_table[pos].bit);
					else
					    SET_BIT(new,flag_table[pos].bit);
			}
    	}
	}
	*flag = new;
	return;
}


/*
 * Modified for use with unlimited bit flags
 * Expandable if you want to want to convert COMM, RES, etc. over.
 */
void do_flag(CHAR_DATA *ch, char *argument)
{
    char arg1[MIL];
    char arg2[MIL];
    char arg3[MIL];
    char word[MIL];
    CHAR_DATA *victim;
    FLAG *flag = NULL;
    FLAG old[MAX_FLAGS];
    FLAG new[MAX_FLAGS];
    FLAG marked[MAX_FLAGS];
    int pos, size;
    char type;
    const struct flag_type *flag_table;

	STR_ZERO_STR( old, MAX_FLAGS );
	STR_ZERO_STR( new, MAX_FLAGS );
	STR_ZERO_STR( marked, MAX_FLAGS );

    argument = one_argument(argument,arg1);
    argument = one_argument(argument,arg2);
    argument = one_argument(argument,arg3);

    type = argument[0];

    if (type == '=' || type == '-' || type == '+')
        argument = one_argument(argument,word);

    if ( IS_NULLSTR( arg1 ) )
    {
		send_to_char("Syntax:\n\r",ch);
		send_to_char("  flag mob  <name> <field> <flags>\n\r",ch);
		send_to_char("  flag char <name> <field> <flags>\n\r",ch);
		send_to_char("  mob  flags: act,aff,off,imm,res,vuln,form,part\n\r",ch);
		send_to_char("  char flags: plr,comm,aff,imm,res,vuln,\n\r",ch);
		send_to_char("  +: add flag, -: remove flag, = set equal to\n\r",ch);
		send_to_char("  otherwise flag toggles the flags listed.\n\r",ch);
		return;
    }

    if ( IS_NULLSTR( arg2 ) )
    {
		send_to_char("What do you wish to set flags on?\n\r",ch);
		return;
    }

    if ( IS_NULLSTR( arg3 ) )
    {
		send_to_char("You need to specify a flag to set.\n\r",ch);
		return;
    }

    if ( IS_NULLSTR( argument ) )
    {
		send_to_char("Which flags do you wish to change?\n\r",ch);
		return;
    }

    if (!str_prefix(arg1,"mob") || !str_prefix(arg1,"char"))
    {
		victim = get_char_world(ch,arg2);
		if (victim == NULL)
		{
		    send_to_char("You can't find them.\n\r",ch);
		    return;
		}

        /* select a flag to set */
		if (!str_prefix(arg3,"act"))
		{
		    if (!IS_NPC(victim))
		    {
				send_to_char("Use plr for PCs.\n\r",ch);
				return;
		    }

			do_flag_norm( ch, victim, argument, type, &victim->form, form_flags );
			return;
		}

		else if (!str_prefix(arg3,"plr"))
		{
		    if (IS_NPC(victim))
		    {
				send_to_char("Use act for NPCs.\n\r",ch);
				return;
		    }

			do_flag_norm( ch, victim, argument, type, &victim->form, plr_flags );
			return;
		}

	 	else if (!str_prefix(arg3,"aff"))
		{
			size = AFF_FLAGS;
		    flag = victim->affected_by;
		    flag_table = affect_flags;
		}

	  	else if (!str_prefix(arg3,"immunity"))
		{
			do_flag_norm( ch, victim, argument, type, &victim->imm_flags, imm_flags );
			return;
		}

		else if (!str_prefix(arg3,"resist"))
		{
			do_flag_norm( ch, victim, argument, type, &victim->res_flags, res_flags );
			return;
		}

		else if (!str_prefix(arg3,"vuln"))
		{
			do_flag_norm( ch, victim, argument, type, &victim->vuln_flags, vuln_flags );
			return;
		}

		else if (!str_prefix(arg3,"form"))
		{
		    if (!IS_NPC(victim))
		    {
			 	send_to_char("Form can't be set on PCs.\n\r",ch);
				return;
		    }

			do_flag_norm( ch, victim, argument, type, &victim->form, form_flags );
			return;
		}
	
		else if (!str_prefix(arg3,"parts"))
		{
		    if (!IS_NPC(victim))
		    {
				send_to_char("Parts can't be set on PCs.\n\r",ch);
				return;
		    }

			do_flag_norm( ch, victim, argument, type, &victim->parts, part_flags );
			return;
		}

		else if (!str_prefix(arg3,"comm"))
		{
		    if (IS_NPC(victim))
		    {
				send_to_char("Comm can't be set on NPCs.\n\r",ch);
				return;
		    }

			do_flag_norm( ch, victim, argument, type, &victim->comm, comm_flags );
			return;
		}

		else 
		{
		    send_to_char("That's not an acceptable flag.\n\r",ch);
		    return;
		}
		
		STR_COPY_STR( old, flag, MAX_FLAGS );
		victim->zone = NULL;

		if (type != '=')
		    STR_COPY_STR( new, old, MAX_FLAGS );

        /* mark the words */
        for (; ;)
        {
		    argument = one_argument(argument,word);

		    if ( IS_NULLSTR( word ) )
				break;

		    pos = flag_lookup(word,flag_table);
		    if (pos == 0)
		    {
				send_to_char("That flag doesn't exist!\n\r",ch);
				return;
		    }
		    else
				STR_SET_BIT(marked,pos);
		}

		for (pos = 0; !IS_NULLSTR( flag_table[pos].name ); pos++)
		{
		    if (!flag_table[pos].settable && STR_IS_SET(old,flag_table[pos].bit))
		    {
				STR_SET_BIT(new,flag_table[pos].bit);
				continue;
		    }

		    if (STR_IS_SET(marked,flag_table[pos].bit))
		    {
				switch(type)
				{
				    case '=':
				    case '+':
						STR_SET_BIT(new,flag_table[pos].bit);
						break;
				    case '-':
						STR_REMOVE_BIT(new,flag_table[pos].bit);
						break;
				    default:
						if (STR_IS_SET(new,flag_table[pos].bit))
						    STR_REMOVE_BIT(new,flag_table[pos].bit);
						else
						    STR_SET_BIT(new,flag_table[pos].bit);
				}
	    	}
		}
		STR_COPY_STR( flag, new, size );
		return;
    }
}


/*
 * Return ascii name of an affect bit vector.
 */
char *affect_str_bit_name( FLAG *vector )
{
    static char buf[MSL];

    buf[0] = '\0';

	if (STR_IS_SET(vector, AFF_POISON)		)
		strcat( buf, " poison"		);
	if (STR_IS_SET(vector, AFF_SLOW)		)
		strcat( buf, " slow"		);
	if (STR_IS_SET(vector, AFF_HASTE)		)
		strcat( buf, " haste"		);
	if (STR_IS_SET(vector, AFF_STOP)		)
		strcat( buf, " stop"		);
	if (STR_IS_SET(vector, AFF_CHICKEN)		)
		strcat( buf, " chicken"		);

	/* Just add any additional AFF flags you may have here.
	 * Yes, even your AFF2 flags */
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}


FLAG *aff_convert_fread_flag( long bits )
{
static FLAG flags[AFF_FLAGS];
	long i;
	int j;

	STR_ZERO_STR( flags, AFF_FLAGS );

	if ( bits == 0 )
		return flags;

	for ( j = 1, i = 1; i <= ee && j < 32; j++, i*=2 )
	{
		if ( IS_SET( bits, i ) )
			STR_SET_BIT( flags, j );

		if ( i == ee )
			break;
	}

	return flags;
}


/* Used to convert aff bits read in as bitvectors in affect structs */
int aff_bit_convert_fread_flag( long bits )
{
	long i;
	int j;

	if ( bits == 0 )
		return 0;

	for ( j = 1, i = 1; i <= ee && j < 32; j++, i*=2 )
	{
		if ( IS_SET( bits, i ) )
			return j;

		if ( i == ee )
			break;
	}

	return 0;
}


/*
 * If you have an aff2 type snippet added in, put this in:

FLAG *aff2_convert_fread_flag( long bits )
{
static FLAG flags[AFF_FLAGS];
	long i;
	int j;

	STR_ZERO_STR( flags, AFF_FLAGS );

	if ( bits == 0 )
		return flags;

	for ( j = 32, i = 1; i <= ee && j < AFF_FLAGS*FLAG_BITSIZE; j++, i*=2 )
	{
		if ( IS_SET( bits, i ) )
			STR_SET_BIT( flags, j );

		if ( i == ee )
			break;
	}

	return flags;
}
*/

/*
 * These aff conversions assume that you did a straight A = 1, B = 2, C = 3,
 * type changing in your defines in merc.h. If there are any variations,
 * here is the function I used when I converted my ACT flags:

FLAG *act_convert_fread_flag( long bits )
{
const int act_convert[32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 0,
							13, 14, 15, 16,	17, 18, 19, 20, 21, 0, 22, 0, 23,
							24, 25, 26, 27 };
static FLAG flags[ACT_FLAGS];
	long i;
	int j;

	STR_ZERO_STR( flags, ACT_FLAGS );

	if ( bits == 0 )
		return flags;

	for ( j = 1, i = 1; i <= ee && j < 32; j++, i*=2 )
	{
		if ( IS_SET( bits, i ) )
			STR_SET_BIT( flags, act_convert[j] );

		if ( i == ee )
			break;
	}

	return flags;
}
*/

/*
 * The only thing you will need to change is the flag types of course
 * (ACT_FLAGS to COMM_FLAGS for example) and the convert[32] table.
 * convert[0] space should always equal 0. The next slots start
 * counting A, B, C, D, etc. Just place the in corresponding number.
 * IE, if you had AFF_INVISIBLE defined as A, but you defined it as 3
 * this time, you would put a 3 in convert[1] spot. This function
 * example was used mainly because ACT has gaps in its letter defines
 * (filled in by zeroes in the convert[32] table).
 */

/*
 * If you wanted to save space while converting over other flags that
 * do not have gaps in their old defines (similar to AFF), this is the
 * function I used to convert my COMM flags. Note: you do not need to
 * add this if you will not be converting your COMM flags.

FLAG *comm_convert_fread_flag( long bits )
{
static FLAG flags[COMM_FLAGS];

	STR_ZERO_STR( flags, COMM_FLAGS );
	STR_COPY_STR( flags, aff_convert_fread_flag( bits ), COMM_FLAGS );

	return flags;
}
*/
