#include "include.h"

char * female_names[256] = {
"Adria",
"Agnes",
"Aimee",
"Alice",
"Alyssa",
"Amanda",
"Amy",
"Angelica",
"Anita",
"Anna",
"Annette",
"Aqua",
"Aquarius",
"Ariel",
"Aska",
"Audrey",
"Aurora",
"Barbara",
"Barnet",
"Barsia",
"Beatrice",
"Beauty",
"Beauty",
"Belinda",
"Bridgette",
"Candy",
"Carmen",
"Carol",
"Caroline",
"Cary",
"Casia",
"Casie",
"Celesh",
"Celfina",
"Celine",
"Chanel",
"Charene",
"Charmaine",
"Cherry",
"Chris",
"Christie",
"Cinderella",
"Cindy",
"Clara",
"Claris",
"Claudia",
"Claudia",
"Comet",
"Connie",
"Constance",
"Cookie",
"Corey",
"Cornelia",
"Cosmos",
"Cynthia",
"Daisy",
"Debra",
"Demi",
"Diana",
"Diana",
"Dionne",
"Dominique",
"Donna",
"Doreth",
"Doris",
"Dorone",
"Dorothy",
"Eleana",
"Eliza",
"Elizabeth",
"Ellen",
"Ellis",
"Eltiana",
"Elza",
"Emerald",
"Emily",
"Eri",
"Esmeralda",
"Eva",
"Eve",
"Fawn",
"Fay",
"Fianna",
"Francis",
"Frannie",
"Freesia",
"Garnett",
"Gena",
"Ginger",
"Glenda",
"Gloria",
"Grace",
"Hannah",
"Helen",
"Hilda",
"Holly",
"Ilean",
"Ilia",
"Ivana",
"Ivory",
"Izabella",
"Jacky",
"Jane",
"Janet",
"Jasmine",
"Jeanne",
"Jenique",
"Jennie",
"Jennifer",
"Jenny",
"Jessica",
"Jill",
"Joanna",
"Joanne",
"Jobeth",
"Jody",
"Jolene",
"Josephine",
"Judy",
"Julia",
"Juliette",
"June",
"Karen",
"Katarin",
"Kate",
"Katherine",
"Kay",
"Kitty",
"Koko",
"Kyoko",
"Kyomi",
"Laura",
"Linda",
"Lisa",
"Liza",
"Lucy",
"Luna",
"Madeline",
"Mandy",
"Margaret",
"Margarita",
"Margo",
"Maria",
"Marilyn",
"Marlene",
"Mary",
"Mary Ann",
"Matilda",
"May",
"Meera",
"Melanie",
"Melissa",
"Michelle",
"Mildred",
"Minerva",
"Miranda",
"Mireiyu",
"Miria",
"Misty",
"Mo",
"Monica",
"Nancy",
"Nastasia",
"Natalie",
"Nena",
"Nena",
"Nikita",
"Nora",
"Olive",
"Opal",
"Ophelia",
"Orange",
"Pamela",
"Papaya",
"Partha",
"Patricia",
"Patsy",
"Paula",
"Pawnia",
"Peline",
"Penelope",
"Peppermint",
"Quinn",
"Rachel",
"Ramia",
"Ramuda",
"Rebecca",
"Reda",
"Ripley",
"Rosa",
"Rose",
"Sabatieni",
"Sabrina",
"Sachiko",
"Sade",
"Sage",
"Saki",
"Sally",
"Salome",
"Samantha",
"Sandra",
"Sandy",
"Sapphire",
"Sara",
"Sara",
"Sasha",
"Scarlet",
"Selena",
"Seth",
"Sevon",
"Shadi",
"Shari",
"Sharon",
"Shawn",
"Sheema",
"Sheila",
"Sheryl",
"Shields",
"Silky",
"Silphy",
"Simone",
"Simonne",
"Sissie",
"Sophia",
"Stella",
"Stephanie",
"Strawberry",
"Susan",
"Susie",
"Suzanne",
"Swan",
"Sylia",
"Sylvia",
"Tabatha",
"Tara",
"Tess",
"Tess",
"Theresa",
"Tia",
"Tiara",
"Tina",
"Tirala",
"Tracy",
"Valerie",
"Vanessa",
"Vanilla",
"Venus",
"Vera",
"Veronica",
"Veronica",
"Vicky",
"Victoria",
"Violet",
"Wendy",
"Yumiko",
"Zora" };

