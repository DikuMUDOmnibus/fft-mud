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
 **************************************************************************/

/***************************************************************************
 *   ROM 2.4 is copyright 1993-1998 Russ Taylor                            *
 *   ROM has been brought to you by the ROM consortium                     *
 *       Russ Taylor (rtaylor@hypercube.org)                               *
 *       Gabrielle Taylor (gtaylor@hypercube.org)                          *
 *       Brian Moore (zump@rom.org)                                        *
 *   By using this code, you have agreed to follow the terms of the        *
 *   ROM license, in the file Rom24/doc/rom.license                        *
 **************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"

const struct job_type job_table[MAX_JOB] = {
{ 0 ,"Squire",    "Basic Skill"   ,-1,-1,-1,-1,-1,-1,100,75 ,90 ,90 ,100,WEAPON_DAGGER,4,3,5},
{ 1 ,"Chemist",   "Item"          ,0 ,1 ,-1,-1,-1,-1,80 ,75 ,75 ,80 ,100,WEAPON_DAGGER,3,3,5},
{ 2 ,"Knight",    "Knight Skill"  ,0 ,1 ,-1,-1,-1,-1,120,80 ,120,80 ,100,WEAPON_DAGGER,3,3,10},
{ 3 ,"Archer",    "Charge"        ,0 ,1 ,-1,-1,-1,-1,100,65 ,110,80 ,100,WEAPON_DAGGER,3,3,10},
{ 4 ,"Monk",      "Punch Art"     ,0 ,1 ,-1,-1,-1,-1,135,80 ,129,80 ,110,WEAPON_DAGGER,3,4,20},
{ 5 ,"Priest",    "White Magic"   ,0 ,1 ,-1,-1,-1,-1,80 ,120,90 ,110,110,WEAPON_DAGGER,3,3,5},
{ 6 ,"Wizard",    "Black Magic"   ,0 ,1 ,-1,-1,-1,-1,75 ,120,60 ,150,100,WEAPON_DAGGER,3,3,5},
{ 7 ,"TimeMage",  "Time Magic"    ,0 ,1 ,-1,-1,-1,-1,75 ,120,50 ,130,100,WEAPON_DAGGER,3,3,5},
{ 8 ,"Summoner",  "Summon"        ,0 ,1 ,-1,-1,-1,-1,70 ,125,50 ,125, 90,WEAPON_DAGGER,3,3,5},
{ 9 ,"Thief",     "Steal"         ,0 ,1 ,-1,-1,-1,-1,90 ,50 ,100,60 ,110,WEAPON_DAGGER,4,4,25},
{ 10,"Mediator",  "Talk Skill"    ,0 ,1 ,-1,-1,-1,-1,80 ,70 ,75 ,75 ,100,WEAPON_DAGGER,3,3,5},
{ 11,"Oracle",    "YY Magic"      ,0 ,1 ,-1,-1,-1,-1,75 ,110,50 ,120,100,WEAPON_DAGGER,3,3,5},
{ 12,"Geomancer", "Geomancy"      ,0 ,1 ,-1,-1,-1,-1,110,95 ,110,105,100,WEAPON_DAGGER,4,3,10},
{ 13,"Lancer"    ,"Jump"          ,0 ,1 ,-1,-1,-1,-1,120,50 ,50 ,100,100,WEAPON_DAGGER,4,3,15},
{ 14,"Samurai"   ,"????"          ,0 ,1 ,-1,-1,-1,-1,75 ,75 ,128,90 ,100,WEAPON_DAGGER,3,3,20},
{ 15,"Ninja"     ,"Throw"         ,0 ,1 ,-1,-1,-1,-1,70 ,50 ,120,75 ,120,WEAPON_DAGGER,3,3,30},
{ 16,"Calculator","Calc"          ,0 ,1 ,-1,-1,-1,-1,65 ,80 ,50 ,70 ,50 ,WEAPON_DAGGER,3,3,5},
{ -1,NULL       ,"Goat Turd",0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

/* the big smelly thing*/

