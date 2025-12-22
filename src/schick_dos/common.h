#if !defined(M302de_COMMON_H)
#define M302de_COMMON_H

enum {
	NORTH	= 0,
	EAST	= 1,
	SOUTH	= 2,
	WEST	= 3
};

/* viewing directions in fight mode, according to keyboard input in fight,
 * and also according to the chessboard x- and y-coordinates,
 * i.e. positive x-direction is "right", positive y-direction is "up" etc.
 */
enum {
	FIG_VIEWDIR_RIGHT = 0,
	FIG_VIEWDIR_DOWN  = 1,
	FIG_VIEWDIR_LEFT  = 2,
	FIG_VIEWDIR_UP    = 3
};

enum {
	SEASON_WINTER = 0,
	SEASON_SPRING = 1,
	SEASON_SUMMER = 2,
	SEASON_AUTUMN = 3
};

enum {
	ACTION_ID_ESC = 1,
	ACTION_ID_CLOSING_SQUARE_BRACKET = 27, /* the ']' key. used in buy_screen() at a merchant to decrease the number of items. */
	ACTION_ID_RETURN = 28,
	ACTION_ID_J = 44, /* the 'J' key. used in seg097.cpp in GUI_menu_input (when called from GUI_bool()). selects the "Ja" (yes) answer. */
	ACTION_ID_N = 49, /* the 'N' key. used in seg097.cpp in GUI_menu_input (when called from GUI_bool()). selects the "Nein" (no) answer. */
	ACTION_ID_SLASH = 53, // the '/' key. used in buy_screen() at a merchant to increase the number of items.
	ACTION_ID_SPACE = 57,
	ACTION_ID_UP = 72,
	ACTION_ID_PAGE_UP = 73, /* opens the menu */
	ACTION_ID_LEFT = 75,
	ACTION_ID_RIGHT = 77,
	ACTION_ID_DOWN = 80,
	ACTION_ID_PAGE_DOWN = 81, /* closes the menu */
	/* in seg048.cpp there is "if (ds_readws(ACTION) >= 128 && ds_readws(ACTION) <= 152) [...]".
	 * I think these 25 numbers are used for the icons in the character screen, probably in the order
	 * head, arms, body, right hand, left hand, legs, feet, knapsack1 .. knapsack16; eye, mouth */
	ACTION_ID_ICON_1 = 129,
	ACTION_ID_ICON_2 = 130,
	ACTION_ID_ICON_3 = 131,
	ACTION_ID_ICON_4 = 132,
	ACTION_ID_ICON_5 = 133,
	ACTION_ID_ICON_6 = 134,
	ACTION_ID_ICON_7 = 135,
	ACTION_ID_ICON_8 = 136,
	ACTION_ID_ICON_9 = 137,
	ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK = 144, /* set in buy_screen() at a merchant if an item is right-clicked to decrease the count. */
	ACTION_ID_240 = 240, /* what does it mean? used in status_menu() in seg048.cpp */
	/* In seg057.cpp and seg058.cpp there is "if (ds_readws(ACTION) >= 241 && ds_readws(ACTION) <= 247) [...]" */
	ACTION_ID_VOID = 999
};

/* 26 ingame timers for (in this order) active miracles (6), unavailable talent usages (2), active spells (2) and unavailable miracles (16). */
enum {
	INGAME_TIMER_RONDRA_NO_SPELLS = 0, /* Rondra miracle active: block spells (6 hours) */
	INGAME_TIMER_EFFERD_FIND_WATER = 1, /* Efferd miracle active: search for water in wildcamp will always be successful (3 days) */
	INGAME_TIMER_EFFERD_SAFE_PASSAGE = 2, /* Efferd miracle active: no incidences on sea traveling (3 days) */
	INGAME_TIMER_FIRUN_HUNT = 3, /* Firun miracle active: hunting in wildcamps will always be successful (3 or 7 days) */
	INGAME_TIMER_TRAVIA_SAFE_REST = 4, /* Travia miracle active: safety during the night (no camp attacks during sleep) active (7 days) */
	INGAME_TIMER_BORON_UNDEAD = 5, /* Boron miracle active: protection from undead (but apparently, does not have an impact anywhere...) (3 days) */
	INGAME_TIMER_AKROBATIK = 6, /* talent usage unavailable: Akrobatik (8 hours) */
	INGAME_TIMER_MUSIZIEREN = 7, /* talent usage unavailable: Musizieren (8 hours) */
	INGAME_TIMER_FLIM_FLAM = 8, /* spell 'Flim Flam' is active */
	INGAME_TIMER_DARKNESS = 9, /* spell 'Dunkelheit' is active */
	INGAME_TIMER_PRAIOS_MU = 10, /* Praios miracle unavailable: MU +1 (1 day) */
	INGAME_TIMER_PRAIOS_MR = 11, /* Praios miracle unavailable: MR +99 (3 days) */
	INGAME_TIMER_RONDRA_SWORDS = 12, /* Rondra miracle unavailable: Schwerter +1, AT + 1 (3 days) */
	INGAME_TIMER_RONDRA_MAGIC_WEAPON = 13, /* Rondra miracle unavailable: make weapon magic (1 day) */
	INGAME_TIMER_EFFERD_SWIM = 14, /* Efferd miracle unavailable: Schwimmen +2 (4 days) */
	INGAME_TIMER_BORON_TA = 15, /* Boron miracle unavailable: TA -1 (4 days) */
	INGAME_TIMER_HESINDE_ANALUES = 16, /* Hesinde miracle unavailable: 'Analues Arkanstruktur +1' (5 days) */
	INGAME_TIMER_HESINDE_MR = 17, /* Hesinde miracle unavailable: MR +5 (4 days) */
	INGAME_TIMER_FIRUN_SATED = 18, /* Firun miracle unavailable: no hunger or thirst (7 days) */
	INGAME_TIMER_PHEX_THIEF = 19, /* Phex miracle unavailable: talents Schleichen +1, Taschendiebstahl +1 (3 days) */
	INGAME_TIMER_PHEX_FEILSCHEN = 20, /* Phex miracle unavailable: Feilschen +1 (3 days) */
	INGAME_TIMER_PHEX_FF = 21, /* Phex miracle unavailable: FF +1 (3 days) */
	INGAME_TIMER_INGERIMM_MAGIC_WEAPON = 22, /* Ingerimm miracle unavailable: make weapon magic (1 day) */
	INGAME_TIMER_RAHJA_TALENTS = 23, /* Rahja miracle unavailable: Tanzen +1, Betoeren +1 (7 days) */
	INGAME_TIMER_RAHJA_CH = 24, /* Rahja miracle unavailable: CH +1 (3 days) */
	INGAME_TIMER_RAHJA_TALENTS_PERMANENT = 25 /* Rahja miracle unavailable: Tanzen +1 (permanent), Betoeren +1 (permanent) (7 days) */
};

enum {
	NPC_NONE	= 0,
	NPC_NARIELL	= 1,
	NPC_HARIKA	= 2,
	NPC_CURIAN	= 3,
	NPC_ARDORA	= 4,
	NPC_GARSVIK	= 5,
	NPC_ERWO	= 6
};

enum {
	HERO_TYPE_NONE		= 0,
	HERO_TYPE_GAUKLER	= 1,	/* juggler */
	HERO_TYPE_JAEGER	= 2,	/* hunter */
	HERO_TYPE_KRIEGER	= 3,	/* warrior */
	HERO_TYPE_STREUNER	= 4,	/* stray */
	HERO_TYPE_THORWALER	= 5,	/* Thorwalian */
	HERO_TYPE_ZWERG		= 6,	/* dwarf */
	/* magic users have id > 6 */
	HERO_TYPE_HEXE		= 7,	/* witch */
	HERO_TYPE_DRUIDE	= 8,	/* druid */
	HERO_TYPE_MAGIER	= 9,	/* mage */
	HERO_TYPE_AUELF		= 10,	/* green elf */
	HERO_TYPE_FIRNELF	= 11,	/* ice elf */
	HERO_TYPE_WALDELF	= 12,	/* sylvan elf */
};

enum {
	ACTOR_SPRITE_ID_NONE = 0,
	ACTOR_SPRITE_ID_GAUKLER__MALE = 1,
	ACTOR_SPRITE_ID_JAEGER__MALE = 2,
	ACTOR_SPRITE_ID_KRIEGER__MALE = 3,
	ACTOR_SPRITE_ID_STREUNER__MALE = 4,
	ACTOR_SPRITE_ID_THORWALER__MALE = 5,
	ACTOR_SPRITE_ID_ZWERG__MALE = 6,
	ACTOR_SPRITE_ID_HEXE__MALE = 7,
	ACTOR_SPRITE_ID_DRUIDE__MALE = 8,
	ACTOR_SPRITE_ID_MAGIER__MALE = 9,
	ACTOR_SPRITE_ID_ELF__MALE = 10,
	ACTOR_SPRITE_ID_GAUKLER__FEMALE = 12,
	ACTOR_SPRITE_ID_JAEGER__FEMALE = 13,
	ACTOR_SPRITE_ID_KRIEGER__FEMALE = 14,
	ACTOR_SPRITE_ID_STREUNER__FEMALE = 15,
	ACTOR_SPRITE_ID_THORWALER__FEMALE = 16,
	ACTOR_SPRITE_ID_ZWERG__FEMALE = 17,
	ACTOR_SPRITE_ID_HEXE__FEMALE = 18,
	ACTOR_SPRITE_ID_DRUIDE__FEMALE = 19,
	ACTOR_SPRITE_ID_MAGIER__FEMALE = 20,
	ACTOR_SPRITE_ID_ELF__FEMALE = 21,
	ACTOR_SPRITE_ID_OGER = 23,
	ACTOR_SPRITE_ID_ORK = 24,
	ACTOR_SPRITE_ID_GOBLIN = 25,
	ACTOR_SPRITE_ID_HESHTHOT = 26,
	ACTOR_SPRITE_ID_TIGER = 27,
	ACTOR_SPRITE_ID_SKELETT = 28,
	ACTOR_SPRITE_ID_WALDSCHRAT = 29,
	ACTOR_SPRITE_ID_MUMIE = 30,
	ACTOR_SPRITE_ID_HARPYIE = 31, /* Strangely, MONSTER_ID_LINDWURM and MONSTER_ID_HOEHLENDRACHE have this sprite_id. But they are not found in the game. */
	ACTOR_SPRITE_ID_SPINNE = 32,
	ACTOR_SPRITE_ID_KAEFER = 33,
	ACTOR_SPRITE_ID_ZOMBIE = 34,
	ACTOR_SPRITE_ID_PIRAT = 35,
	ACTOR_SPRITE_ID_RAEUBER = 36,
	ACTOR_SPRITE_ID_ZAUBERER = 37,
	ACTOR_SPRITE_ID_HUND = 38,
	ACTOR_SPRITE_ID_LOEWE = 39,
	ACTOR_SPRITE_ID_WOLF = 40
};

enum {
	FIG_ACTION_NONE			= 0,
	FIG_ACTION_MOVE			= 1,
	FIG_ACTION_MELEE_ATTACK		= 2,
	FIG_ACTION_GUARD		= 3,
	FIG_ACTION_SPELL		= 4,
	FIG_ACTION_USE_ITEM		= 5,
	FIG_ACTION_DROP_ITEM		= 6,
	FIG_ACTION_EXCHANGE_WEAPON	= 7,
	FIG_ACTION_EXCHANGE_ITEM	= 8,
	FIG_ACTION_CHECK_VALUES		= 9,
	FIG_ACTION_WAIT			= 10,
	FIG_ACTION_COMPUTER_FIGHT	= 11,
	FIG_ACTION_QUIT_AND_LOAD	= 12,
	FIG_ACTION_REPEAT_OPTION	= 13,
	FIG_ACTION_RANGE_ATTACK		= 15,
	FIG_ACTION_FLEE			= 16,
	FIG_ACTION_PARRY		= 100, /* is set in sub_hero_le() when all le are lost */
	FIG_ACTION_UNKNOWN3		= 102, /* drink potion? */
	FIG_ACTION_UNKNOWN4		= 103  /* cast spell? */
};

enum {
	DISEASE_ID_NONE			= 0,
	DISEASE_ID_WUNDFIEBER		= 1,
	DISEASE_ID_DUMPFSCHAEDEL	= 2,
	DISEASE_ID_BLAUE_KEUCHE		= 3,
	DISEASE_ID_PARALYSE		= 4,
	DISEASE_ID_SCHLACHTENFIEBER	= 5,
	DISEASE_ID_FROSTSCHAEDEN	= 6,
	DISEASE_ID_TOLLWUT		= 7
};

enum {
	DISEASE_STATUS_DISEASED	= -1,
	DISEASE_STATUS_HEALTHY	= 0,
	DISEASE_STATUS_RECOVER	= 1
};

enum {
	POISON_ID_NONE			= 0,
	POISON_ID_SHURINKNOLLENGIFT	= 1,
	POISON_ID_ARAX			= 2,
	POISON_ID_ANGSTGIFT		= 3,
	POISON_ID_SCHLAFGIFT		= 4,
	POISON_ID_GOLDLEIM		= 5,
	POISON_ID_KROETENSCHEMEL	= 6,
	POISON_ID_LOTUSGIFT		= 7,
	POISON_ID_KUKRIS		= 8,
	POISON_ID_BANNSTAUB		= 9
};

enum {
	POISON_STATUS_POISONED	= -1,
	POISON_STATUS_HEALTHY	= 0,
	POISON_STATUS_RECOVER	= 1
};

enum {
	ATTRIB_MU = 0,
	ATTRIB_KL = 1,
	ATTRIB_CH = 2,
	ATTRIB_FF = 3,
	ATTRIB_GE = 4,
	ATTRIB_IN = 5,
	ATTRIB_KK = 6,
	ATTRIB_AG = 7,
	ATTRIB_HA = 8,
	ATTRIB_RA = 9,
	ATTRIB_GG = 10,
	ATTRIB_TA = 11,
	ATTRIB_NG = 12,
	ATTRIB_JZ = 13,
	ATTRIB__END = 14
};