char * monster_names[256] = {
"Adonis",
"Ageia",
"Aigiparn",
"Aigiss",
"Aiguptos",
"Aiorides",
"Aisacos",
"Aiterra",
"Aitone",
"Aitorn",
"Akaia",
"Akakos",
"Algo",
"Alkoun",
"Alsnova",
"Amperos",
"Analieze",
"Anios",
"Antigone",
"Apemorsh",
"Arcadia",
"Arches",
"Asjarabos",
"Atropos",
"Ausorn",
"Bakas",
"Bakis",
"Barok",
"Belekyuntei",
"Beros",
"Boine",
"Bonas",
"Brontes",
"Cemerei",
"Ciburei",
"Ciden",
"Cikorn",
"Cilwanna",
"Cinope",
"Ciumei",
"Cleovis",
"Dader",
"Daimoth",
"Damiar",
"Danaei",
"Dardanos",
"Dark Wind",
"Deashria",
"Deimacos",
"Delpoi",
"Derkunos",
"Diars",
"Dies",
"Dorna",
"Dropeer",
"Echo",
"Eidotea",
"Eipionne",
"Eketos",
"Ellebos",
"Elnopell",
"Eniepeu",
"Epigoni",
"Erator",
"Eratoss",
"Euadonne",
"Euanos",
"Euekenor",
"Eunmoss",
"Euparamo",
"Euporupo",
"Europpe",
"Famers",
"Fauna",
"Foashpil",
"Fobos",
"Galeo",
"Garateia",
"Gedros",
"Gigas",
"Gryupus",
"Guares",
"Guraukei",
"Hariara",
"Helcuna",
"Heppoko",
"Heriars",
"Herios",
"Hien",
"Hiportes",
"Hmiear",
"Hopradarmos",
"Huzares",
"Iamoss",
"Ianttee",
"Iapis",
"Illusion ",
"Inferry",
"Inoss",
"Ion",
"Iopee",
"Iros",
"Irose",
"Isa",
"Iskys",
"Itess",
"Kamerus",
"Kampe",
"Kanous",
"Kapus",
"Karis",
"Karna",
"Kartils",
"Kaukorn",
"Kayes",
"Kepalos",
"Kermis",
"Kiriks",
"Kranaos",
"Kraros",
"Kryusa",
"Latinos",
"Latner",
"Lightning bolt",
"Lios",
"Maiandos",
"Makariar",
"Makaron",
"Marls",
"Marwars",
"Meatis",
"Megareus",
"Mempis",
"Menesteus",
"Merambors",
"Meraneus",
"Merops",
"Mestorle",
"Metaneila",
"Metorpe",
"Minors",
"Miserikordi",
"Mnuas",
"Mornes",
"Morossos",
"Morpeus",
"Muilto",
"Muknaiar",
"Murine",
"Murkbell",
"Musa",
"Naucitos",
"Nauteis",
"Neaira",
"Neiros",
"Nemea",
"Nessos",
"Nieless",
"Nomios",
"Nuks",
"Ogappon",
"Ogyugos",
"Oiaks",
"Oibaros",
"Oksyuros",
"Omega",
"Ops",
"Ortia",
"Paneas",
"Paris",
"Pasaria",
"Pedasos",
"Peirene",
"Penartes",
"Peniar",
"Pentecireia",
"Peraiar",
"Perias",
"Peripanos",
"Perseas",
"Persepone",
"Pesos",
"Petes",
"Pierria",
"Pitane",
"Porksor",
"Poteitei",
"Pracios",
"Preglar",
"Priamos",
"Priksos",
"Prmios",
"Prokner",
"Prokris",
"Proteus",
"Proteus",
"Protores",
"Prubos",
"Prudamuna",
"Prular",
"Pryukkaon",
"Ptnos",
"Queberay",
"Quejikos",
"Quenos",
"Radinea",
"Radmanteis",
"Rampetos",
"Raodameia",
"Raodkei",
"Rapites",
"Rauss",
"Rawelna",
"Regolio",
"Reia",
"Reimonee",
"Reucorn",
"Reukei",
"Ribentina",
"Ribukei",
"Rinos",
"Ritersete",
"Rukeios",
"Ruklegos",
"Rukmedes",
"Rukoprone",
"Runkoss",
"Rurcipper",
"Saramis",
"Sarios",
"Satiros",
"Seshposs",
"Shibaris",
"Snow Wind",
"Sol",
"Sword Dance",
"Tamyuriss",
"Tantars",
"Taraous",
"Tborne",
"Teia",
"Telepassa",
"Tenes",
"Teopanei",
"Terkcion",
"Tespei",
"Teteus",
"Toars",
"Tucross",
"Tukee",
"Urans",
"Ustitia",
"Westa",
"World",
"Yarnes",
"Yupitell" };