const struct f_spell f_mag_table[] = {
{ 0,"NullSpell",	0 ,0 ,ELE_NON,FALSE,FALSE,FALSE,0,0,0,FALSE },
{ 1,"Bahamut",		46,60,ELE_NON,FALSE,FALSE,FALSE,4,4,3,FALSE },
{ 2,"Bio",		12,8, ELE_NON,TRUE ,TRUE ,FALSE,4,2,3,TRUE  },
{ 3,"Bio3",		24,24,ELE_NON,TRUE ,TRUE ,FALSE,4,3,3,TRUE  },
{ 4,"Bolt",		14,6, ELE_BOL,TRUE ,TRUE ,TRUE ,4,2,1,TRUE  },
{ 5,"Bolt2",		18,10,ELE_BOL,TRUE ,TRUE ,TRUE ,4,2,2,TRUE  },
{ 6,"Bolt3",		24,24,ELE_BOL,TRUE ,TRUE ,TRUE ,4,2,3,TRUE  },
{ 7,"Bolt4",		32,48,ELE_BOL,FALSE,FALSE,FALSE,4,3,3,TRUE  },
{ 8,"Cure",		14, 6,ELE_NON,FALSE,TRUE ,TRUE ,4,2,1,FALSE },
{ 9,"Cure2",		20,12,ELE_NON,FALSE,TRUE ,TRUE ,4,2,1,FALSE },
{10,"Cure3",		30,16,ELE_NON,FALSE,TRUE ,TRUE ,4,2,2,FALSE },
{11,"Cure4",		40,20,ELE_NON,FALSE,FALSE,FALSE,4,2,3,FALSE },
{12,"Cyclops",		50,62,ELE_EAR,FALSE,FALSE,FALSE,4,3,2,FALSE },
{13,"DarkHoly",		37,40,ELE_DAR,TRUE ,FALSE,FALSE,4,1,0,FALSE },
{14,"Fairy",		24,24,ELE_NON,FALSE,FALSE,FALSE,4,3,2,FALSE },
{15,"Fire",		14, 6,ELE_FIR,TRUE ,TRUE ,TRUE ,4,2,1,TRUE  },
{16,"Fire2",		18,12,ELE_FIR,TRUE ,TRUE ,TRUE ,4,2,2,TRUE  },
{17,"Fire3",		24,24,ELE_FIR,TRUE ,TRUE ,TRUE ,4,2,3,TRUE  },
{18,"Fire4",		32,48,ELE_FIR,FALSE,FALSE,FALSE,4,3,3,TRUE  },
{19,"Flare",		46,60,ELE_FIR,TRUE ,TRUE ,TRUE ,5,1,0,TRUE  },
{20,"Flare2",		38,35,ELE_FIR,FALSE,FALSE,FALSE,4,2,3,FALSE },
{21,"Holy",		50,56,ELE_HOL,TRUE ,TRUE ,TRUE ,5,1,0,FALSE },
{22,"Ice",		14, 6,ELE_ICE,TRUE ,TRUE ,TRUE ,4,2,1,TRUE  },
{23,"Ice2",		18,12,ELE_ICE,TRUE ,TRUE ,TRUE ,4,2,2,TRUE  },
{24,"Ice3",		24,24,ELE_ICE,TRUE ,TRUE ,TRUE ,4,2,3,TRUE  },
{25,"Ice4",		32,48,ELE_ICE,FALSE,FALSE,FALSE,4,3,3,TRUE  },
{26,"Ifrit",		24,24,ELE_FIR,FALSE,FALSE,FALSE,4,3,2,FALSE },
{27,"Leviathan",	38,48,ELE_WAT,FALSE,FALSE,FALSE,4,4,3,FALSE },
{28,"Melt",		38,70,ELE_FIR,TRUE ,FALSE,FALSE,4,4,3,FALSE },
{29,"Meteor",		60,70,ELE_FIR,TRUE ,FALSE,FALSE,4,4,3,FALSE },
{30,"Odin",		40,50,ELE_NON,FALSE,FALSE,FALSE,4,4,3,FALSE },
{31,"Quake",		39,70,ELE_EAR,TRUE ,FALSE,FALSE,4,4,3,FALSE },
{32,"Ramuh",		24,24,ELE_BOL,FALSE,FALSE,FALSE,4,3,2,FALSE },
{33,"Salamander",	38,48,ELE_FIR,FALSE,FALSE,FALSE,4,3,2,FALSE },
{34,"Shiva",		24,24,ELE_ICE,FALSE,FALSE,FALSE,4,3,2,FALSE },
{35,"Titan",		28,30,ELE_EAR,FALSE,FALSE,FALSE,4,3,2,FALSE },
{36,"Tornado",		37,70,ELE_WIN,TRUE ,FALSE,FALSE,4,4,3,FALSE },
{37,"Ultima",		23,10,ELE_NON,FALSE,FALSE,FALSE,4,2,1,FALSE },
{38,"Ultima2",		30,25,ELE_NON,FALSE,FALSE,FALSE,4,3,3,FALSE },
{39,"Zodiac",		96,99,ELE_NON,FALSE,FALSE,FALSE,4,4,3,FALSE },
{40,"All-Ultima",	55,50,ELE_NON,FALSE,FALSE,FALSE,5,4,3,FALSE },
{41,"NullSpell2",	0 ,0 ,ELE_NON,FALSE,FALSE,FALSE,0,0,0,FALSE }
};