enum {
	/* Kampf */
	TA_WAFFENLOS		=  0, // 0x00
	TA_HIEBWAFFEN		=  1, // 0x01
	TA_STICHWAFFEN		=  2, // 0x02
	TA_SCHWERTER		=  3, // 0x03
	TA_AEXTE		=  4, // 0x04
	TA_SPEERE		=  5, // 0x05
	TA_ZWEIHAENDER		=  6, // 0x06
	TA_SCHUSSWAFFEN		=  7, // 0x07
	TA_WURFWAFFEN		=  8, // 0x08
	/* Körper */
	TA_AKROBATIK		=  9, // 0x09
	TA_KLETTERN		= 10, // 0x0a
	TA_KOERPERBEHERRSCHUNG	= 11, // 0x0b
	TA_REITEN		= 12, // 0x0c
	TA_SCHLEICHEN		= 13, // 0x0d
	TA_SCHWIMMEN		= 14, // 0x0e
	TA_SELBSTBEHERRSCHUNG	= 15, // 0x0f
	TA_TANZEN		= 16, // 0x10
	TA_VERSTECKEN		= 17, // 0x11
	TA_ZECHEN		= 18, // 0x12
	/* Gesellschaft */
	TA_BEKEHREN		= 19, // 0x13
	TA_BETOEREN		= 20, // 0x14
	TA_FEILSCHEN		= 21, // 0x15
	TA_GASSENWISSEN		= 22, // 0x16
	TA_LUEGEN		= 23, // 0x17
	TA_MENSCHENKENNTNIS	= 24, // 0x18
	TA_SCHAETZEN		= 25, // 0x19
	/* Natur */
	TA_FAEHRTENSUCHEN	= 26, // 0x1a
	TA_FESSELN		= 27, // 0x1b
	TA_ORIENTIERUNG		= 28, // 0x1c
	TA_PFLANZENKUNDE	= 29, // 0x1d
	TA_TIERKUNDE		= 30, // 0x1e
	TA_WILDNISLEBEN		= 31, // 0x1f
	/* Wissen */
	TA_ALCHIMIE		= 32, // 0x20
	TA_ALTE_SPRACHEN	= 33, // 0x21
	TA_GEOGRAPHIE		= 34, // 0x22
	TA_GESCHICHTE		= 35, // 0x23
	TA_GOETTER_KULTE	= 36, // 0x24
	TA_KRIEGSKUNST		= 37, // 0x25
	TA_LESEN		= 38, // 0x26
	TA_MAGIEKUNDE		= 39, // 0x27
	TA_SPRACHEN		= 40, // 0x28
	/* Handwerk */
	TA_ABRICHTEN		= 41, // 0x29
	TA_FAHRZEUGE		= 42, // 0x2a
	TA_FALSCHSPIEL		= 43, // 0x2b
	TA_HEILEN_GIFT		= 44, // 0x2c
	TA_HEILEN_KRANKHEITEN	= 45, // 0x2d
	TA_HEILEN_WUNDEN	= 46, // 0x2e
	TA_MUSIZIEREN		= 47, // 0x2f
	TA_SCHLOESSER		= 48, // 0x30
	TA_TASCHENDIEBSTAHL	= 49, // 0x31
	/* Intuition */
	TA_GEFAHRENSINN		= 50, // 0x32
	TA_SINNESSCHAERFE	= 51, // 0x33
	TA__END			= 52
};

enum {
	SP_NONE					=  0, // 0x00
	/* Antimagie */
	SP_BEHERRSCHUNG_BRECHEN			=  1, // 0x01
	SP_DESTRUCTIBO_ARCANITAS		=  2, // 0x02
	SP_GARDIANUM_PARADEI			=  3, // 0x03
	SP_ILLUSIONEN_ZERSTOEREN		=  4, // 0x04
	SP_VERWANDLUNG_BEENDEN			=  5, // 0x05
	/* Beherrschung */
	SP_BAND_UND_FESSEL			=  6, // 0x06
	SP_BANNBALADIN				=  7, // 0x07
	SP_BOESER_BLICK				=  8, // 0x08
	SP_GROSSE_GIER				=  9, // 0x09
	SP_GROSSE_VERWIRRUNG			= 10, // 0x0a
	SP_HERR_UEBER_DAS_TIERREICH		= 11, // 0x0b
	SP_HORRIPHOBUS				= 12, // 0x0c
	SP_MAGISCHER_RAUB			= 13, // 0x0d
	SP_RESPONDAMI_VERITAR			= 14, // 0x0e
	SP_SANFTMUT				= 15, // 0x0f
	SP_SOMNIGRAVIS_TAUSENDSCHLAF		= 16, // 0x10
	SP_ZWINGTANZ				= 17, // 0x11
	/* Daemonologie */
	SP_FUROR_BLUT_UND_SULPHURDAMPF		= 18, // 0x12
	SP_GEISTER_AUSTREIBEN			= 19, // 0x13
	SP_GEISTER_BESCHWOEREN			= 20, // 0x14
	SP_HEPTAGON_UND_KROETENEI		= 21, // 0x15
	SP_KRAEHENRUF				= 22, // 0x16
	SP_SKELETTARIUS_KRYPTADUFT		= 23, // 0x17
	/* Elemente */
	SP_ELEMENTARE_BESCHWOEREN		= 24, // 0x18
	SP_NIHILATIO_GRAVITAS			= 25, // 0x19
	SP_SOLIDIRID_FARBENSPIEL		= 26, // 0x1a
	/* Bewegung */
	SP_AXXELERATUS_BLITZGESCHWIND		= 27, // 0x1b
	SP_FORAMEN_FORAMINOR			= 28, // 0x1c
	SP_MOTORICUS_MOTILITICH			= 29, // 0x1d
	SP_SPURLOS_TRITTLOS_FAEHRTENLOS		= 30, // 0x1e
	SP_TRANSVERSALIS_TELEPORT		= 31, // 0x1f
	SP_UEBER_EIS_UND_UEBER_SCHNEE		= 32, // 0x20
	/* Heilung */
	SP_BALSAM_SALABUNDE			= 33, // 0x21
	SP_HEXENSPEICHEL			= 34, // 0x22
	SP_KLARUM_PURUM_KRAEUTERSUD		= 35, // 0x23
	SP_RUHE_KOERPER_RUHE_GEIST		= 36, // 0x24
	SP_TIERE_BESPRECHEN			= 37, // 0x25
	/* Wahrnehmung */
	SP_ADLERAUG_UND_LUCHSENOHR		= 38, // 0x26
	SP_ANALUES_ARCANSTRUKTUR		= 39, // 0x27
	SP_EIGENSCHAFTEN_SEID_GELESEN		= 40, // 0x28
	SP_EXPOSAMI_CREATUR			= 41, // 0x29
	SP_ODEM_ARCANUM				= 42, // 0x2a
	SP_PENETRIZZEL_HOLZ_UND_STEIN		= 43, // 0x2b
	SP_SENSIBAR_WAHR_UND_KLAR		= 44, // 0x2c
	/* Visionen */
	SP_CHAMAELIONI_MIMIKRY			= 45, // 0x2d
	SP_DUPLICATUS_DOPPELPEIN		= 46, // 0x2e
	SP_HARMLOSE_GESTALT			= 47, // 0x2f
	SP_HEXENKNOTEN				= 48, // 0x30
	/* Kampf */
	SP_BLITZ_DICH_FIND			= 49, // 0x31
	SP_ECLIPTIFACTUS			= 50, // 0x32
	SP_EISENROST_UND_GRUENER_SPAN		= 51, // 0x33
	SP_FULMINICTUS_DONNERKEIL		= 52, // 0x34
	SP_IGNIFAXIUS_FLAMMENSTRAHL		= 53, // 0x35
	SP_PLUMBUMBARUM_UND_NARRETEI		= 54, // 0x36
	SP_RADAU				= 55, // 0x37
	SP_SAFT_KRAFT_MONSTERMACHT		= 56, // 0x38
	SP_SCHARFES_AUGE_UND_SICHRE_HAND	= 57, // 0x39
	/* Verstaendigung */
	SP_HEXENBLICK				= 58, // 0x3a
	SP_NEKROPATHIA_MODERND_LEICH		= 59, // 0x3b
	/* Verwandlung */
	SP_ADLER_WOLF_UND_HAMMERHAI		= 60, // 0x3c
	SP_ARCANO_PSYCHOSTABILIS		= 61, // 0x3d
	SP_ARMATRUTZ				= 62, // 0x3e
	SP_CH_STEIGERN				= 63, // 0x3f
	SP_FEUERBANN				= 64, // 0x40
	SP_FF_STEIGERN				= 65, // 0x41
	SP_GE_STEIGERN				= 66, // 0x42
	SP_IN_STEIGERN				= 67, // 0x43
	SP_KK_STEIGERN				= 68, // 0x44
	SP_KL_STEIGERN				= 69, // 0x45
	SP_MU_STEIGERN				= 70, // 0x46
	SP_MUTABILI_HYBRIDIL			= 71, // 0x47
	SP_PARALUE_PARALEIN			= 72, // 0x48
	SP_SALANDER_MUTANDERER			= 73, // 0x49
	SP_IN_SEE_UND_FLUSS			= 74, // 0x4a
	SP_VISIBILI_VANITAR			= 75, // 0x4b
	/* Veraenderung */
	SP_ABVENENUM_PEST_UND_GALLE		= 76, // 0x4c
	SP_AEOLITUS_WINDGEBRAUS			= 77, // 0x4d
	SP_BRENNE_TOTER_STOFF			= 78, // 0x4e
	SP_CLAUDIBUS_CLAVISTIBOR		= 79, // 0x4f
	SP_DUNKELHEIT				= 80, // 0x50
	SP_WEICHES_ERSTARRE			= 81, // 0x51
	SP_FLIM_FLAM_FUNKEL			= 82, // 0x52
	SP_HARTES_SCHMELZE			= 83, // 0x53
	SP_SILENTIUM_SILENTILLE			= 84, // 0x54
	SP_STURMGEBRUELL_BESAENFTGE_DICH	= 85, // 0x55
	SP__END					= 86
};

enum {
	MONSTER_ID_NONE                 = 0,
	MONSTER_ID_OGER                 = 1,
	MONSTER_ID_ORK__LVL_1           = 2,
	MONSTER_ID_GOBLIN__LVL_1        = 3,
	MONSTER_ID_HESHTHOT__LVL_1      = 4,
	MONSTER_ID_SAEBELZAHNTIGER      = 5,
	MONSTER_ID_SKELETT__LVL_1       = 6,
	MONSTER_ID_WALDSCHRAT           = 7,
	MONSTER_ID_MUMIE__LVL_1         = 8,
	MONSTER_ID_HARPYIE              = 9,
	MONSTER_ID_WALDSPINNE           = 10,
	MONSTER_ID_GROSSE_SCHROETER     = 11,
	MONSTER_ID_PIRAT__LVL_1         = 12,
	MONSTER_ID_STEUERMANN           = 13,
	MONSTER_ID_KAPITAEN             = 14,
	MONSTER_ID_STREUNER__LVL_1      = 15,
	MONSTER_ID_ZOMBIE               = 16,
	MONSTER_ID_HESHTHOT__LVL_2      = 17,
	MONSTER_ID_DRUIDE__LVL_2        = 18,
	MONSTER_ID_DRUIDE__LVL_3        = 19,
	MONSTER_ID_THORWALER__LVL_1     = 20,
	MONSTER_ID_THORWALER__LVL_3     = 21,
	MONSTER_ID_THORWALER__LVL_5     = 22,
	MONSTER_ID_THORWALER__LVL_6     = 23,
	MONSTER_ID_GOBLIN__LVL_2        = 24,
	MONSTER_ID_GOBLIN__LVL_3        = 25,
	MONSTER_ID_ORK__LVL_2           = 26,
	MONSTER_ID_ORK__LVL_3           = 27,
	MONSTER_ID_PIRAT__LVL_2         = 28,
	MONSTER_ID_PIRAT__LVL_3         = 29,
	MONSTER_ID_STREUNER__LVL_2      = 30,
	MONSTER_ID_STREUNER__LVL_3      = 31,
	MONSTER_ID_STREUNER__LVL_4      = 32,
	MONSTER_ID_SKELETT__LVL_2       = 33,
	MONSTER_ID_FOLTERKNECHT         = 34,
	MONSTER_ID_GRIMWOLF__LVL_1      = 35,
	MONSTER_ID_GRIMWOLF__LVL_3      = 36,
	MONSTER_ID_HOEHLENSPINNE__LVL_1 = 37,
	MONSTER_ID_HOEHLENSPINNE__LVL_2 = 38,
	MONSTER_ID_HOEHLENSPINNE__LVL_3 = 39,
	MONSTER_ID_KRIEGER              = 40,
	MONSTER_ID_STADTGARDIST__LVL_3  = 41,
	MONSTER_ID_STADTGARDIST__LVL_4  = 42,
	MONSTER_ID_STADTGARDIST__LVL_5  = 43,
	MONSTER_ID_KRIEGSVETERAN        = 44,
	MONSTER_ID_MUMIE__LVL_2         = 45,
	MONSTER_ID_MAGIER__LVL_3        = 46,
	MONSTER_ID_MAGIER__LVL_6        = 47,
	MONSTER_ID_ZAUBERER__LVL_3      = 48,
	MONSTER_ID_ZAUBERER__LVL_6      = 49,
	MONSTER_ID_ZWERG                = 50,
	MONSTER_ID_LINDWURM             = 51,
	MONSTER_ID_HOEHLENDRACHE        = 52,
	MONSTER_ID_KULTIST__LVL_2       = 53,
	MONSTER_ID_KULTIST__LVL_3__1    = 54,
	MONSTER_ID_KULTIST__LVL_3__2    = 55,
	MONSTER_ID_KULTIST__LVL_4       = 56,
	MONSTER_ID_PRIESTER__LVL_2      = 57,
	MONSTER_ID_PRIESTER__LVL_3      = 58,
	MONSTER_ID_HOHEPRIESTER         = 59,
	MONSTER_ID_ESTORIK              = 60,
	MONSTER_ID_PIRATENBOSS          = 61,
	MONSTER_ID_PIRAT__LVL_5         = 62,
	MONSTER_ID_MAGIER__LVL_4        = 63,
	MONSTER_ID_ZWERGENVETERAN       = 64,
	MONSTER_ID_RAEUBER__LVL_2       = 65,
	MONSTER_ID_RAEUBER__LVL_3       = 66,
	MONSTER_ID_RAEUBER__LVL_5       = 67,
	MONSTER_ID_RAEUBER__LVL_6       = 68,
	MONSTER_ID_RAEUBER__LVL_8       = 69,
	MONSTER_ID_GORAH                = 70,
	MONSTER_ID_STEPPENHUND          = 71,
	MONSTER_ID_ORKCHAMPION          = 72,
	MONSTER_ID_PIRAT__LVL_10        = 73,
	MONSTER_ID_STADTGARDIST__LVL_2  = 74,
	MONSTER_ID_WALDLOEWE            = 75,
	MONSTER_ID_SPINNENDAEMON        = 76,
	MONSTER_ID_HESHTOT__WEAK        = 77
};

enum {
	MONSTER_SPELL_NONE			=  0, // 0x00
	MONSTER_SPELL_VERWANDLUNG_BEENDEN	=  1, // 0x01
	MONSTER_SPELL_BANNBALADIN		=  2, // 0x02
	MONSTER_SPELL_BOESER_BLICK		=  3, // 0x03
	MONSTER_SPELL_HORRIPHOBUS		=  4, // 0x04
	MONSTER_SPELL_AXXELERATUS		=  5, // 0x05
	MONSTER_SPELL_BALSAM			=  6, // 0x06
	MONSTER_SPELL_BLITZ			=  7, // 0x07
	MONSTER_SPELL_EISENROST			=  8, // 0x08
	MONSTER_SPELL_FULMINICTUS		=  9, // 0x09
	MONSTER_SPELL_IGNIFAXIUS		= 10, // 0x0a
	MONSTER_SPELL_PLUMBUMBARUM		= 11, // 0x0b
	MONSTER_SPELL_SAFT_KRAFT_MONSTERMACHT	= 12, // 0x0c
	MONSTER_SPELL_ARMATRUTZ			= 13, // 0x0d
	MONSTER_SPELL_PARALUE			= 14  // 0x0e
};