char * male_names[256] = {
"Abel",
"Abraham",
"Akintunde",
"Albert",
"Alberto",
"Alexei",
"Anderson",
"Andy",
"Anton",
"Arnold",
"Badum",
"Baker",
"Barney",
"Barton",
"Baskirk",
"Bean",
"Beck",
"Benedict",
"Berkeley",
"Bernar",
"Bernard",
"Blackbarn",
"Blake",
"Boville",
"Boyce",
"Boyle",
"Brandol",
"Brett",
"Brian",
"Bruce",
"Bruno",
"Bugsy",
"Buster",
"Cain",
"Carlson",
"Carmine",
"Casim",
"Cedric",
"Chamberlain",
"Chang",
"Chester",
"Chips",
"Clayton",
"Clive",
"Coleman",
"Collins",
"Crawford",
"Curtis",
"Daison",
"Dallas",
"Daniel",
"David",
"David",
"Davis",
"Dawson",
"Deacon",
"Dennis",
"Deverca",
"Diaz",
"Dick",
"Dietrich",
"Dillon",
"Dog",
"Donald",
"Donovan",
"Dorma",
"Drake",
"Draper",
"Dreyfuss",
"Eckart",
"Eddie",
"Edgar",
"Eliot",
"Eric",
"Evans",
"Feldman",
"Fernando",
"Fink",
"Fisher",
"Flansburgh",
"Fordham",
"Forward",
"Fox",
"Fred",
"Freddie",
"Gailar",
"Garcia",
"Gaston",
"Geist",
"Gelhardt",
"Gerald",
"Gilliam",
"Glen",
"Goodman",
"Gordon",
"Gordon",
"Greg",
"Gwynn",
"Hamilton",
"Hampsten",
"Hannibal",
"Harold",
"Harry",
"Heinline",
"Hendricksen",
"Hicks",
"Hogan",
"Hoisler",
"Hugo",
"Hugo",
"Humphrey",
"Ian",
"Indurain",
"Indy",
"Ingram",
"Ironside",
"J.I.",
"Jared",
"Jean",
"Jeremy",
"Jorge",
"Judd",
"Junk",
"Kaplan",
"Keane",
"Keith",
"Kelly",
"Kenji",
"Kimball",
"Kinison",
"Knox",
"Lars",
"Leon",
"Leonard",
"Leopold",
"Linus",
"Little Jon",
"Lloyd",
"Loggins",
"Lowell",
"Luke",
"Malcolm",
"Mancel",
"Marlow",
"Marty",
"Masahiro",
"Matsuo",
"Maverick",
"Mayfield",
"McCloud",
"McGyle",
"McNeal",
"McNorton",
"McPherson",
"Merlin",
"Montana",
"Morris",
"Morrison",
"Mueller",
"Musashi",
"Nash",
"Nathan",
"Navarro",
"Neal",
"Neilson",
"Nelson",
"Nichol",
"Nielson",
"Nigel",
"Noah",
"Noppo",
"Norris",
"Norton",
"O'Bannon",
"O'Neil",
"Orwell",
"Otto",
"Pado",
"Parker",
"Pete",
"Philo",
"Pierre",
"Ponapalt",
"Radcliff",
"Ralph",
"Rampart",
"Randolf",
"Randy",
"Rat",
"Ravel",
"Redford",
"Reeves",
"Rico",
"Rienhart",
"Rivaldi",
"Robbins",
"Roberto",
"Roland",
"Rush",
"Samson",
"Samuel",
"Sanders",
"Santos",
"Schenberg",
"Scott",
"Sean",
"Selleck",
"Serge",
"Shaker",
"Singleton",
"Slader",
"Soetheby",
"Sorvez",
"Southerland",
"Spencer",
"Spider",
"Spike",
"Stanley",
"Stone",
"Striker",
"Sullivan",
"Swift",
"Tak",
"Tatoolo",
"Taylor",
"Ted",
"Tensberger",
"Thompson",
"Tim",
"Tuckert",
"Tyler",
"Tymothi",
"Valentine",
"Vang",
"Vashkoda",
"Vector",
"Velger",
"Verner",
"Victor",
"Vincent",
"Vincent",
"Voltaire",
"Walter",
"Weazel",
"Whitman",
"Willy",
"Wineberg",
"Winter",
"Woody",
"Wyatt",
"Yeager",
"Zack",
"Zeke",
"Zelig",
"Zemekis",
"Ziggy" };