/* item type list */
const struct item_type item_table[] = {
    {ITEM_LIGHT, "light"},
    {ITEM_SCROLL, "scroll"},
    {ITEM_WAND, "wand"},
    {ITEM_STAFF, "staff"},
    {ITEM_WEAPON, "weapon"},
    {ITEM_TREASURE, "treasure"},
    {ITEM_ARMOR, "armor"},
    {ITEM_POTION, "potion"},
    {ITEM_HEAL_FIXED, "heal_fixed"},
    {ITEM_CLOTHING, "clothing"},
    {ITEM_FURNITURE, "furniture"},
    {ITEM_TRASH, "trash"},
    {ITEM_CONTAINER, "container"},
    {ITEM_DRINK_CON, "drink"},
    {ITEM_KEY, "key"},
    {ITEM_FOOD, "food"},
    {ITEM_MONEY, "money"},
    {ITEM_BOAT, "boat"},
    {ITEM_CORPSE_NPC, "npc_corpse"},
    {ITEM_CORPSE_PC, "pc_corpse"},
    {ITEM_FOUNTAIN, "fountain"},
    {ITEM_PILL, "pill"},
    {ITEM_PROTECT, "protect"},
    {ITEM_MAP, "map"},
    {ITEM_PORTAL, "portal"},
    {ITEM_WARP_STONE, "warp_stone"},
    {ITEM_ROOM_KEY, "room_key"},
    {ITEM_GEM, "gem"},
    {ITEM_JEWELRY, "jewelry"},
    {ITEM_JUKEBOX, "jukebox"},
    {0, NULL}
};


/* weapon selection table */
const struct weapon_type weapon_table[] = {
    {"exotic", WEAPON_EXOTIC, 1,0},
    {"sword", WEAPON_SWORD, 1,0},
    {"dagger", WEAPON_DAGGER, 1,0},
    {"spear", WEAPON_SPEAR, 1,0},
    {"mace", WEAPON_MACE, 1,0},
    {"axe", WEAPON_AXE, 1,0},
    {"flail", WEAPON_FLAIL, 1,0},
    {"whip", WEAPON_WHIP, 1,0},
    {"ninjasword", WEAPON_NINJA_SWORD, 1,0},
    {"knightsword", WEAPON_KNIGHT_SWORD, 1,0},
    {"bag", WEAPON_BAG, 1,0},
    {"bow", WEAPON_BOW, 1,0},
    {"stick", WEAPON_STICK, 1,0},
    {"stave", WEAPON_STAVE, 1,0},
    {"polearm", WEAPON_GUN, 1,0},
    {"dictionary", WEAPON_DICT, 1,0},
    {"katana", WEAPON_KATANA, 1,0},
    {"instrument", WEAPON_INST, 1,0},
    {NULL, 0, 0, 0}
};