enum {
	HERO_INVENTORY_SLOT_HEAD	= 0,
	HERO_INVENTORY_SLOT_ARMS	= 1,
	HERO_INVENTORY_SLOT_BODY	= 2,
	HERO_INVENTORY_SLOT_RIGHT_HAND	= 3,
	HERO_INVENTORY_SLOT_LEFT_HAND	= 4,
	HERO_INVENTORY_SLOT_LEGS	= 5,
	HERO_INVENTORY_SLOT_FEET	= 6,
	HERO_INVENTORY_SLOT_KNAPSACK_1	= 7,
	HERO_INVENTORY_SLOT_KNAPSACK_2	= 8,
	HERO_INVENTORY_SLOT_KNAPSACK_3	= 9,
	HERO_INVENTORY_SLOT_KNAPSACK_4	= 10,
	HERO_INVENTORY_SLOT_KNAPSACK_5	= 11,
	HERO_INVENTORY_SLOT_KNAPSACK_6	= 12,
	HERO_INVENTORY_SLOT_KNAPSACK_7	= 13,
	HERO_INVENTORY_SLOT_KNAPSACK_8	= 14,
	HERO_INVENTORY_SLOT_KNAPSACK_9	= 15,
	HERO_INVENTORY_SLOT_KNAPSACK_10	= 16,
	HERO_INVENTORY_SLOT_KNAPSACK_11	= 17,
	HERO_INVENTORY_SLOT_KNAPSACK_12	= 18,
	HERO_INVENTORY_SLOT_KNAPSACK_13	= 19,
	HERO_INVENTORY_SLOT_KNAPSACK_14	= 20,
	HERO_INVENTORY_SLOT_KNAPSACK_15	= 21,
	HERO_INVENTORY_SLOT_KNAPSACK_16	= 22,
	HERO_INVENTORY_SLOT_EYE		= 23,
	HERO_INVENTORY_SLOT_MOUTH	= 24,
};

#define NR_HERO_INVENTORY_SLOTS (23)

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/ITEMS.DAT */
	/* except the last entry ITEM_SUBTYPE_ARMOR_LEFT_HAND (shields), all values agree with HERO_INVENTORY_SLOT_... */
	ARMOR_TYPE_HEAD 	= 0,
	ARMOR_TYPE_ARMS 	= 1,
	ARMOR_TYPE_BODY 	= 2,
	ARMOR_TYPE_LEGS 	= 5,
	ARMOR_TYPE_FEET 	= 6,
	ARMOR_TYPE_LEFT_HAND 	= 9, /* shields */
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/ITEMS.DAT */
	/* parallel to TA_WAFFENLOS, TA_HIEBWAFFEN, TA_STICHWAFFEN ... */
	WEAPON_TYPE_WAFFENLOS	= 0, /* note that ammunition (arrows, bolts) have this weapon type entry */
	WEAPON_TYPE_HIEBWAFFE	= 1,
	WEAPON_TYPE_STICHWAFFE	= 2,
	WEAPON_TYPE_SCHWERT	= 3,
	WEAPON_TYPE_AXT		= 4,
	WEAPON_TYPE_SPEER	= 5,
	WEAPON_TYPE_ZWEIHAENDER	= 6,
	WEAPON_TYPE_SCHUSSWAFFE	= 7,
	WEAPON_TYPE_WURFWAFFE	= 8
};

/* https://github.com/shihan42/BrightEyesWiki/wiki/SCHICKM.EXE#Waffentabelle */

/* ranged distance types, named according to DSA3 rules. */
enum {
	/*                                       implemented distance on chessboard */
	/*                                                    distance in Schritt according to DSA3 rules */
	RANGED_DISTANCE_TYPE_EXTREM_NAH  = 0, /*  1-- 2       1--  5 */
	RANGED_DISTANCE_TYPE_SEHR_NAH    = 1, /*  3-- 4       5-- 10 */
	RANGED_DISTANCE_TYPE_NAH         = 2, /*  5-- 6      10-- 15 */
	RANGED_DISTANCE_TYPE_MITTEL      = 3, /*  7-- 9      15-- 25 */
	RANGED_DISTANCE_TYPE_WEIT        = 4, /* 10-- 15     25-- 40 */
	RANGED_DISTANCE_TYPE_SEHR_WEIT   = 5, /* 16-- 20     40-- 60 */
	RANGED_DISTANCE_TYPE_EXTREM_WEIT = 6  /* 21--        61--100 */
};

/* target sizes (for ranged attack), named according to DSA3 rules. */
enum {
	/*                             examples according to DSA3 rules */
	TARGET_SIZE_WINZIG     = 0, /* Silbertaler, Drachenauge, Maus, Ratte, Kroete */
	TARGET_SIZE_SEHR_KLEIN = 1, /* Schlange, Fasan, Katze, Rabe */
	TARGET_SIZE_KLEIN      = 2, /* Wolf, Reh, Kobold, Zwerg */
	TARGET_SIZE_MITTEL     = 3, /* Goblin, Elf, Mensch, Ork */
	TARGET_SIZE_GROSS      = 4, /* Pferd, Elch, Oger, Troll */
	TARGET_SIZE_SEHR_GROSS = 5  /* Scheunentor, Drache, Elefant, Riese */
};

enum {
	NUTRITION_TYPE_DRINK	= 0,
	NUTRITION_TYPE_FOOD	= 1
};

enum {
	HERB_POTION_TYPE_HERB_AND_POISON = 0,
	HERB_POTION_TYPE_POTION = 1
};

enum {
	LOCTYPE_NONE		= 0,
	LOCTYPE_UNKN1		= 1,
	LOCTYPE_TEMPLE		= 2,
	LOCTYPE_TAVERN		= 3,
	LOCTYPE_HEALER		= 4,
	LOCTYPE_MERCHANT	= 5,
	LOCTYPE_WILDCAMP	= 6,
	LOCTYPE_INN		= 7,
	LOCTYPE_SMITH		= 8,
	LOCTYPE_MARKET		= 9,
	LOCTYPE_CITIZEN		= 10, // Leuchtturm Runin is of this type
	LOCTYPE_HARBOR 		= 11,
	LOCTYPE_SIGNPOST	= 12,
	LOCTYPE_INFORMER	= 13,
	LOCTYPE_DUNGEON_ENTRY	= 14, // used in Prem, Oberorken. The entry to "Alte Zwingfeste" in Thorwal is LOCTYPE_SPECIAL
	LOCTYPE_UNKN2		= 15,
	LOCTYPE_HOUSE		= 16,
	LOCTYPE_SPECIAL		= 17, // used in Thorwal, Phexcaer, Einsiedlersee
	LOCTYPE_AREA_CAMP	= 18
};

enum {
	MERCHANT_WEAPONS = 1,
	MERCHANT_HERBS = 2,
	MERCHANT_GENERAL = 3
};

enum {
	MERCHANT_TYPEINDEX_MARKET = 90,
	MERCHANT_TYPEINDEX_TREBORN_KOLBERG = 91,
	MERCHANT_TYPEINDEX_THORWAL_ZEUGHAUS = 92,
	MERCHANT_TYPEINDEX_STREET_MERCHANT = 93
};

enum {
	GAME_STATE_MAIN		= 0,
	GAME_STATE_DEAD		= 1, /* all heroes dead */
	GAME_STATE_UNKNOWN	= 2,
	GAME_STATE_QUIT		= 3, /* ordinary quit */
	GAME_STATE_TIMEUP	= 4, /* max. game time is up */
	GAME_STATE_OUTRO	= 5, /* after (successful) outro */
	GAME_STATE_FIGQUIT	= 7, /* quit game during fight */
	GAME_STATE_VICTORY	= 99 /* final fight won (before outro) */
};

enum {
	GOD_ID_PRAIOS		= 1,
	GOD_ID_RONDRA		= 2,
	GOD_ID_EFFERD		= 3,
	GOD_ID_TRAVIA		= 4,
	GOD_ID_BORON		= 5,
	GOD_ID_HESINDE		= 6,
	GOD_ID_FIRUN		= 7,
	GOD_ID_TSA		= 8,
	GOD_ID_PHEX		= 9,
	GOD_ID_PERAINE		= 10,
	GOD_ID_INGERIMM		= 11,
	GOD_ID_RAHJA		= 12,
	GOD_ID_SWAFNIR		= 13,
	GOD_ID_IFIRN		= 14,
	GOD_ID__END		= 15
};

enum {
	INFORMER_ID_JURGE	= 0,
	INFORMER_ID_HJORE	= 1,
	INFORMER_ID_YASMA	= 2,
	INFORMER_ID_UMBRIK	= 3,
	INFORMER_ID_ISLEIF	= 4,
	INFORMER_ID_RAGNA	= 5,
	INFORMER_ID_BEORN	= 6,
	INFORMER_ID_ASGRIMM	= 7,
	INFORMER_ID_ELIANE	= 8,
	INFORMER_ID_OLVIR	= 9,
	INFORMER_ID_SWAFNILD	= 10,
	INFORMER_ID_TREBORN	= 11,
	INFORMER_ID_UNICORN	= 12,
	INFORMER_ID_ALGRID	= 13,
	INFORMER_ID_TIOMAR	= 14,
	INFORMER_ID__END	= 15
};