/* wiznet table and prototype for future flag setting */
const struct wiznet_type wiznet_table[] = {
    {"on", WIZ_ON, IM},
    {"prefix", WIZ_PREFIX, IM},
    {"ticks", WIZ_TICKS, IM},
    {"logins", WIZ_LOGINS, IM},
    {"sites", WIZ_SITES, L4},
    {"links", WIZ_LINKS, L7},
    {"newbies", WIZ_NEWBIE, IM},
    {"spam", WIZ_SPAM, L5},
    {"deaths", WIZ_DEATHS, IM},
    {"resets", WIZ_RESETS, L4},
    {"mobdeaths", WIZ_MOBDEATHS, L4},
    {"flags", WIZ_FLAGS, L5},
    {"penalties", WIZ_PENALTIES, L5},
    {"saccing", WIZ_SACCING, L5},
    {"levels", WIZ_LEVELS, IM},
    {"load", WIZ_LOAD, L2},
    {"restore", WIZ_RESTORE, L2},
    {"snoops", WIZ_SNOOPS, L2},
    {"switches", WIZ_SWITCHES, L2},
    {"secure", WIZ_SECURE, L1},
    {NULL, 0, 0}
};

/* attack table  -- not very organized :( */
const struct attack_type attack_table[MAX_DAMAGE_MESSAGE] = {
    {"none", "hit", -1},        /*  0 */
    {"stab", "stab", ELE_NON},
    {"chill", "chill", ELE_ICE},
    {NULL, NULL, 0}
};



/* race table */
const struct race_type race_table[] = {
/*
    {
    name,        pc_race?,
    act bits,    aff_by bits,    off bits,
    imm,        res,        vuln,
    form,        parts 
    },
*/
    {"unique", FALSE, 0, { 0, 0 }, 0, 0, 0, 0, 0, 0},

    {
     "human", TRUE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K },

    {
     "elf", TRUE,
     0, { 1024, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "dwarf", TRUE,
     0, { 1024, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "giant", TRUE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "bat", FALSE,
     0, { 0, 2080 }, OFF_DODGE | OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | P},

    {
     "bear", FALSE,
     0, { 0, 0 }, OFF_CRUSH | OFF_DISARM | OFF_BERSERK,
     0, 0, 0,
     A | G | V, A | B | C | D | E | F | H | J | K | U | V},

    {
     "cat", FALSE,
     0, { 0, 2048 }, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | Q | U | V},

    {
     "centipede", FALSE,
     0, { 0, 2048 }, 0,
	     0, 0, 0,
     A | B | G | O, A | C | K},

    {
     "dog", FALSE,
     0, { 0, 0 }, OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | U | V},

    {
     "doll", FALSE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     E | J | M | cc, A | B | C | G | H | K},

    {
     "dragon", FALSE,
     0, { 1024, 32 }, 0,
     0, 0, 0,
     A | H | Z, A | C | D | E | F | G | H | I | J | K | P | Q | U | V | X},

    {
     "fido", FALSE,
     0, { 0, 0 }, OFF_DODGE | ASSIST_RACE,
     0, 0, 0,
     A | B | G | V, A | C | D | E | F | H | J | K | Q | V},

    {
     "fox", FALSE,
     0, { 0, 2048 }, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K | Q | V},

    {
     "goblin", FALSE,
     0, { 1024, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "hobgoblin", FALSE,
     0, { 1024, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K | Y},

    {
     "kobold", FALSE,
     0, { 1024, 0 }, 0,
     0, 0, 0,
     A | B | H | M | V, A | B | C | D | E | F | G | H | I | J | K | Q},

    {
     "lizard", FALSE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     A | G | X | cc, A | C | D | E | F | H | K | Q | V},

    {
     "modron", FALSE,
     0, { 1024, 0 }, ASSIST_RACE | ASSIST_ALIGN,
     0, 0, 0,
     H, A | B | C | G | H | J | K},

    {
     "orc", FALSE,
     0, { 1024, 0 }, 0,
     0, 0, 0,
     A | H | M | V, A | B | C | D | E | F | G | H | I | J | K},

    {
     "pig", FALSE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K},

    {
     "rabbit", FALSE,
     0, { 0, 0 }, OFF_DODGE | OFF_FAST,
     0, 0, 0,
     A | G | V, A | C | D | E | F | H | J | K},

    {
     "school monster", FALSE,
     ACT_NOALIGN, { 0, 0 }, 0,
     0, 0, 0,
     A | M | V, A | B | C | D | E | F | H | J | K | Q | U},

    {
     "snake", FALSE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     A | G | X | Y | cc, A | D | E | F | K | L | Q | V | X},

    {
     "song bird", FALSE,
     0, { 0, 32 }, OFF_FAST | OFF_DODGE,
     0, 0, 0,
     A | G | W, A | C | D | E | F | H | K | P},

    {
     "troll", FALSE,
     0, { 1088, 16384 }, OFF_BERSERK,
     0, 0, 0,
     A | B | H | M | V, A | B | C | D | E | F | G | H | I | J | K | U | V},

    {
     "water fowl", FALSE,
     0, { 0, 9216 }, 0,
     0, 0, 0,
     A | G | W, A | C | D | E | F | H | K | P},

    {
     "wolf", FALSE,
     0, { 0, 2048 }, 
     0, 0, 0,
     A | G | V, A | C | D | E | F | J | K | Q | V},

    {
     "wyvern", FALSE,
     0, { 80, 1024 },
     0,0,0,
     A | B | G | Z, A | C | D | E | F | H | J | K | Q | V | X},

    {
     "unique", FALSE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     0, 0},


    {
     NULL, FALSE,
     0, { 0, 0 }, 0,
     0, 0, 0,
     0, 0 },
};

const struct pc_race_type pc_race_table[] = {
    {"null race", "", 0, {100, 100, 100, 100},
     {"",0}, {13, 13, 13, 13, 13}, {18, 18, 18, 18, 18}, 0},

/*
    {
    "race name",     short name,     points,    { class multipliers },
    { bonus skills },
    { base stats },        { max stats },        size 
    },
*/
    {
     "human", "Human", 0, {100, 100, 100, 100},
     {"",0},
     {1, 1, 1, 1, 1}, {99, 99, 99, 99, 99}, SIZE_MEDIUM},

    {
     "elf", " Elf ", 5, {100, 125, 100, 120},
     {"sneak", "hide"},
     {12, 14, 13, 15, 11}, {16, 20, 18, 21, 15}, SIZE_SMALL},

    {
     "dwarf", "Dwarf", 8, {150, 100, 125, 100},
     {"berserk"},
     {14, 12, 14, 10, 15}, {20, 16, 19, 14, 21}, SIZE_MEDIUM},

    {
     "giant", "Giant", 6, {200, 150, 150, 105},
     {"bash", "fast healing"},
     {16, 11, 13, 11, 14}, {22, 15, 18, 15, 20}, SIZE_LARGE}
};





/*
 * Attribute bonus tables.
 */
const struct str_app_type str_app[26] = {
    {-5, -4, 0, 0},                /* 0  */
    {-5, -4, 3, 1},                /* 1  */
    {-3, -2, 3, 2},
    {-3, -1, 10, 3},            /* 3  */
    {-2, -1, 25, 4},
    {-2, -1, 55, 5},            /* 5  */
    {-1, 0, 80, 6},
    {-1, 0, 90, 7},
    {0, 0, 100, 8},
    {0, 0, 100, 9},
    {0, 0, 115, 10},            /* 10  */
    {0, 0, 115, 11},
    {0, 0, 130, 12},
    {0, 0, 130, 13},            /* 13  */
    {0, 1, 140, 14},
    {1, 1, 150, 15},            /* 15  */
    {1, 2, 165, 16},
    {2, 3, 180, 22},
    {2, 3, 200, 25},            /* 18  */
    {3, 4, 225, 30},
    {3, 5, 250, 35},            /* 20  */
    {4, 6, 300, 40},
    {4, 6, 350, 45},
    {5, 7, 400, 50},
    {5, 8, 450, 55},
    {6, 9, 500, 60}                /* 25   */
};



const struct int_app_type int_app[26] = {
    {3},                        /*  0 */
    {5},                        /*  1 */
    {7},
    {8},                        /*  3 */
    {9},
    {10},                        /*  5 */
    {11},
    {12},
    {13},
    {15},
    {17},                        /* 10 */
    {19},
    {22},
    {25},
    {28},
    {31},                        /* 15 */
    {34},
    {37},
    {40},                        /* 18 */
    {44},
    {49},                        /* 20 */
    {55},
    {60},
    {70},
    {80},
    {85}                        /* 25 */
};



const struct wis_app_type wis_app[26] = {
    {0},                        /*  0 */
    {0},                        /*  1 */
    {0},
    {0},                        /*  3 */
    {0},
    {1},                        /*  5 */
    {1},
    {1},
    {1},
    {1},
    {1},                        /* 10 */
    {1},
    {1},
    {1},
    {1},
    {2},                        /* 15 */
    {2},
    {2},
    {3},                        /* 18 */
    {3},
    {3},                        /* 20 */
    {3},
    {4},
    {4},
    {4},
    {5}                            /* 25 */
};



const struct dex_app_type dex_app[26] = {
    {60},                        /* 0 */
    {50},                        /* 1 */
    {50},
    {40},
    {30},
    {20},                        /* 5 */
    {10},
    {0},
    {0},
    {0},
    {0},                        /* 10 */
    {0},
    {0},
    {0},
    {0},
    {-10},                        /* 15 */
    {-15},
    {-20},
    {-30},
    {-40},
    {-50},                        /* 20 */
    {-60},
    {-75},
    {-90},
    {-105},
    {-120}                        /* 25 */
};


const struct con_app_type con_app[26] = {
    {-4, 20},                    /*  0 */
    {-3, 25},                    /*  1 */
    {-2, 30},
    {-2, 35},                    /*  3 */
    {-1, 40},
    {-1, 45},                    /*  5 */
    {-1, 50},
    {0, 55},
    {0, 60},
    {0, 65},
    {0, 70},                    /* 10 */
    {0, 75},
    {0, 80},
    {0, 85},
    {0, 88},
    {1, 90},                    /* 15 */
    {2, 95},
    {2, 97},
    {3, 99},                    /* 18 */
    {3, 99},
    {4, 99},                    /* 20 */
    {4, 99},
    {5, 99},
    {6, 99},
    {7, 99},
    {8, 99}                        /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const struct liq_type liq_table[] = {
/*    name            color    proof, full, thirst, food, ssize */
    {"water", "clear", {0, 1, 10, 0, 16}},
    {"beer", "amber", {12, 1, 8, 1, 12}},
    {"red wine", "burgundy", {30, 1, 8, 1, 5}},
    {"ale", "brown", {15, 1, 8, 1, 12}},
    {"dark ale", "dark", {16, 1, 8, 1, 12}},

    {"whisky", "golden", {120, 1, 5, 0, 2}},
    {"lemonade", "pink", {0, 1, 9, 2, 12}},
    {"firebreather", "boiling", {190, 0, 4, 0, 2}},
    {"local specialty", "clear", {151, 1, 3, 0, 2}},
    {"slime mold juice", "green", {0, 2, -8, 1, 2}},

    {"milk", "white", {0, 2, 9, 3, 12}},
    {"tea", "tan", {0, 1, 8, 0, 6}},
    {"coffee", "black", {0, 1, 8, 0, 6}},
    {"blood", "red", {0, 2, -1, 2, 6}},
    {"salt water", "clear", {0, 1, -2, 0, 1}},

    {"coke", "brown", {0, 2, 9, 2, 12}},
    {"root beer", "brown", {0, 2, 9, 2, 12}},
    {"elvish wine", "green", {35, 2, 8, 1, 5}},
    {"white wine", "golden", {28, 1, 8, 1, 5}},
    {"champagne", "golden", {32, 1, 8, 1, 5}},

    {"mead", "honey-colored", {34, 2, 8, 2, 12}},
    {"rose wine", "pink", {26, 1, 8, 1, 5}},
    {"benedictine wine", "burgundy", {40, 1, 8, 1, 5}},
    {"vodka", "clear", {130, 1, 5, 0, 2}},
    {"cranberry juice", "red", {0, 1, 9, 2, 12}},

    {"orange juice", "orange", {0, 2, 9, 3, 12}},
    {"absinthe", "green", {200, 1, 4, 0, 2}},
    {"brandy", "golden", {80, 1, 5, 0, 4}},
    {"aquavit", "clear", {140, 1, 5, 0, 2}},
    {"schnapps", "clear", {90, 1, 5, 0, 2}},

    {"icewine", "purple", {50, 2, 6, 1, 5}},
    {"amontillado", "burgundy", {35, 2, 8, 1, 5}},
    {"sherry", "red", {38, 2, 7, 1, 5}},
    {"framboise", "red", {50, 1, 7, 1, 5}},
    {"rum", "amber", {151, 1, 4, 0, 2}},

    {"cordial", "clear", {100, 1, 5, 0, 2}},
    {NULL, NULL, {0, 0, 0, 0, 0}}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)    n

const struct skill_type skill_table[MAX_SKILL] = {

/*
 * Magic spells.
 */

    {
     " --- ", 
     0, TAR_IGNORE, POS_STANDING,
     NULL, SLOT (0), 0, 0,
     "", "", "", "", "",0,200,-1,"This skill is a bug."},

    {
   
     "armor", 
     spell_armor, TAR_CHAR_DEFENSIVE, POS_STANDING,
     NULL, SLOT (1), 5, 12,
     "", "You feel less armored.", "", "", "",0,200,-1,"No description"},

    {
     "bless", 
     spell_bless, TAR_OBJ_CHAR_DEF, POS_STANDING,
     NULL, SLOT (3), 5, 12,
     "", "You feel less righteous.",
     "$p's holy aura fades.", "", "",0,200,-1,"No description"},

    {
     "Throw Stone", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_throw_stone, SLOT (0), 0, 0,
     "rock", "!throw stone!", "", "", "",SKILL_JOB,200,0,"Throw a stone"},

/* FAITH BASED */




    {
     "NullSpell",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s00, 0, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Annoys The Coders Endlessly"},

    {
     "Bahamut",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s01, 1, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Lots of Fire damage"},

    {
     "Bio",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s02, 2, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Poisons"},

    {
     "Bio3",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s03, 3, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Poisons/AA"},

    {
     "Bolt",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s04, 4, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"LIT damage"},

    {
     "Bolt2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s05, 5, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"More LIT damage"},

    {
     "Bolt3",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s06, 6, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Even more LIT damage"},

    {
     "Bolt4",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s07, 7, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Huge loads of LIT damage"},

    {
     "Cure",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s08, 8, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_PRIEST,"Restores HP"},

    {
     "Cure2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s09, 9, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_PRIEST,"Restores more HP"},

    {
     "Cure3",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s10, 10, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_PRIEST,"Restores even more HP"},

    {
     "Cure4",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s11, 11, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_PRIEST,"Restores tons of HP"},

    {
     "Cyclops",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s12, 12, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for earth damage"},

    {
     "DarkHoly",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s13, 13, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"High level DRK damage"},

    {
     "Fairy",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s14, 14, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for wind damage"},

    {
     "Fire",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s15, 15, 20, 12,
     "", "!DeadSpellBug!", "", "", "Hot fire, burn like something really, really hot!",SKILL_JOB,200,JOB_WIZARD,"Fire damage"},

    {
     "Fire2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s16, 16, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"More fire damage"},

    {
     "Fire3",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s17, 17, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Loads of fire damage"},

    {
     "Fire4",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s18, 18, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Destroys small villages"},

    {
     "Flare",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s19, 19, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Obscene fire damage"},

    {
     "Flare2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s20, 20, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Kills people dead/fire damage"},

    {
     "Holy",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s21, 21, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_PRIEST,"Lots of damage"},

    {
     "Ice",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s22, 22, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Ice damage"},

    {
     "Ice2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s23, 23, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"More Ice damage"},

    {
     "Ice3",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s24, 24, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Even more Ice damage"},

    {
     "Ice4",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s25, 25, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Big loads of ice damage"},

    {
     "Ifrit",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s26, 26, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for fire damage"},

    {
     "Leviathan",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s27, 27, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for water damage"},

    {
     "Melt",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s28, 28, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Overheat CPU for fire damage"},

    {
     "Meteor",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s29, 29, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_TIMEMAGE,"Falls down and kills people"},

 {
     "Odin",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s30, 30, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for Non-element dmg"},
 {
     "Quake",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s31, 31, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_WIZARD,"Strong earth element spell"},
 {
     "Ramuh",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s32, 32, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for Lightning dmg"},
 {
     "Salamander",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s33, 33, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for even more fire damage"},
 {
     "Shiva",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s34, 34, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for Ice damage"},
 {
     "Titan",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s35, 35, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Summon for earth damage"},
 {
     "Tornado",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s36, 36, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"High level Wind attack"},
 {
     "Ultima",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s37, 37, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Ultimate Attack spell"},
 {
     "Ultima2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s38, 38, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Even more Ultimate attack spell"},
 {
     "Zodiac",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s39, 39, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,JOB_SUMMONER,"Massive non-element summon"},
 {
     "All-Ultima",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s40, 40, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Even more ultimate attack spell (heh)"},
 {
     "NullSpell2",
     spell_fspell, TAR_CHAR_OFFENSIVE, POS_STANDING,
     &gsn_s41, 41, 20, 12,
     "", "!DeadSpellBug!", "", "", "",SKILL_JOB,200,-1,"Squid!"},





/* END FAITH BASED*/


 {
     "Counter", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_counter, SLOT (0), 0, 0,
     "rock", "!counter!", "", "", "",SKILL_REACT,400,-1,"No description"
 },
 {
     "Hamedo", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_hamedo, SLOT (0), 0, 0,
     "rock", "!hamedo!", "", "", "",SKILL_REACT,800,-1,"No description"
 },
 {
     "Counter Tackle", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_counter_tackle, SLOT (0), 0, 0,
     "rock", "!countertackle!", "", "", "",SKILL_REACT,100,0,"Counter with Tackle"
 },
 {
     "Move+1", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_move_plus_1, SLOT (0), 0, 0,
     "rock", "!Move+1!", "", "", "",SKILL_MOVE,200,0,"Movement Range + 1"
 },
 {
     "Move+2", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_move_plus_2, SLOT (0), 0, 0,
     "rock", "!Move+2!", "", "", "",SKILL_MOVE,400,-1,"Movement Range + 2"
 },
 {
     "Move+3", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_move_plus_3, SLOT (0), 0, 0,
     "rock", "!Move+3!", "", "", "",SKILL_MOVE,600,-1,"Movement Range + 3"
 }, 


    {
     "backstab",
     spell_null, TAR_IGNORE, POS_STANDING,
     &gsn_backstab, SLOT (0), 0, 24,
     "backstab", "!Backstab!", "", "", "",0,200,-1,"No description"
     },

    {
     "bash",
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_bash, SLOT (0), 0, 24,
     "bash", "!Bash!", "", "", "",0,200,-1,"No description"},

    {
     "dodge", 
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_dodge, SLOT (0), 0, 0,
     "", "!Dodge!", "", "", "",0,200,-1,"No description"},

    {
     "hand to hand",
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_hand_to_hand, SLOT (0), 0, 0,
     "", "!Hand to Hand!", "", "", "",0,200,-1,"No description"},

    {
     "kick", 
     spell_null, TAR_CHAR_OFFENSIVE, POS_FIGHTING,
     &gsn_kick, SLOT (0), 0, 12,
     "kick", "!Kick!", "", "", "",0,200,-1,"No description"},

    {
     "parry",
     spell_null, TAR_IGNORE, POS_FIGHTING,
     &gsn_parry, SLOT (0), 0, 0,
     "", "!Parry!", "", "", "",0,200,-1,"No description"},

};

const struct group_type group_table[MAX_GROUP] = {

    {
     "rom basics",
     {"scrolls", "staves", "wands", "recall"}
     }



};