enum { /* identifiers are based on the entries of FIGHT.LST */
	FIGHT_ID_ZUFALL1_LAND  = 0,
	FIGHT_ID_LAND_FIGHT1_1 = 1,
	FIGHT_ID_ZUFALL2_LAND  = 2,
	FIGHT_ID_ZUFALL3_LAND  = 3,
	FIGHT_ID_ZUFALL4_LAND  = 4,
	FIGHT_ID_SHIP3         = 5,
	FIGHT_ID_SHIP4         = 6,
	FIGHT_ID_SHIP5         = 7,
	FIGHT_ID_SHIP6         = 8,
	FIGHT_ID_SHIP8         = 9,
	FIGHT_ID_SHIP9         = 10,
	FIGHT_ID_SHIP12        = 11,
	FIGHT_ID_SHIP14        = 12,
	FIGHT_ID_SHIP15        = 13,
	FIGHT_ID_SHIP17        = 14,
	FIGHT_ID_SHIP18        = 15,
	FIGHT_ID_SHIP19        = 16,
	FIGHT_ID_SHIP21        = 17,
	FIGHT_ID_SHIP22        = 18,
	FIGHT_ID_SHIP23A       = 19,
	FIGHT_ID_SHIP23B       = 20,
	FIGHT_ID_SHIP24        = 21,
	FIGHT_ID_SHIP10        = 22,
	FIGHT_ID_F061_2        = 23,
	FIGHT_ID_F061_3        = 24,
	FIGHT_ID_F061_4B       = 25,
	FIGHT_ID_F061_4A       = 26,
	FIGHT_ID_F061_5        = 27,
	FIGHT_ID_F061_6A       = 28,
	FIGHT_ID_F108_1        = 29,
	FIGHT_ID_F108_2        = 30,
	FIGHT_ID_F108_3A       = 31,
	FIGHT_ID_F108_3B       = 32,
	FIGHT_ID_F108_3C       = 33,
	FIGHT_ID_F108_4        = 34,
	FIGHT_ID_F108_6        = 35,
	FIGHT_ID_F108_9        = 36,
	FIGHT_ID_F108_7        = 37,
	FIGHT_ID_F051_05_4     = 38,
	FIGHT_ID_F051_05_3     = 39,
	FIGHT_ID_F051_05_2     = 40,
	FIGHT_ID_F051_05_1     = 41,
	FIGHT_ID_F051_07       = 42,
	FIGHT_ID_F051_09       = 43,
	FIGHT_ID_F051_13       = 44,
	FIGHT_ID_F108_15       = 45,
	FIGHT_ID_F051_15       = 46,
	FIGHT_ID_F051_16       = 47,
	FIGHT_ID_F051_16_1     = 48,
	FIGHT_ID_F051_03       = 49,
	FIGHT_ID_F051_04       = 50,
	FIGHT_ID_F051_17       = 51,
	FIGHT_ID_F051_18       = 52,
	FIGHT_ID_F051_19       = 53,
	FIGHT_ID_F051_20_2     = 54,
	FIGHT_ID_F051_18_3     = 55,
	FIGHT_ID_F046_01       = 56,
	FIGHT_ID_F046_04       = 57,
	FIGHT_ID_F046_06       = 58,
	FIGHT_ID_F046_07       = 59,
	FIGHT_ID_F046_10       = 60,
	FIGHT_ID_F046_12       = 61,
	FIGHT_ID_F046_13       = 62,
	FIGHT_ID_F046_15       = 63,
	FIGHT_ID_F046_16       = 64,
	FIGHT_ID_F046_18       = 65,
	FIGHT_ID_F046_22       = 66,
	FIGHT_ID_F046_24       = 67,
	FIGHT_ID_F046_25       = 68,
	FIGHT_ID_F046_26       = 69,
	FIGHT_ID_F046_26_5     = 70,
	FIGHT_ID_F046_27       = 71,
	FIGHT_ID_F046_28       = 72,
	FIGHT_ID_F046_31       = 73,
	FIGHT_ID_F076_04       = 74,
	FIGHT_ID_F076_06       = 75,
	FIGHT_ID_F076_07       = 76,
	FIGHT_ID_F076_10       = 77,
	FIGHT_ID_F076_11       = 78,
	FIGHT_ID_F076_13       = 79,
	FIGHT_ID_F094_02       = 80,
	FIGHT_ID_F094_05       = 81,
	FIGHT_ID_F094_10       = 82,
	FIGHT_ID_F094_13       = 83,
	FIGHT_ID_F094_17       = 84,
	FIGHT_ID_F094_19       = 85,
	FIGHT_ID_F094_22       = 86,
	FIGHT_ID_F100_01       = 87,
	FIGHT_ID_F100_03       = 88,
	FIGHT_ID_F100_05       = 89,
	FIGHT_ID_F100_12       = 90,
	FIGHT_ID_F100_13       = 91,
	FIGHT_ID_F126_03       = 92,
	FIGHT_ID_F126_07       = 93,
	FIGHT_ID_F126_08       = 94, /* fleeing cultist */
	FIGHT_ID_F126_09       = 95,
	FIGHT_ID_F126_11       = 96,
	FIGHT_ID_F126_12       = 97,
	FIGHT_ID_F126_13       = 98,
	FIGHT_ID_F126_17       = 99,
	FIGHT_ID_F126_18       = 100,
	FIGHT_ID_F126_20       = 101,
	FIGHT_ID_F126_22       = 102,
	FIGHT_ID_F126_23       = 103,
	FIGHT_ID_F126_25       = 104,
	FIGHT_ID_F126_27       = 105,
	FIGHT_ID_F126_28       = 106,
	FIGHT_ID_F129_05       = 107,
	FIGHT_ID_F129_08       = 108,
	FIGHT_ID_F129_09       = 109,
	FIGHT_ID_F129_18       = 110,
	FIGHT_ID_F129_21       = 111,
	FIGHT_ID_F129_23       = 112,
	FIGHT_ID_F129_24       = 113,
	FIGHT_ID_F129_25       = 114,
	FIGHT_ID_F129_27       = 115,
	FIGHT_ID_F129_28       = 116,
	FIGHT_ID_F129_29       = 117,
	FIGHT_ID_F129_30       = 118,
	FIGHT_ID_F131_01A      = 119,
	FIGHT_ID_F131_01B      = 120,
	FIGHT_ID_F131_04       = 121,
	FIGHT_ID_F131_05       = 122,
	FIGHT_ID_F131_06       = 123,
	FIGHT_ID_F131_07       = 124,
	FIGHT_ID_F131_08       = 125,
	FIGHT_ID_F131_10       = 126,
	FIGHT_ID_F131_11_1     = 127,
	FIGHT_ID_F131_14A      = 128,
	FIGHT_ID_F131_14B      = 129,
	FIGHT_ID_F131_16       = 130,
	FIGHT_ID_DFIN12        = 131,
	FIGHT_ID_DFIN16        = 132,
	FIGHT_ID_DFIN18A       = 133,
	FIGHT_ID_DFIN18B       = 134,
	FIGHT_ID_DFIN26        = 135,
	FIGHT_ID_DFIN27A       = 136,
	FIGHT_ID_DFIN27B       = 137,
	FIGHT_ID_DFIN28        = 138,
	FIGHT_ID_DFIN30        = 139,
	FIGHT_ID_DPRE10_1      = 140,
	FIGHT_ID_DOBE07        = 141,
	FIGHT_ID_DOBE09        = 142,
	FIGHT_ID_DOBE11        = 143,
	FIGHT_ID_DOBE20        = 144,
	FIGHT_ID_DOBE19        = 145,
	FIGHT_ID_DOBE22        = 146,
	FIGHT_ID_DTHO03        = 147,
	FIGHT_ID_DTHO05        = 148,
	FIGHT_ID_DTHO06        = 149,
	FIGHT_ID_DTHO09        = 150,
	FIGHT_ID_DTHO10        = 151,
	FIGHT_ID_DTHO13        = 152,
	FIGHT_ID_DTHO15        = 153,
	FIGHT_ID_DTHO16        = 154,
	FIGHT_ID_DTHO18        = 155,
	FIGHT_ID_DTHO19        = 156,
	FIGHT_ID_DTHO20        = 157,
	FIGHT_ID_DTHO20_1      = 158,
	FIGHT_ID_DTHO21A       = 159,
	FIGHT_ID_DTHO21B       = 160,
	FIGHT_ID_DTHO23        = 161,
	FIGHT_ID_DTHO25        = 162,
	FIGHT_ID_DTHO27        = 163,
	FIGHT_ID_DTHO43        = 164,
	FIGHT_ID_DTHO48_1      = 165,
	FIGHT_ID_DTHO49        = 166,
	FIGHT_ID_DTHO50        = 167,
	FIGHT_ID_DTHO53        = 168,
	FIGHT_ID_DTHO55        = 169,
	FIGHT_ID_DTHO56        = 170,
	FIGHT_ID_DTHO57        = 171,
	FIGHT_ID_DTHO57_1      = 172,
	FIGHT_ID_DTHO58        = 173,
	FIGHT_ID_DTHO59        = 174,
	FIGHT_ID_DTHO60        = 175,
	FIGHT_ID_DTHO61        = 176,
	FIGHT_ID_F031          = 177,
	FIGHT_ID_F035          = 178,
	FIGHT_ID_F046          = 179,
	FIGHT_ID_F064          = 180, /* Gorah */
	FIGHT_ID_F066          = 181,
	FIGHT_ID_F074          = 182,
	FIGHT_ID_F075_A        = 183,
	FIGHT_ID_F075_B        = 184,
	FIGHT_ID_F075_C        = 185,
	FIGHT_ID_F077          = 186,
	FIGHT_ID_F080          = 187,
	FIGHT_ID_F084          = 188, /* travel event 84 */
	FIGHT_ID_F099          = 189, /* 4 harpies */
	FIGHT_ID_F101          = 190,
	FIGHT_ID_F122          = 191,
	FIGHT_ID_F144          = 192, /* travel event 144: final fight */
	FIGHT_ID_DASP1A        = 193,
	FIGHT_ID_DASP1B        = 194,
	FIGHT_ID_DASP2         = 195,
	FIGHT_ID_DASP3         = 196,
	FIGHT_ID_DASP4         = 197,
	FIGHT_ID_DASP5         = 198,
	FIGHT_ID_DASP6A        = 199,
	FIGHT_ID_DASP6B        = 200,
	FIGHT_ID_DASP7         = 201,
	FIGHT_ID_DASP8         = 202,
	FIGHT_ID_DASP9         = 203,
	FIGHT_ID_DASP10        = 204,
	FIGHT_ID_DASP11        = 205,
	FIGHT_ID_DASP12A       = 206,
	FIGHT_ID_DASP12B       = 207,
	FIGHT_ID_DASP13        = 208,
	FIGHT_ID_DASP14        = 209,
	FIGHT_ID_DASP16        = 210,
	FIGHT_ID_DASP17        = 211,
	FIGHT_ID_DASP18        = 212,
	FIGHT_ID_THOR8         = 213,
	FIGHT_ID_CITYFIGHT1    = 214,
	FIGHT_ID_CITYFIGHT2    = 215,
	FIGHT_ID_CITYFIGHT3    = 216,
	FIGHT_ID_CAMPFIGHT1    = 217,
	FIGHT_ID_CAMPFIGHT2    = 218,
	FIGHT_ID_CAMPFIGHT3    = 219,
	FIGHT_ID_SHIP10_1      = 220,
	FIGHT_ID_F061_6B       = 221,
	FIGHT_ID_F051_16A      = 222,
	FIGHT_ID_F051_16B      = 223,
	FIGHT_ID_F051_14A      = 224,
	FIGHT_ID_F051_14B      = 225,
	FIGHT_ID_F051_117      = 226,
	FIGHT_ID_F129_124      = 227,
	FIGHT_ID_F131_08A      = 228,
	FIGHT_ID_F131_08B      = 229,
	FIGHT_ID_F031_14A      = 230,
	FIGHT_ID_DTH021A       = 231,
	FIGHT_ID_CAMPFIGHT4    = 232,
	FIGHT_ID_SHIP10_2      = 233,
	FIGHT_ID_F051_02       = 234,
	FIGHT_ID_F051_5        = 235,
	FIGHT_ID_PHEX3         = 236,
	FIGHT_ID_PHEX22        = 237,
	FIGHT_ID_PHEX23        = 238,
	FIGHT_ID_PHEX24        = 239,
	FIGHT_ID_F131_01       = 240,
	FIGHT_ID_F129_08A      = 241,
	FIGHT_ID_DTHO14        = 242,
	FIGHT_ID_WILD1         = 243,
	FIGHT_ID_WILD2         = 244,
	FIGHT_ID_WILD3         = 245,
	FIGHT_ID_WILD4         = 246,
	FIGHT_ID_WILD5         = 247,
	FIGHT_ID_WILD6         = 248,
	FIGHT_ID_WILD7         = 249,
	FIGHT_ID_WILD8         = 250,
	FIGHT_ID_S001          = 251,
	FIGHT_ID_F051_14C      = 252,
	FIGHT_ID_F129_17       = 253,
	FIGHT_ID_DCAMPFIGHT1   = 254,
	FIGHT_ID_DCAMPFIGHT2   = 255,
	FIGHT_ID_DCAMPFIGHT3   = 256,
	FIGHT_ID_DCAMPFIGHT4   = 257
};

enum {
	FIG_BACKGROUND_ID_NONE = 0,
	FIG_BACKGROUND_ID_1 = 1,
	FIG_BACKGROUND_ID_2 = 2,
	FIG_BACKGROUND_ID_3 = 3,
	FIG_BACKGROUND_ID_4 = 4,
	FIG_BACKGROUND_ID_6 = 6,
	FIG_BACKGROUND_ID_7 = 7
};

enum {
	AREA_TYPE_DUNGEON = 0,
	AREA_TYPE_TOWN = 1,
	AREA_TYPE_NONE = -1
};

enum {
	TOWN_ID_NONE			=  0, // %0x00
	TOWN_ID_THORWAL			=  1, // %0x01
	TOWN_ID_SERSKE			=  2, // %0x02
	TOWN_ID_BREIDA			=  3, // %0x03
	TOWN_ID_PEILINEN		=  4, // %0x04
	TOWN_ID_ROVAMUND		=  5, // %0x05
	TOWN_ID_NORDVEST		=  6, // %0x06
	TOWN_ID_KRAVIK			=  7, // %0x07
	TOWN_ID_SKELELLEN		=  8, // %0x08
	TOWN_ID_MERSKE			=  9, // %0x09
	TOWN_ID_EFFERDUN		= 10, // %0x0a
	TOWN_ID_TJOILA			= 11, // %0x0b
	TOWN_ID_RUKIAN			= 12, // %0x0c
	TOWN_ID_ANGBODIRTAL		= 13, // %0x0d
	TOWN_ID_AUPLOG			= 14, // %0x0e
	TOWN_ID_VILNHEIM		= 15, // %0x0f
	TOWN_ID_BODON			= 16, // %0x10
	TOWN_ID_OBERORKEN		= 17, // %0x11
	TOWN_ID_PHEXCAER		= 18, // %0x12
	TOWN_ID_GROENVELDEN		= 19, // %0x13
	TOWN_ID_FELSTEYN		= 20, // %0x14
	TOWN_ID_EINSIEDLERSEE		= 21, // %0x15
	TOWN_ID_ORKANGER		= 22, // %0x16
	TOWN_ID_CLANEGH			= 23, // %0x17
	TOWN_ID_LISKOR			= 24, // %0x18
	TOWN_ID_THOSS			= 25, // %0x19
	TOWN_ID_TJANSET			= 26, // %0x1a
	TOWN_ID_ALA			= 27, // %0x1b
	TOWN_ID_ORVIL			= 28, // %0x1c
	TOWN_ID_OVERTHORN		= 29, // %0x1d
	TOWN_ID_ROVIK			= 30, // %0x1e
	TOWN_ID_HJALSINGOR		= 31, // %0x1f
	TOWN_ID_GUDDASUNDEN		= 32, // %0x20
	TOWN_ID_KORD			= 33, // %0x21
	TOWN_ID_TREBAN			= 34, // %0x22
	TOWN_ID_ARYN			= 35, // %0x23
	TOWN_ID_RUNINSHAVEN		= 36, // %0x24
	TOWN_ID_OTTARJE			= 37, // %0x25
	TOWN_ID_SKJAL			= 38, // %0x26
	TOWN_ID_PREM			= 39, // %0x27
	TOWN_ID_DASPOTA			= 40, // %0x28
	TOWN_ID_RYBON			= 41, // %0x29
	TOWN_ID_LJASDAHL		= 42, // %0x2a
	TOWN_ID_VARNHEIM		= 43, // %0x2b
	TOWN_ID_VAERMHAG		= 44, // %0x2c
	TOWN_ID_TYLDON			= 45, // %0x2d
	TOWN_ID_VIDSAND			= 46, // %0x2e
	TOWN_ID_BRENDHIL		= 47, // %0x2f
	TOWN_ID_MANRIN			= 48, // %0x30
	TOWN_ID_FAEHRSTATION_TJOILA	= 49, // %0x31
	TOWN_ID_FAEHRE_ANGBODIRTAL	= 50, // %0x32
	TOWN_ID_HJALLANDER_HOF		= 51, // %0x33
	TOWN_ID_LEUCHTTURM_RUNIN	= 52, // %0x34
	TOWN_ID__END			= 53
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/DAT-%28Stadt%29 seems to be wrong */
	TOWN_TILE_STREET = 0,
	TOWN_TILE_TEMPLE = 1,
	TOWN_TILE_HOUSE_1 = 2, /* usually: stone wall, 3 stairs at the entrance, with a hatch under the window. */ /* dark brown */
	TOWN_TILE_HOUSE_2 = 3, /* usually: wooden blockhouse. red-white signs on the window shutters. */ //dark blue
	TOWN_TILE_HOUSE_3 = 4, /* usually: timber framework, window half-open. */ //olive green
	TOWN_TILE_HOUSE_4 = 5, /* usually: stone wall, red roof, trapezial ornament at the door. (there are exceptions in the city of Thorwal) */ /* dark purple */
	TOWN_TILE_WATER = 6,
	TOWN_TILE_GRASS = 7,
	TOWN_TILE_SIGNPOST = 8,
	TOWN_TILE_INN_OR_TAVERN = 9,
	TOWN_TILE_MERCHANT = 10, /* The lighthouse on Runin is also displayed in Grey */
	TOWN_TILE_SMITH = 11,
	TOWN_TILE_HEALER = 12,
	TOWN_TILE_BLACK_FINGER = 13 /* The black tower in the city of Thorwal */
};

enum {
	DUNGEON_ID_NONE = 0,
	DUNGEON_ID_TOTENSCHIFF = 1, /* random encounter in high seas */ /* wood */
	DUNGEON_ID_VERFALLENE_HERBERGE = 2, /* Ottarje-Daspota */ /* marble */
	DUNGEON_ID_SPINNENHOEHLE = 3, /* Ottarje-Skjal */ /* stone */
	DUNGEON_ID_WOLFSHOEHLE = 4, /* Ottarje-Orvil */ /* stone */
	DUNGEON_ID_GOBLINHOEHLE = 5, /* Felsteyn-Orkanger */ /* stone */
	DUNGEON_ID_DASPOTASCHATZ = 6, /* Rybon-Thoss */ /* stone */
	DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS = 7, /* Rybon-Thoss */ /* marble */
	DUNGEON_ID_ORKBEHAUSUNG = 8, /* Phexcaer-Skelellen */ /* stone */
	DUNGEON_ID_KULTSTAETTE_DES_NAMENLOSEN = 9, /* on island Hjalland */ /* marble */
	DUNGEON_ID_DRACHENHORT = 10, /* on island Runin */ /* stone */
	DUNGEON_ID_PIRATENHOEHLE = 11, /* on island Manrek */ /* marble */
	DUNGEON_ID_ZWERGENFESTE = 12, /* in town Oberorken */ /* marble */
	DUNGEON_ID_VERLASSENE_MINE = 13, /* in town Prem */ /* marble */
	DUNGEON_ID_ZWINGFESTE = 14, /* in town Thorwal */ /* marble */
	DUNGEON_ID_HYGGELIKS_RUINE = 15 /* Phexcaer-Vilnheim / Phexcaer-Skelellen */ /* marble */
};

enum {
	DNG_MENU_MODE_PLAIN = 0,
	DNG_MENU_MODE_OPEN_DOOR = 1,
	DNG_MENU_MODE_OPEN_CHEST = 2,
	DNG_MENU_MODE_CLOSE_DOOR = 3,
	DNG_MENU_MODE_LEVER = 4,
	DNG_MENU_MODE_UNLOCK_DOOR = 5
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/DNG */
	DNG_TILE_CORRIDOR = 0,
	DNG_TILE_CLOSED_DOOR = 1, /* flag 1: 0 = 'locked' / 1 = 'unlocked' */
	DNG_TILE_OPEN_DOOR = 2,
	DNG_TILE_STAIR_DOWN = 3,
	DNG_TILE_STAIR_UP = 4,
	DNG_TILE_PIT_IN_CEILING = 5,
	DNG_TILE_PIT = 6,
	DNG_TILE_BLOCKER = 7, /* what is this? */
	DNG_TILE_CHEST = 8, /* flag 1: 0 = 'closed' / 1 = 'opened' */
	DNG_TILE_SMASHED_DOOR = 9,
	DNG_TILE_REMOVABLE_WALL = 10,
	DNG_TILE_SEMIPERMEABLE_WALL = 11, /* 4 flags for the 4 directions. 0 = 'blocked', 1 = 'accessible' */
	DNG_TILE_FAKE_WALL = 12,
	DNG_TILE_WALL = 15,
};

enum {
	MAP_TILE_BLACK = 0,
	MAP_TILE_RED = 1,
	MAP_TILE_BRIGHT_GREEN = 2,
	MAP_TILE_BLUE = 3,
	MAP_TILE_YELLOW_ARROW = 4,
	MAP_TILE_PINK = 5,
	MAP_TILE_LIGHT_BLUE = 6,
	MAP_TILE_CROSS = 7,
	MAP_TILE_LIGHT_PURPLE = 8, /* unused? */
	MAP_TILE_DARK_GREEN = 9,
	MAP_TILE_DARK_PURPLE = 10,
	MAP_TILE_DARK_RED = 11,
	MAP_TILE_ORANGE = 12,
	MAP_TILE_DARK_PINK = 13, /* unused? */
	MAP_TILE_OLIVE_GREEN = 14, /* unused? */
	MAP_TILE_GREY = 15,
	MAP_TILE_PURPLE_ARROW = 16,
	MAP_TILE_BROWN = 17,
	MAP_TILE_GREEN = 18,
	MAP_TILE_DARK_GREY = 19,
	MAP_TILE_DARK_BLUE = 20, /* unused? */
	MAP_TILE_DARK_BROWN = 21 /* unused? */
};

enum {
	LROUTE_ID_THORWAL_VAERMHAG         =  1,
	LROUTE_ID_VAERMHAG_VARNHEIM        =  2,
	LROUTE_ID_VARNHEIM_DASPOTA         =  3,
	LROUTE_ID_THORWAL_SERSKE           =  4,
	LROUTE_ID_SERSKE_MERSKE            =  5,
	LROUTE_ID_MERSKE_EFFERDUN          =  6,
	LROUTE_ID_SERSKE_BREIDA            =  7,
	LROUTE_ID_F_TJOILA_TJOILA          =  8,
	LROUTE_ID_TJOILA_BREIDA            =  9,
	LROUTE_ID_BREIDA_PEILINEN          = 10,
	LROUTE_ID_PEILINEN_ROVAMUND        = 11,
	LROUTE_ID_ROVAMUND_NORDVEST        = 12,
	LROUTE_ID_NORDVEST_KRAVIK          = 13,
	LROUTE_ID_KRAVIK_SKELELLE          = 14,
	LROUTE_ID_THORWAL_F_TJOILA         = 15,
	LROUTE_ID_F_TJOILA_RUKIAN          = 16,
	LROUTE_ID_RUKIAN_F_ANGBOD          = 17,
	LROUTE_ID_F_ANGBOD_AUPLOG          = 18,
	LROUTE_ID_F_ANGBOD_ANGBODIR        = 19,
	LROUTE_ID_AUPLOG_VILNHEIM          = 20,
	LROUTE_ID_VILNHEIM_BODON           = 21,
	LROUTE_ID_VILNHEIM_PHEXCAER        = 22,
	LROUTE_ID_PHEXCAER_GROENVEL        = 23,
	LROUTE_ID_PHEXCAER_EINSIEDL        = 24,
	LROUTE_ID_VARNHEIM_AUPLOG          = 25,
	LROUTE_ID_DASPOTA_RYBON            = 26,
	LROUTE_ID_DASPOTA_OTTARJE          = 27,
	LROUTE_ID_OTTARJE_SKJAL            = 28,
	LROUTE_ID_SKJAL_PREM               = 29,
	LROUTE_ID_PREM_KORD                = 30,
	LROUTE_ID_OTTARJE_ORVIL            = 31,
	LROUTE_ID_ORVIL_ALA                = 32,
	LROUTE_ID_ALA_TJANSET              = 33,
	LROUTE_ID_TJANSET_LISKOR           = 34,
	LROUTE_ID_LISKOR_CLANEGH           = 35,
	LROUTE_ID_ALA_THOSS                = 36,
	LROUTE_ID_THOSS_LISKOR             = 37,
	LROUTE_ID_TJANSET_THOSS            = 38,
	LROUTE_ID_VILNHEIM_OBERORKE        = 39,
	LROUTE_ID_OBERORKE_FELSTEYN        = 40,
	LROUTE_ID_FELSTEYN_ORKANGER        = 41,
	LROUTE_ID_ORKANGER_CLANEGH         = 42,
	LROUTE_ID_CLANEGH_TYLDON           = 43,
	LROUTE_ID_TYLDON_VIDSAND           = 44,
	LROUTE_ID_OBERORKE_EINSIEDL        = 45,
	LROUTE_ID_FELSTEYN_EINSIEDL        = 46,
	LROUTE_ID_RYBON_THOSS              = 47,
	LROUTE_ID_SKJAL_ORVIL              = 48,
	LROUTE_ID_SKELELLE_PHEXCAER        = 49,
	LROUTE_ID_MERSKE_ROVAMUND          = 50,
	LROUTE_ID_ORVIL_ROVIK              = 51,
	LROUTE_ID_LJASDAHL__CIRCULAR       = 52, // circular route
	LROUTE_ID_LJASDAHL_HJALLA_H        = 53,
	LROUTE_ID_RUNINSHA_L_RUNIN__1      = 54, // not linked at LEUCHTTURM_RUNIN // TODO: rename to __WEST or __EAST
	LROUTE_ID_RUNINSHA_L_RUNIN__MIDDLE = 55, // middle route; the only route linked at LEUCHTTURM_RUNIN
	LROUTE_ID_BRENDHIL_MANRIN          = 56,
	LROUTE_ID_EINSIEDL__CIRCULAR       = 57, // circular route
	LROUTE_ID_L_RUNIN_RUNINSHA__2      = 58, // not linked at LEUCHTTURM_RUNIN // TODO: rename to __WEST or __EAST
	LROUTE_ID__CROSSLINK               = 59, // A crosslink between LROUTE_ID_KRAVIK_SKELELLE and LROUTE_ID_PEILINEN_ROVAMUND
	LROUTE_ID__END                     = 60
};

enum {
	/* High seas routes */
	SEA_ROUTE_ID_THORWAL_PREM      =  1,
	SEA_ROUTE_ID_PREM_HJALSING     =  2,
	SEA_ROUTE_ID_PREM_MANRIN       =  3,
	SEA_ROUTE_ID_PREM_KORD         =  4,
	SEA_ROUTE_ID_KORD_HJALSING     =  5,
	SEA_ROUTE_ID_HJALSING_OVERTHOR =  6,
	SEA_ROUTE_ID_HJALSING_MANRIN   =  7,
	/* Costal routes */
	SEA_ROUTE_ID_THORWAL_VAERMHAG  =  8,
	SEA_ROUTE_ID_VAERMHAG_VARNHEIM =  9,
	SEA_ROUTE_ID_VARNHEIM_LJASDAHL = 10,
	SEA_ROUTE_ID_VARNHEIM_OTTARJE  = 11,
	SEA_ROUTE_ID_LJASDAHL_OTTARJE  = 12,
	SEA_ROUTE_ID_SKJAL_OTTARJE     = 13,
	SEA_ROUTE_ID_SKJAL_PREM        = 14,
	SEA_ROUTE_ID_PREM_ARYN         = 15,
	SEA_ROUTE_ID_PREM_RUNINSHA     = 16,
	SEA_ROUTE_ID_ARYN_RUNINSHA     = 17,
	SEA_ROUTE_ID_RUNINSHA_TREBAN   = 18,
	SEA_ROUTE_ID_TREBAN_KORD       = 19,
	SEA_ROUTE_ID_KORD_GUDDASUN     = 20,
	SEA_ROUTE_ID_GUDDASUN_HJALSING = 21,
	SEA_ROUTE_ID_HJALSING_ROVIK    = 22,
	SEA_ROUTE_ID_ROVIK_OVERTHOR    = 23,
	SEA_ROUTE_ID_ROVIK_ORVIL       = 24,
	SEA_ROUTE_ID_OVERTHOR_TJANSET  = 25,
	SEA_ROUTE_ID_OVERTHOR_VIDSAND  = 26,
	SEA_ROUTE_ID_TJANSET_LISKOR    = 27,
	SEA_ROUTE_ID_LISKOR_VIDSAND    = 28,
	SEA_ROUTE_ID_TJANSET_VIDSAND   = 29,
	SEA_ROUTE_ID_OVERTHOR_BRENDHIL = 30,
	SEA_ROUTE_ID_BRENDHIL_MANRIN   = 31,
	SEA_ROUTE_ID_ROVIK_MANRIN      = 32,
	SEA_ROUTE_ID_THORWAL_MERSKE    = 33,
	SEA_ROUTE_ID_MERSKE_EFFERDUN   = 34,
	SEA_ROUTE_ID_THORWAL_EFFERDUN  = 35,
	SEA_ROUTE_ID_SERSKE_MERSKE     = 36,
	SEA_ROUTE_ID_SERSKE_EFFERDUN   = 37,
	SEA_ROUTE_ID_OVERTHOR_LISKOR   = 38,
	SEA_ROUTE_ID_THORWAL_VARNHEIM  = 39,
	SEA_ROUTE_ID_OTTARJE_PREM      = 40,
	SEA_ROUTE_ID_OTTARJE_RUNINSHA  = 41,
	SEA_ROUTE_ID_SKJAL_RUNINSHA    = 42,
	SEA_ROUTE_ID_RUNINSHA_L_RUNIN  = 43,
	SEA_ROUTE_ID_TREBAN_L_RUNIN    = 44,
	SEA_ROUTE_ID_MANRIN_OVERTHOR   = 45,
	SEA_ROUTE_ID__END              = 46
};

enum {
	JOURNEY_DIRECTION_CHANGE_TO_FORWARD = -1,
	JOURNEY_DIRECTION_FORWARD = 0,
	JOURNEY_DIRECTION_CHANGE_TO_BACKWARD = 1,
	JOURNEY_DIRECTION_BACKWARD = 2
};

enum {
	CROSSLINK_STATUS_NONE = 0,
	CROSSLINK_STATUS_FROM_KRAVIK = 1,
	CROSSLINK_STATUS_FROM_PEILINEN = 2,
	CROSSLINK_STATUS_FROM_SKELELLEN = 3,
	CROSSLINK_STATUS_FROM_ROVAMUND = 4
};

enum {
	ARCHIVE_FILE_PLAYM_UK		= 0x00,
	ARCHIVE_FILE_KDBACK_DAT		= 0x01,
	ARCHIVE_FILE_KCBACK_DAT		= 0x02,
	ARCHIVE_FILE_KLBACK_DAT		= 0x03,
	ARCHIVE_FILE_KSBACK_DAT		= 0x04,
	ARCHIVE_FILE_KARTE_DAT		= 0x05,
	ARCHIVE_FILE_BICONS 		= 0x06,
	ARCHIVE_FILE_OBJECTS_NVF	= 0x07,
	ARCHIVE_FILE_LROUT_DAT		= 0x08,
	ARCHIVE_FILE_SROUT_DAT		= 0x09,
	ARCHIVE_FILE_HSROUT_DAT		= 0x0a,
	ARCHIVE_FILE_FONT6 		= 0x0b,
	ARCHIVE_FILE_FONT8 		= 0x0c,
	ARCHIVE_FILE_GGSTS_NVF		= 0x0d,
	ARCHIVE_FILE_TEXT_LTX		= 0x0e,
	ARCHIVE_FILE_ICONS 		= 0x0f,
	ARCHIVE_FILE_MONSTER		= 0x10,
	ARCHIVE_FILE_MONSTER_TAB	= 0x11,
	ARCHIVE_FILE_COMPASS		= 0x12,
	ARCHIVE_FILE_MAPTEXT_LTX	= 0x13,
	ARCHIVE_FILE_ZUSTA_UK		= 0x14,
	ARCHIVE_FILE_FIGHTTXT_LTX	= 0x15,
	ARCHIVE_FILE_ANIS 		= 0x16,
	ARCHIVE_FILE_ANIS_TAB		= 0x17,
	ARCHIVE_FILE_MFIGS 		= 0x18,
	ARCHIVE_FILE_MFIGS_TAB		= 0x19,
	ARCHIVE_FILE_CITY_DAT		= 0x1a,
	/*
		THORWAL.DAT, SERSKE.DAT, BREIDA.DAT, PEILINEN.DAT, ROVAMUND.DAT, NORDVEST.DAT,
		KRAVIK.DAT, SKELELLE.DAT, MERSKE.DAT, EFFERDUN.DAT, TJOILA.DAT, RUKIAN.DAT,
		ANGBODIRTAL.DAT, AUPLOG.DAT, VILNHEIM.DAT, BODON.DAT, OBERORKEN.DAT,
		PHEXCAER.DAT, GROENVEL.DAT, FELSTEYN.DAT, EINSIEDL.DAT, ORKANGER.DAT,
		CLANEGH.DAT, LISKOR.DAT, THOSS.DAT, TJANSET.DAT, ALA.DAT, ORVIL.DAT,
		OVERTHORN.DAT, ROVIK.DAT, HJALSING.DAT, GUDDASUN.DAT, KORD.DAT, TREBAN.DAT,
		ARYN.DAT, RUNINSHA.DAT, OTTARJE.DAT, SKJAL.DAT, PREM.DAT, DASPOTA.DAT,
		RYBON.DAT, LJASDAHL.DAT, VARNHEIM.DAT, VAERMHAG.DAT, TYLDON.DAT, VIDSAND.DAT,
		BRENDHIL.DAT, MANRIN.DAT, FTJOILA.DAT, FANGBODI.DAT, HJALLAND.DAT, RUNIN.DAT
	*/
	ARCHIVE_FILE_CITY_LTX		= 0x4d,
	/*
		THORWAL_LTX, SERSKE_LTX, BREIDA_LTX, PEILINEN_LTX, ROVAMUND_LTX, NORDVEST_LTX,
		KRAVIK_LTX, SKELELLE_LTX, MERSKE_LTX, EFFERDUN_LTX, TJOILA_LTX, RUKIAN_LTX,
		ANGBODIR_LTX, AUPLOG_LTX, VILNHEIM_LTX, BODON_LTX, OBERORKE_LTX, PHEXCAER_LTX,
		GROENVEL_LTX, FELSTEYN_LTX, EINSIEDL_LTX, ORKANGER_LTX, CLANEGH_LTX, LISKOR_LTX,
		THOSS_LTX, TJANSET_LTX, ALA_LTX, ORVIL_LTX, OVERTHOR_LTX, ROVIK_LTX,
		HJALSING_LTX, GUDDASUN_LTX, KORD_LTX, TREBAN_LTX, ARYN_LTX, RUNINSHA_LTX,
		OTTARJE_LTX, SKJAL_LTX, PREM_LTX, DASPOTA_LTX, RYBON_LTX, LJASDAHL_LTX,
		VARNHEIM_LTX, VAERMHAG_LTX, TYLDON_LTX, VIDSAND_LTX, BRENDHIL_LTX, MANRIN_LTX,
		F_TJOILA_LTX, F_ANGBOD_LTX, HJALLAND_LTX, L_RUNIN_LTX
	*/
	ARCHIVE_FILE_TAVERN_TLK		= 0x82,
	ARCHIVE_FILE_SPSTAR_NVF		= 0x83,
	ARCHIVE_FILE_GUERTEL_NVF	= 0x84,
	ARCHIVE_FILE_HAFEN_LTX		= 0x85,
	ARCHIVE_FILE_FACE_NVF		= 0x86,
	ARCHIVE_FILE_HYGBACK_NVF	= 0x87,
	ARCHIVE_FILE_HYGGELIK_NVF	= 0x88,
	ARCHIVE_FILE_SKULL_NVF		= 0x89,
	ARCHIVE_FILE_OUTRO1_NVF		= 0x8a,
	ARCHIVE_FILE_OUTRO2_NVF		= 0x8b,
	ARCHIVE_FILE_OUTRO3_NVF		= 0x8c,
	ARCHIVE_FILE_TOWNPAL_DAT	= 0x8d,
	ARCHIVE_FILE_THORWAL_XMI	= 0x8e,
	ARCHIVE_FILE_SUMMARY_XMI	= 0x8f,
	ARCHIVE_FILE_TEMPLE_XMI		= 0x90,
	ARCHIVE_FILE_TERMS_XMI		= 0x91,
	ARCHIVE_FILE_SMITH_XMI		= 0x92,
	ARCHIVE_FILE_INN_XMI		= 0x93,
	ARCHIVE_FILE_HEALER_XMI		= 0x94,
	ARCHIVE_FILE_CAMP_XMI		= 0x95,
	ARCHIVE_FILE_VICTORY_XMI	= 0x96,
	ARCHIVE_FILE_SAMPLE_AD		= 0x97,
	ARCHIVE_FILE_ATTIC		= 0x98,
	ARCHIVE_FILE_POPUP_DAT		= 0x99,
	ARCHIVE_FILE_154		= 0x9a,
	ARCHIVE_FILE_155		= 0x9b,
	ARCHIVE_FILE_DIALOGS_TLK	= 0x9c,
	/*
		F092.TLK, HERBERG.TLK, 158, DASPOTA1.TLK, DASPOTA2.TLK, DASPOTA3.TLK,
		INFO1.TLK, INFO2.TLK, INFO3.TLK, INFO4.TLK, INFO5.TLK, INFO6.TLK,
		INFO7.TLK, SCHMIED.TLK, GHANDEL.TLK, KHANDEL.TLK, WHANDEL.TLK, F115.TLK,
		EREMIT.TLK, HYGGELIK.TLK
	*/
	ARCHIVE_FILE_WEAPONS_NVF	= 0xb0,
	ARCHIVE_FILE_BUCH_DAT		= 0xb1,
	ARCHIVE_FILE_SPELLOBJ_NVF	= 0xb2,
	ARCHIVE_FILE_IN_HEADS_NVF	= 0xb3,
	ARCHIVE_FILE_TEMPICON		= 0xb4,
	ARCHIVE_FILE_SHIPSL_NVF		= 0xb5,
	ARCHIVE_FILE_STONESL_NVF	= 0xb6,
	ARCHIVE_FILE_MARBLESL_NVF	= 0xb7,
	ARCHIVE_FILE_TFLOOR1_NVF	= 0xb8,
	ARCHIVE_FILE_TDIVERSE_NVF	= 0xb9,
	ARCHIVE_FILE_HOUSE1_NVF		= 0xba,
	ARCHIVE_FILE_HOUSE2_NVF		= 0xbb,
	ARCHIVE_FILE_HOUSE3_NVF		= 0xbc,
	ARCHIVE_FILE_HOUSE4_NVF		= 0xbd,
	ARCHIVE_FILE_FEATURE_LTX	= 0xbe,
	ARCHIVE_FILE_FEATURE1_LTX	= 0xbf,
	ARCHIVE_FILE_FEATURE2_LTX	= 0xc0,
	ARCHIVE_FILE_FEATURE3_LTX	= 0xc1,
	ARCHIVE_FILE_FEATURE4_LTX	= 0xc2,
	ARCHIVE_FILE_FEATURE5_LTX	= 0xc3,
	ARCHIVE_FILE_FEATURE6_LTX	= 0xc4,
	ARCHIVE_FILE_FEATURE7_LTX	= 0xc5,
	ARCHIVE_FILE_FEATURE8_LTX	= 0xc6,
	ARCHIVE_FILE_FIGHTOBJ_NVF	= 0xc7,
	ARCHIVE_FILE_SCENARIO_LST	= 0xc8,
	ARCHIVE_FILE_KDLBACK_DAT	= 0xc9,
	ARCHIVE_FILE_KCLBACK_DAT	= 0xca,
	ARCHIVE_FILE_KLLBACK_DAT	= 0xcb,
	ARCHIVE_FILE_KSLBACK_DAT	= 0xcc,
	ARCHIVE_FILE_FIGHT_LST		= 0xcd,
	ARCHIVE_FILE_SKARTE_NVF		= 0xce,
	ARCHIVE_FILE_GAMES_NAM		= 0xcf,
	ARCHIVE_FILE_ITEMNAME		= 0xd0,
	ARCHIVE_FILE_MONNAMES		= 0xd1,
	ARCHIVE_FILE_THORWAL1_LTX	= 0xd2,
	ARCHIVE_FILE_THORWAL2_LTX	= 0xd3,
	ARCHIVE_FILE_DUNGEON_XMI	= 0xd4,
	ARCHIVE_FILE_COMBAT_XMI		= 0xd5,
	ARCHIVE_FILE_PLAYM_US		= 0xd6,
	ARCHIVE_FILE_ZUSTA_US		= 0xd7,
	ARCHIVE_FILE_ANI_DAT		= 0xd8,
	ARCHIVE_FILE_WEAPANI_DAT	= 0xd9,
	ARCHIVE_FILE_MONSTER_DAT	= 0xda,
	ARCHIVE_FILE_BSKILLS_DAT	= 0xdb,
	ARCHIVE_FILE_ITEMS_DAT		= 0xdc,
	ARCHIVE_FILE_CHARTEXT_LTX	= 0xdd,
	ARCHIVE_FILE_SPELLTXT_LTX	= 0xde,
	ARCHIVE_FILE_WONDER_LTX		= 0xdf,
	ARCHIVE_FILE_STRASSE_LTX	= 0xe0,
	ARCHIVE_FILE_NSC_LTX		= 0xe1,
	ARCHIVE_FILE_NPCS		= 0xe2,
	/*
		NARIELL.NPC, HARIKA.NPC, CURIAN.NPC, ARDORA.NPC, GARSVIK.NPC, ERWO.NPC
	*/
	ARCHIVE_FILE_FINGER_NVF		= 0xe8,
	ARCHIVE_FILE_LTURM_NVF		= 0xe9,
	ARCHIVE_FILE_WFIGS		= 0xea,
	ARCHIVE_FILE_WFIGS_TAB		= 0xeb,
	ARCHIVE_FILE_SPLASHES_DAT	= 0xec,
	ARCHIVE_FILE_FEATURE9_LTX	= 0xed,
	ARCHIVE_FILE_PHEX1_LTX		= 0xee,
	ARCHIVE_FILE_PHEX2_LTX		= 0xef,
	ARCHIVE_FILE_WILD_LTX		= 0xf0,
	ARCHIVE_FILE_WAFFINFO_LTX	= 0xf1,
	ARCHIVE_FILE_DNGS		= 0xf2,
	/*
		SHIP.DNG, F046.DNG, F051.DNG, F061.DNG, F076.DNG, F094.DNG, F100.DNG, F108.DNG,
		F126.DNG, F129.DNG, F131.DNG, OBER.DNG, PREM.DNG, THORWAL.DNG, FINAL.DNG
	*/
	ARCHIVE_FILE_DNGS_DDT		= 0x100,
	/*
		SHIP.DDT, F046.DDT, F051.DDT, F061.DDT, F076.DDT, F094.DDT, F100.DDT, F108.DDT,
		F126.DDT, F129.DDT, F131.DDT, OBER.DDT, PREM.DDT, THORWAL.DDT, FINAL.DDT
	*/
	ARCHIVE_FILE_DNGS_DTX		= 0x10f,
	/*
		SHIP.DTX, F046.DTX, F051.DTX, F061.DTX, F076.DTX, F094.DTX, F100.DTX, F108.DTX,
		F126.DTX, F129.DTX, F131.DTX, OBER.DTX, PREM.DTX, THORWAL.DTX, FINAL.DTX
	*/
	ARCHIVE_FILE_TFLOOR2_NVF	= 0x11f,
	ARCHIVE_FILE_STORY_LTX		= 0x120,
	ARCHIVE_FILE_FX1_VOC		= 0x121,
	ARCHIVE_FILE_FX2_VOC		= 0x122,
	ARCHIVE_FILE_FX3_VOC		= 0x123,
	ARCHIVE_FILE_FX4_VOC		= 0x124,
	ARCHIVE_FILE_FX5_VOC		= 0x125,
	ARCHIVE_FILE_FX6_VOC		= 0x126,
	ARCHIVE_FILE_FX7_VOC		= 0x127,
	ARCHIVE_FILE_FX8_VOC		= 0x128,
	ARCHIVE_FILE_FX9_VOC		= 0x129,
	ARCHIVE_FILE_FX10_VOC		= 0x12a,
	ARCHIVE_FILE_FX11_VOC		= 0x12b,
	ARCHIVE_FILE_FX12_VOC		= 0x12c,
	ARCHIVE_FILE_FX13_VOC		= 0x12d,
	ARCHIVE_FILE_FX14_VOC		= 0x12e,
	ARCHIVE_FILE_FX15_VOC		= 0x12f,
	ARCHIVE_FILE_FX16_VOC		= 0x130,
	ARCHIVE_FILE_FX17_VOC		= 0x131,
	ARCHIVE_FILE_FX18_VOC		= 0x132,
	ARCHIVE_FILE_OUTRO_XMI		= 0x133
};

enum {
	ITEM_NONE				= 0x00, /* nothing / Bloße Hand (bare hand) */
	ITEM_SCHWERT				= 0x01, /* sword */
	ITEM_KNUEPPEL				= 0x02, /* club */
	ITEM_SAEBEL				= 0x03, /* saber */
	ITEM_MESSER				= 0x04, /* knife */
	ITEM_SPEER				= 0x05, /* spear */
	ITEM_KURZSCHWERT			= 0x06, /* short sword */
	ITEM_SCHILD				= 0x07, /* shield */
	ITEM_KRIEGSBEIL__SPECIAL		= 0x08, /* hatchet [special "Schicksalsklinge" variant, also for witches, druids and mages, 1D6 + 4, KK+14, BF 5, AT-0, PA-3, weight 120, price 45] */
	ITEM_KURZBOGEN				= 0x09, /* shortbow */
	ITEM_PFEIL				= 0x0a, /* arrow */
	ITEM_STREITAXT				= 0x0b, /* battleaxe */
	ITEM_ARMBRUST				= 0x0c, /* crossbow */
	ITEM_BOLZEN				= 0x0d, /* bolt */
	ITEM_DOLCH				= 0x0e, /* dagger */
	ITEM_IRON_SHIELD			= 0x0f, /* iron shield */
	ITEM_WURFBEIL				= 0x10, /* francesca */
	ITEM_WURFSTERN				= 0x11, /* throwing star */
	ITEM_ZWEIHAENDER			= 0x12, /* two-handed sword */
	ITEM_LANGBOGEN				= 0x13, /* longbow */
	ITEM_MORGENSTERN			= 0x14, /* morning star */
	ITEM_VULKANGLASDOLCH			= 0x15, /* volcanic glass dagger */
	ITEM_FACKEL__LIT			= 0x16, /* torch [lit] */
	ITEM_BIER				= 0x17, /* beer */
	ITEM_WURFHAKEN				= 0x18, /* grapple */
	ITEM_LATERNE__UNLIT			= 0x19, /* lantern [unlit] */
	ITEM_BRECHEISEN				= 0x1a, /* crowbar */
	ITEM_HAMMER				= 0x1b, /* hammer */
	ITEM_ANGELHAKEN				= 0x1c, /* fishhook */
	ITEM_SCHRIFTROLLE			= 0x1d, /* scroll */
	ITEM_WASSERSCHLAUCH			= 0x1e, /* waterskin */
	ITEM_GLASFLASCHE			= 0x1f, /* flask glass */
	ITEM_STRICKLEITER			= 0x20, /* rope ladder */
	ITEM_WURFAXT				= 0x21, /* throwing axe */
	ITEM_MESSINGSPIEGEL			= 0x22, /* brass mirror */
	ITEM_DIETRICHE				= 0x23, /* picklocks */
	ITEM_SCHREIBZEUG			= 0x24, /* writing material */
	ITEM_HARFE				= 0x25, /* harp */
	ITEM_TRINKHORN				= 0x26, /* drinking horn */
	ITEM_SILBERSCHMUCK			= 0x27, /* silver jewelry */
	ITEM_KLETTERHAKEN			= 0x28, /* climbing hooks */
	ITEM_OEL				= 0x29, /* oil */
	ITEM_BRONZEFLASCHE			= 0x2a, /* flask bronze */
	ITEM_EISENHELM				= 0x2b, /* iron helmet */
	ITEM_PIKE				= 0x2c, /* pike */
	ITEM_PROVIANTPAKET			= 0x2d, /* food package */
	ITEM_FLOETE				= 0x2e, /* flute */
	ITEM_ALCHIMIESET			= 0x2f, /* alchemy kit */
	ITEM_HEMD				= 0x30, /* shirt */
	ITEM_HOSE				= 0x31, /* trousers */
	ITEM_SCHUHE				= 0x32, /* shoes */
	ITEM_STIEFEL				= 0x33, /* boots */
	ITEM_SCHNEESCHUHE			= 0x34, /* laceup shoe */
	ITEM_LEDERHARNISCH			= 0x35, /* leather armor */
	ITEM_SCHUPPENRUESTUNG			= 0x36, /* scale armor */
	ITEM_SHURINKNOLLEKNOLLENGIFT		= 0x37, /* Shurin poison */
	ITEM_ARAXGIFT				= 0x38, /* Arax poison */
	ITEM_ANGSTGIFT				= 0x39, /* angst gift */
	ITEM_SCHLAFGIFT				= 0x3a, /* sleep poison */
	ITEM_GOLDLEIM				= 0x3b, /* Goldleim */
	ITEM_EINBEERE				= 0x3c, /* Einbeere */
	ITEM_WIRSELKRAUT			= 0x3d, /* whirlweed */
	ITEM_EITRIGER_KROETENSCHEMEL		= 0x3e, /* Eitriger Krötenschemel */
	ITEM_GULMOND_BLATT			= 0x3f, /* Gulmond leaf */
	ITEM_TARNELE				= 0x40, /* Tarnele */
	ITEM_FACKEL__UNLIT			= 0x41, /* torch [unlit] */
	ITEM_STREITKOLBEN			= 0x42, /* mace */
	ITEM_DEGEN				= 0x43, /* epee */
	ITEM_FLORETT				= 0x44, /* floret */
	ITEM_KAMPFSTAB				= 0x45, /* quarterstaff */
	ITEM_KRISTALLKUGEL			= 0x46, /* crystal ball */
	ITEM_PEITSCHE				= 0x47, /* whip */
	ITEM_DECKE				= 0x48, /* blanket */
	ITEM_SCHAUFEL				= 0x49, /* shovel */
	ITEM_GOLDSCHMUCK			= 0x4a, /* gold jewelry */
	ITEM_ROBE__GREEN_1			= 0x4b, /* robe [green, cheap] */
	ITEM_ROBE__RED				= 0x4c, /* robe [red, expensive] */
	ITEM_TOPFHELM				= 0x4d, /* pot helmet */
	ITEM_LEDERHELM				= 0x4e, /* leather helmet */
	ITEM_WAFFENROCK				= 0x4f, /* surcoat */
	ITEM_KETTENHEMD				= 0x50, /* chain mail */
	ITEM_KROETENHAUT			= 0x51, /* toad skin */
	ITEM_PLATTENZEUG			= 0x52, /* plate gear */
	ITEM_KETTENZEUG				= 0x53, /* chain gear */
	ITEM_LEDERZEUG				= 0x54, /* leather gear */
	ITEM_ZUNDERKAESTCHEN			= 0x55, /* tinderbox */
	ITEM_SCHLEIFSTEIN			= 0x56, /* grindstone */
	ITEM_ESSBESTECK				= 0x57, /* cutlery */
	ITEM_ESSGESCHIRR			= 0x58, /* dishes */
	ITEM_LAKRITZE				= 0x59, /* licorice */
	ITEM_BONBONS				= 0x5a, /* bonbons */
	ITEM_WEINFLASCHE			= 0x5b, /* wine */
	ITEM_SCHNAPSFLASCHE			= 0x5c, /* brandy */
	ITEM_HACKE				= 0x5d, /* pickaxe */
	ITEM_PRAIOS_AMULETT			= 0x5e, /* Praios amulet [magic; found in Spinnenhoehle] */
	ITEM_LAUTE				= 0x5f, /* lute */
	ITEM_WINTERMANTEL			= 0x60, /* winter coat */
	ITEM_NETZ				= 0x61, /* net */
	ITEM_WURFMESSER				= 0x62, /* throwing knife */
	ITEM_SICHEL				= 0x63, /* sickle */
	ITEM_SENSE				= 0x64, /* scythe */
	ITEM_KRIEGSHAMMER			= 0x65, /* war hammer */
	ITEM_DREIZACK				= 0x66, /* trident */
	ITEM_HELLEBARDE				= 0x67, /* halberd */
	ITEM_DRESCHFLEGEL			= 0x68, /* flail */
	ITEM_ZWEILILIEN				= 0x69, /* Zweililien */
	ITEM_OCHSENHERDE			= 0x6a, /* oxen herd */
	ITEM_BASILISKENZUNGE			= 0x6b, /* basilisk tongue */
	ITEM_OGERFAENGER			= 0x6c, /* ogre catcher */
	ITEM_MENGBILAR				= 0x6d, /* Mengbilar */
	ITEM_SCHWERER_DOLCH			= 0x6e, /* heavy dagger */
	ITEM_RONDRAKAMM				= 0x6f, /* Rondra comb */
	ITEM_ENTERMESSER			= 0x70, /* cutlass */
	ITEM_BASTARDSCHWERT			= 0x71, /* bastard sword */
	ITEM_TUZAKMESSER			= 0x72, /* Tuzak knife */
	ITEM_RABENSCHNABEL			= 0x73, /* raven's beak */
	ITEM_BRABAKBENGEL			= 0x74, /* Brabakbengel */
	ITEM_RAPIER				= 0x75, /* rapier */
	ITEM_KHUNCHOMER				= 0x76, /* Khunchomer */
	ITEM_DOPPELKHUNCHOMER			= 0x77, /* double Khunchomer */
	ITEM_SCHLEUDER				= 0x78, /* sling */
	ITEM_SEIL				= 0x79, /* rope */
	ITEM_SHURINKNOLLE			= 0x7a, /* Shurink bulb */
	ITEM_BELMART_BLATT			= 0x7b, /* Belmart leaf */
	ITEM_DONFSTENGEL			= 0x7c, /* Donf spring */
	ITEM_MENCHALKAKTUS			= 0x7d, /* Menchal cactus */
	ITEM_ALRAUNE				= 0x7e, /* mandrake */
	ITEM_ATMONBLUETE			= 0x7f, /* Atmon bloom */
	ITEM_ILMENBLATT				= 0x80, /* Ilmen leaf */
	ITEM_FINAGEBAEMCHEN			= 0x81, /* Finage tree */
	ITEM_JORUGAWURZEL			= 0x82, /* Joruga root */
	ITEM_THONNYSBLUETE			= 0x83, /* Thonnys bloom */
	ITEM_LOTUSBLUETE			= 0x84, /* Lotus bloom */
	ITEM_ZAUBERSTAB				= 0x85, /* magic wand */
	ITEM_SKRAJA				= 0x86, /* Skraja */
	ITEM_KRIEGSBEIL				= 0x87, /* hatchet [the common one, 1D6 + 4, KK+13, BF 2, AT-2, PA-4] */
	ITEM_ORKNASE				= 0x88, /* orc nose */
	ITEM_SCHNEIDZAHN			= 0x89, /* cutting tooth */
	ITEM_ROBBENTOETER			= 0x8a, /* seal killer */
	ITEM_WOLFSMESSER			= 0x8b, /* wolf knive */
	ITEM_HEXENBESEN				= 0x8c, /* witches broom */
	ITEM_LOTUSGIFT				= 0x8d, /* Lotus poison */
	ITEM_KUKRIS				= 0x8e, /* Kukris */
	ITEM_BANNSTAUB				= 0x8f, /* Bannstaub */
	ITEM_KROETENSCHEMELGIFT			= 0x90, /* Krötenschemel poison */
	ITEM_HEILTRANK				= 0x91, /* LE potion */
	ITEM_STARKER_HEILTRANK			= 0x92, /* strong LE potion */
	ITEM_MU_ELIXIER				= 0x93, /* MU Elixier */
	ITEM_KL_ELIXIER				= 0x94, /* KL Elixier */
	ITEM_CH_ELIXIER				= 0x95, /* CH Elixier */
	ITEM_FF_ELIXIER				= 0x96, /* FF Elixier */
	ITEM_GE_ELIXIER				= 0x97, /* GE Elixier */
	ITEM_IN_ELIXIER				= 0x98, /* IN Elixier */
	ITEM_KK_ELIXIER				= 0x99, /* KK Elixier */
	ITEM_ZAUBERTRANK			= 0x9a, /* AE potion */
	ITEM_ZAUBERTRANK_STARK			= 0x9b, /* strong AE potion */
	ITEM_OLGINWURZEL			= 0x9c, /* Olgin root */
	ITEM_KAIRANHALM				= 0x9d, /* Kairan stem */
	ITEM_BASTARDSCHWERT__MAGIC		= 0x9e, /* bastard sword [magic: lower BF; found in Daspota] */
	ITEM_ORKNASE__MAGIC			= 0x9f, /* orc nose [magic: damage +5, AT-1, PA-5; found in Daspota] */
	ITEM_KURZSCHWERT__MAGIC			= 0xa0, /* short sword [magic: AT + 2, unbreakable; found in Daspota ] */
	ITEM_SICHEL__MAGIC			= 0xa1, /* sickle [magic, while in inventory: Pflanzenkunde + 3; found in Daspota] */
	ITEM_AMULETT__UNKNOWN_1			= 0xa2, /* amulet [found at a corpse in tevent014. Analues: "Dient der Abwehr von Ignifaxius"]. */ /* Called "Feuerbannamulett" in https://www.crystals-dsa-foren.de/showthread.php?tid=653&pid=16411 But apparently, no effect is implemented. */
	ITEM_AMULETT__BLUE			= 0xa3, /* amulet [blue, magic: while in inventory MR + 5; found in ruin of the black wizard and at a fight vs. orcs at the track Phexcaer <-> Einsiedlersee] */
	ITEM_KNOCHEN_MIT_RUNE			= 0xa4, /* bone with rune [found in Gorah's chest] */
	ITEM_RING__RED				= 0xa5, /* ring [red, magic, MR + 2. additionally 5 charges of Arcano Psychostabilis (additionally MR + 2) when used; found in Gorah's chest ] */
	ITEM_EXPURGICUM				= 0xa6, /* Expurgicum */
	ITEM_REZEPT_FUER_EXPURGICUM		= 0xa7, /* recipe for Expurgicum */
	ITEM_VOMICUM				= 0xa8, /* Vomicum */
	ITEM_REZEPT_FUER_VOMICUM		= 0xa9, /* recipe for Vomicum */
	ITEM_DOKUMENT__UNICORN			= 0xaa, /* unicorn letter [found in tevent073] */
	ITEM_SILBERNER_STIRNREIF		= 0xab, /* silver coronet [magic, 3 charges, use -> Armatrutz +5, i.e. RS + 5] */
	ITEM_SAEBEL__MAGIC			= 0xac, /* sabre [magic, damage+1 to skeletons and zombies] */
	ITEM_AMULETT__RED			= 0xad, /* amulet [red, magic, protection from fire; found at corpse between Rovamund and Nordvest ] */
	ITEM_AMULETT__GREEN			= 0xae, /* amulet [green, magic, 6 charges: use -> Flim Flam; found in Totenschiff] */
	ITEM_TRAVIA_AMULETT			= 0xaf, /* amulet [golden cross, magic: no hunger or thirst; found in Wolfshoehle ] */
	ITEM_SCHULDBUCH				= 0xb0, /* debt register [found in Wolfshoehle] */
	ITEM_ROTE_MONDSCHEIBE			= 0xb1, /* red moon disk [found in Orkhoehle; red herring] */
	ITEM_ZWEIHAENDER__MAGIC			= 0xb2, /* two-handed sword [magic: unbreakable; found in Orkhoehle] */
	ITEM_ORKDOKUMENT			= 0xb3, /* orc document [found in Orkhoehle] */
	ITEM_GEGENGIFT				= 0xb4, /* antivenom */
	ITEM_GRIMRING				= 0xb5, /* Grimring [found at Hyggelik's ruin] */
	ITEM_TOTENKOPFGUERTEL			= 0xb6, /* skull belt [magic: TA - 4; found in Verfallene Herberge ] */
	ITEM_KRAFTGUERTEL			= 0xb7, /* girdle of might [magic: KK + 5; found in Drachenhoehle] */
	ITEM_MAGISCHER_BROTBEUTEL		= 0xb8, /* magic breadbag [cannot be found in the game] */
	ITEM_MAGISCHER_WASSERSCHLAUCH		= 0xb9, /* magic waterskin [cannot be found in the game] */
	ITEM_REZEPT_FUER_HEILTRANK		= 0xba, /* recipe for heal potion */
	ITEM_SCHREIBEN_VON_JADRA		= 0xbb, /* writing of Jarda */
	ITEM_AMULETT__UNKNOWN_2			= 0xbc, /* amulet [TODO: which one?? has the shape of a golden cross. it might be yellow, magic: protects from fire; found at Totenschiff] */
	ITEM_LOBPREISUNGEN			= 0xbd, /* praises of the nameless god [found in Tempel des Namenlosen and in Piratenhoehle] */
	ITEM_MITGLIEDERLISTE			= 0xbe, /* member list [found in Tempel des Namenlosen] */
	ITEM_DOKUMENT__2			= 0xbf, /* document [2, not readable. Depotschein?] */
	ITEM_SEEKARTE				= 0xc0, /* sea chart [found in Piratenhoehle] */
	ITEM_KUKRISDOLCH			= 0xc1, /* Kukris dagger */
	ITEM_SCHLUESSEL__1			= 0xc2, /* key [1] */ /* TODO: Farbe? umbenennen! */
	ITEM_GOLDENER_SCHLUESSEL		= 0xc3, /* golden key [found in the dungeon of the Daspota treasure] / note that there is also ITEM_GOLDSCHLUESSEL */
	ITEM_HELM__CURSED			= 0xc4, /* helmet [cursed: CH - 1; found in goblin cave and at NPC Ardora von Greifenfurt ] */
	ITEM_KETTENHEMD__CURSED			= 0xc5, /* chain mail [magic, cursed, protection +1 vs. skeletons and zombies (arising as damage-3, but protection-2), but protection -2 vs. everything else, additionally drains LE; found in Drachenhoehle] */
	ITEM_KETTENHEMD__MAGIC			= 0xc6, /* chain mail [magic, AT+1, BE-1, RS+1; found in the dungeon of the Daspota treasure (Rybon-Thoss)] */
	ITEM_SCHWERT__MAGIC			= 0xc7, /* sword [magic: AT + 2; found in Drachenhoehle] */
	ITEM_KUKRIS_MENGBILAR			= 0xc8, /* Kukris mengbilar */
	ITEM_PLATINSCHLUESSEL			= 0xc9, /* platin key */
	ITEM_REZEPT_FUER_GEGENGIFT		= 0xca, /* recipe for antivenom */
	ITEM_REZEPT_FUER_HYLAILER_FEUER		= 0xcb, /* recipe for Hylailer Feuer */
	ITEM_REZEPT_FUER_KRAFTELIXIER		= 0xcc, /* recipe for KK elixir */
	ITEM_REZEPT_FUER_MUTELIXIER		= 0xcd, /* recipe for MU elixir */
	ITEM_REZEPT_FUER_ZAUBERTRANK		= 0xce, /* recipe for AE potion */
	ITEM_RING__BLUE				= 0xcf, /* ring [blue, not magic, found in Zwingfeste] */
	ITEM_SCHLUESSEL__2			= 0xd0, /* key [2] */ /* TODO: Farbe? */
	ITEM_EISENSCHLUESSEL__1			= 0xd1, /* iron key [1] */ /* TODO: Farbe? */
	ITEM_EISENSCHLUESSEL__2			= 0xd2, /* iron key [2] */ /* TODO: Farbe? */
	ITEM_ROTER_SCHLUESSEL			= 0xd3, /* red key */
	ITEM_BRONZESCHLUESSEL			= 0xd4, /* bronze key */
	ITEM_SILBERHELM				= 0xd5, /* silver helmet [magic: can be worn by all hero classes; found in Wolfshoehle] */
	ITEM_SILBERSTREITKOLBEN			= 0xd6, /* silver mace [magic; found in Verfallene Herberge] */
	ITEM_SILBERSCHMUCK__MAGIC		= 0xd7, /* silver jewelry [magic: TA - 2; found in Verfallene Herberge] */
	ITEM_SPEER__MAGIC			= 0xd8, /* spear [magic; found in Spinnenhoehle] */
	ITEM_STIRNREIF__BLUE			= 0xd9, /* coronet [blau, magic: MR + 2; found in Tempel des Namenlosen] */
	ITEM_WURFDOLCH__MAGIC			= 0xda, /* throwing dagger [magic, improved accuracy, damage 1D6 + 9; found in Drachenhoehle]. Note that there is no 'ordinary' throwing dagger in the game. See the discussion at https://crystals-dsa-foren.de/showthread.php?pid=170046 */
	ITEM_GOLDSCHLUESSEL			= 0xdb, /* gold key. note that there is also ITEM_GOLDENER_SCHLUESSEL (golden key) */
	ITEM_RING__GREEN			= 0xdc, /* ring [green, magic: protection from magic fire; found in Drachenhoehle] */
	ITEM_BEUTEL				= 0xdd, /* bag [found and used in the first level of the ruin of the black wizard to open a passage] */
	ITEM_BUCH__PIRATE_ACCOUNTING		= 0xde, /* book [dacades old accounting book about loot income of Daspota pirates] */
	ITEM_ANTIKRANKHEITSELIXIER		= 0xdf, /* anti disease elixir */
	ITEM_BUCH__KAISERSPRUECHE_HALS		= 0xe0, /* book ["Kaisersprüche Hals"; found in the dungeon of the Daspota treasure (Rybon-Thoss)] */
	ITEM_KRISTALL				= 0xe1, /* crystal */
	ITEM_MU_ELIXIER__BAD			= 0xe2, /* MU elixir [bad] */
	ITEM_KL_ELIXIER__BAD			= 0xe3, /* KL elixir [bad] */
	ITEM_CH_ELIXIER__BAD			= 0xe4, /* CH elixir [bad] */
	ITEM_FF_ELIXIER__BAD			= 0xe5, /* FF elixir [bad] */
	ITEM_GE_ELIXIER__BAD			= 0xe6, /* GE elixir [bad] */
	ITEM_IN_ELIXIER__BAD			= 0xe7, /* IN elixir [bad] */
	ITEM_KK_ELIXIER__BAD			= 0xe8, /* KK elixir [bad] */
	ITEM_ROBE__GREEN_2			= 0xe9, /* robe [green] */
	ITEM_GOLDENER_SCHILD			= 0xea, /* golden shield [magic: can be equiped by all hero classes; found in Tempel des Namenlosen ] */
	ITEM_EMPFEHLUNGSSCHREIBEN__HETMAN	= 0xeb, /* commendation letter [Hetman] */
	ITEM_WUNDERKUR				= 0xec, /* Wunderkur */
	ITEM_SCHLAFTRUNK			= 0xed, /* sleep potion */
	ITEM_MIASTHMATICUM			= 0xee, /* Miasthmaticum */
	ITEM_HYLAILER_FEUER			= 0xef, /* Hylailer Feuer */
	ITEM_REZEPT_FUER_STARKEN_HEILTRANK	= 0xf0, /* recipe for strong LE potion [given by Olimone, tevent_114 Phexcaer <-> Skelellen: a swamp ]*/
	ITEM_REZEPT_FUER_WUNDERKUR		= 0xf1, /* recipe for Wunderkur */
	ITEM_REZEPT_FUER_SCHLAFTRUNK		= 0xf2, /* recipe for sleeping potion */
	ITEM_REZEPT_FUER_STARKER_ZAUBERTRANK	= 0xf3, /* recipe for strong AE potion */
	ITEM_REZEPT_FUER_MIASTHMATICUM		= 0xf4, /* recipe for Miasthmaticum */
	ITEM_STIRNREIF__GREEN			= 0xf5, /* coronet [green, magic, 3 charges: use -> Armatrutz +5, i.e. RS + 5; found 2x in Spinnenhoehle] */ /* TODO: Is this in fact another SILBERNER_STIRNREIF? */
	ITEM_BUCH__HEPTAGON			= 0xf6, /* book [read -> 'Heptagon' spell +3; found in Spinnenhoehle] */
	ITEM_EMPFEHLUNGSSCHREIBEN__SIEBENSTEIN	= 0xf7, /* commendation letter [Umbrik Siebenstein] */
	ITEM_SCHWARZE_STATUETTE			= 0xf8, /* black figurine, found in Tempel des Namenlosen and used as evidence to Eliane Windenbeck */
	ITEM_LATERNE__LIT			= 0xf9, /* lantern [lit] */
	ITEM_DUKATEN				= 0xfa, /* ducats */
	ITEM_200_PFEILE				= 0xfb, /* 200 arrows */
	ITEM_50_BOLZEN				= 0xfc, /* 50 bolts */
	ITEM_20_KLETTERHAKEN			= 0xfd  /* 20 climbing hooks */
};

enum {
	IGNITE_MODE_SPELL_OR_USE_TINDER	= 0,
	IGNITE_MODE_USE_TORCH	= 1,
	IGNITE_MODE_USE_LANTERN	= 2
};

enum {
	GRAMMAR_GENDER_MASCULINE	= 0,
	GRAMMAR_GENDER_FEMININE		= 1,
	GRAMMAR_GENDER_NEUTER		= 2
};

enum {
	GRAMMAR_ARTICLE_INDEFINITE = 0,
	GRAMMAR_ARTICLE_NONE = 1,
	GRAMMAR_ARTICLE_DEFINITE = 2
};

enum {
	GRAMMAR_NUMBER_SINGULAR = 0,
	GRAMMAR_NUMBER_PLURAL = 1
};

enum {
	GRAMMAR_CASE_1ST = 0,
	GRAMMAR_CASE_2ND = 1,
	GRAMMAR_CASE_4TH = 2,
	GRAMMAR_CASE_3RD = 3
};

enum {
	INFLECT_NAME_TYPE_ITEM = 0,
	INFLECT_NAME_TYPE_MONSTER = 1
};

enum {
	INFLECT_1ST_CASE            = GRAMMAR_CASE_1ST,                     // 0x0000
	INFLECT_2ND_CASE            = GRAMMAR_CASE_2ND,                     // 0x0001
	INFLECT_4TH_CASE            = GRAMMAR_CASE_4TH,                     // 0x0002
	INFLECT_3RD_CASE            = GRAMMAR_CASE_3RD,                     // 0x0003
	INFLECT_CASE_BITMASK        = 0x03,				    // 0x0003
	INFLECT_SINGULAR            = GRAMMAR_NUMBER_SINGULAR << 2,         // 0x0000
	INFLECT_PLURAL              = GRAMMAR_NUMBER_PLURAL << 2,           // 0x0004
	INFLECT_NUMBER_BITMASK      = 0x01 << 2,                            // 0x0004
	INFLECT_CASE_NUMBER_BITMASK = 0x0f,			            // 0x000f (1st nibble, includes bit 3 (unused))
	INFLECT_GENDER_UNSPECIFIED  = 0x00 << 12,                           // 0x0000
	INFLECT_MASCULINE           = (GRAMMAR_GENDER_MASCULINE + 1) << 12, // 0x1000
	INFLECT_FEMININE            = (GRAMMAR_GENDER_FEMININE + 1) << 12,  // 0x2000
	INFLECT_NEUTER              = (GRAMMAR_GENDER_NEUTER + 1) << 12,    // 0x3000
	INFLECT_GENDER_BITMASK      = 0x03 << 12,                           // 0x3000
	INFLECT_INDEFINITE_ARTICLE  = GRAMMAR_ARTICLE_INDEFINITE << 14,     // 0x0000
	INFLECT_OMIT_ARTICLE        = GRAMMAR_ARTICLE_NONE << 14,           // 0x4000
	INFLECT_DEFINITE_ARTICLE    = GRAMMAR_ARTICLE_DEFINITE << 14,       // 0x8000
	INFLECT_ARTICLE_BITMASK     = 0x03 << 14                            // 0xc000
};
// INFLECT_INDEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_1ST_CASE

enum {
	/* strictly speaking, these values encode not only the ship type, but the combined information
	 * route type (high seas vs. costal); ship type (Langschiff, etc.); passage type (Kabinenpassage, etc.)
	 * As there are at most two possible combinations per type of ship, we still call it SHIP_TYPE for simplicity. */

	/* ships on high seas routes */
	SHIP_TYPE_LANGSCHIFF_HIGH_SEAS  = 0,
	SHIP_TYPE_KARRACKE  = 1,
	SHIP_TYPE_SCHNELLSEGLER  = 2,
	SHIP_TYPE_SCHNELLSEGLER_LUXURY = 3,
	/* ships on costal routes */
	SHIP_TYPE_LANGSCHIFF_COSTAL  = 4,
	SHIP_TYPE_KUESTENSEGLER  = 5,
	SHIP_TYPE_KUTTER  = 6,
	SHIP_TYPE_FISCHERBOOT = 7
};

enum {
	SHIP_TABLE_PASSAGE_TYPE = 0, /* one byte */ /*
	       0 = Heuer (Regen. -2);
	       1 = Begleitschutzfahrt (Regen. 0);
	       2 = Luxuspassage (Regen. +5);
	       3 = komfortable Passage (Regen. +4);
	       4 = Kabinenpassage (Regen. +3);
	       5 = Deckpassage (Regen. +1);
	       6 = Mitfahrgelegenheit (Regen. 0)
	*/
};

enum {
	GAME_MODE_UNSPECIFIED	= -1,
	GAME_MODE_BEGINNER	= 1,
	GAME_MODE_ADVANCED	= 2
};

enum { /* note that the order differs from the one in HERO_TYPE... :( */
	SPELL_ORIGIN_DRUIDE = 0,
	SPELL_ORIGIN_MAGIER = 1,
	SPELL_ORIGIN_HEXE = 2,
	SPELL_ORIGIN_AUELF = 3,
	SPELL_ORIGIN_FIRNELF = 4,
	SPELL_ORIGIN_WALDELF = 5
};

enum {
	MENU_ICON_NONE			= -1,
	MENU_ICON_HIRE_HERO		= 0,
	MENU_ICON_DISMISS_HERO		= 1,
	MENU_ICON_DELETE_HERO		= 2,
	MENU_ICON_LOAD_GAME		= 3,
	MENU_ICON_SAVE_GAME		= 4,
	MENU_ICON_QUIT_GAME		= 5,
	MENU_ICON_PRAY			= 6,
	MENU_ICON_SACRIFICE		= 7,
	MENU_ICON_LEAVE			= 8,
	MENU_ICON_GUARDS		= 9,
	MENU_ICON_GIVE_GOLD		= 10, /* unused? */
	MENU_ICON_MAGIC			= 11,
	MENU_ICON_SPLIT_GROUP		= 12,
	MENU_ICON_ORDER_FOOD		= 13,
	MENU_ICON_TAKE_GOLD		= 14, /* unused? */
	MENU_ICON_MERGE_GROUP		= 15,
	MENU_ICON_REPLENISH_SUPPLIES	= 16,
	MENU_ICON_SLEEP			= 17,
	MENU_ICON_REPAIR		= 18,
	MENU_ICON_BOOK_BED		= 19,
	MENU_ICON_GATHER_HERBS		= 20,
	MENU_ICON_TALK			= 21,
	MENU_ICON_BUY			= 22,
	MENU_ICON_BARGAIN		= 23,
	MENU_ICON_SELL			= 24,
	MENU_ICON_APPLY_TALENT		= 25,
	MENU_ICON_SCROLL_RIGHT		= 26,
	MENU_ICON_SCROLL_LEFT		= 27,
	MENU_ICON_HERO			= 28,
	MENU_ICON_SWITCH_GROUP		= 29,
	MENU_ICON_HEAL_WOUNDS		= 30,
	MENU_ICON_HEAL_DISEASE		= 31,
	MENU_ICON_HEAL_POISON		= 32,
	MENU_ICON_OPEN_CLOSE_DOOR	= 33,
	MENU_ICON_SMASH_DOOR		= 34,
	MENU_ICON_PICK_LOCK		= 35,
	MENU_ICON_WIZARD_FLOOR		= 36, /* unused? */
	MENU_ICON_INFO			= 37,
	MENU_ICON_OPEN_CHEST		= 38,
	MENU_ICON_MAP			= 39,
	MENU_ICON_BOOK_SHIP_PASSAGE	= 40,
	MENU_ICON_BOARD_SHIP		= 41,
	MENU_ICON_HARBOR_MASTER		= 42,
	MENU_ICON_MARKET		= 43,
	MENU_ICON_HYGELLIK_MAP		= 44,
	MENU_ICON_MERGE_GROUP_GRAYED	= 45,
	MENU_ICON_MOVE_LEVER		= 46,
	MENU_ICON_ATTACK		= 47, /* unused? */
	MENU_ICON_QUIT_CAMP		= 48,
	MENU_ICON_INN			= 49,
	MENU_ICON_TAVERN		= 50,
	MENU_ICON_BATTLE_FRAME_RATE	= 51,
	MENU_ICON_DIARY			= 52,
	MENU_ICON_SOUND			= 53,
	MENU_ICON_CAMP			= 54
};

enum {
	SPECIAL_DATE_ID_NONE                            =  0,
	SPECIAL_DATE_ID_SOMMERSONNENWENDE               =  1,
	SPECIAL_DATE_ID_PRAIOSFEST                      =  2,
	SPECIAL_DATE_ID_TAG_DES_SCHWURS                 =  3,
	SPECIAL_DATE_ID_SCHWERTFEST                     =  4,
	SPECIAL_DATE_ID_TAG_DES_WASSERS                 =  5,
	SPECIAL_DATE_ID_VIERTER_RASTULLAHELLAH          =  6,
	SPECIAL_DATE_ID_NEBELFEST                       =  7,
	SPECIAL_DATE_ID_FISCHERFEST                     =  8,
	SPECIAL_DATE_ID_PRUEFUNGSFEST                   =  9,
	SPECIAL_DATE_ID_TAG_DER_HEIMKEHR                = 10,
	SPECIAL_DATE_ID_FEST_DER_EINGEBRACHTEN_FRUECHTE = 11,
	SPECIAL_DATE_ID_TAG_DER_HELDEN                  = 12,
	SPECIAL_DATE_ID_TAG_DER_TREUE                   = 13,
	SPECIAL_DATE_ID_TOTENFEST                       = 14,
	SPECIAL_DATE_ID_FUENFTER_RASTULLAHELLAH         = 15,
	SPECIAL_DATE_ID_DES_KAISERS_GEBURTSTAG          = 16,
	SPECIAL_DATE_ID_TAG_DES_GROSSEN_SCHLAFES        = 17,
	SPECIAL_DATE_ID_ROHALS_VERHUELLUNG              = 18,
	SPECIAL_DATE_ID_ERLEUCHTUNGSFEST                = 19,
	SPECIAL_DATE_ID_TAG_DER_JAGD                    = 20,
	SPECIAL_DATE_ID_TAG_DER_IFIRN                   = 21,
	SPECIAL_DATE_ID_ERSTER_RASTULLAHELLAH           = 22,
	SPECIAL_DATE_ID_TAG_DER_ERNEUERUNG              = 23,
	SPECIAL_DATE_ID_TAG_DES_PHEX                    = 24,
	SPECIAL_DATE_ID_GLUECKSTAG                      = 25,
	SPECIAL_DATE_ID_VERSENKUNGSFEST                 = 26,
	SPECIAL_DATE_ID_SAATFEST                        = 27,
	SPECIAL_DATE_ID_ZWEITER_RASTULLAHELLAH          = 28,
	SPECIAL_DATE_ID_TAG_DES_FEUERS                  = 29,
	SPECIAL_DATE_ID_TAG_DES_AUFBRUCHS               = 30,
	SPECIAL_DATE_ID_TAG_DER_WAFFENSCHMIEDE          = 31,
	SPECIAL_DATE_ID_FEST_DER_FREUDEN                = 32,
	SPECIAL_DATE_ID_REINIGUNGSFEST                  = 33
};

enum{
	STACK_SIZE_MAX = 99
};

#endif
