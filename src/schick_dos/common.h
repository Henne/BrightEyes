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
	ACTION_ID_ESC                                =   1,
	ACTION_ID_CLOSING_SQUARE_BRACKET             =  27, /* the ']' key. used in buy_screen() at a merchant to decrease the number of items. */
	ACTION_ID_RETURN                             =  28,
	ACTION_ID_J                                  =  44, /* the 'J' key. used in seg097.cpp in GUI_menu_input (when called from GUI_bool()). selects the "Ja" (yes) answer. */
	ACTION_ID_N                                  =  49, /* the 'N' key. used in seg097.cpp in GUI_menu_input (when called from GUI_bool()). selects the "Nein" (no) answer. */
	ACTION_ID_SLASH                              =  53, // the '/' key. used in buy_screen() at a merchant to increase the number of items.
	ACTION_ID_SPACE                              =  57,
	ACTION_ID_UP                                 =  72,
	ACTION_ID_PAGE_UP                            =  73, /* opens the menu */
	ACTION_ID_LEFT                               =  75,
	ACTION_ID_RIGHT                              =  77,
	ACTION_ID_DOWN                               =  80,
	ACTION_ID_PAGE_DOWN                          =  81, /* closes the menu */
	/* in seg048.cpp there is "if (ds_readws(ACTION) >= 128 && ds_readws(ACTION) <= 152) [...]".
	 * I think these 25 numbers are used for the icons in the character screen, probably in the order
	 * head, arms, body, right hand, left hand, legs, feet, knapsack1 .. knapsack16; eye, mouth */
	ACTION_ID_ICON_1                             = 129,
	ACTION_ID_ICON_2                             = 130,
	ACTION_ID_ICON_3                             = 131,
	ACTION_ID_ICON_4                             = 132,
	ACTION_ID_ICON_5                             = 133,
	ACTION_ID_ICON_6                             = 134,
	ACTION_ID_ICON_7                             = 135,
	ACTION_ID_ICON_8                             = 136,
	ACTION_ID_ICON_9                             = 137,
	ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK = 144, /* set in buy_screen() at a merchant if an item is right-clicked to decrease the count. */
	ACTION_ID_240                                = 240, /* what does it mean? used in status_menu() in seg048.cpp */
	/* In seg057.cpp and seg058.cpp there is "if (ds_readws(ACTION) >= 241 && ds_readws(ACTION) <= 247) [...]" */
	ACTION_ID_VOID                               = 999
};

/* 26 ingame timers for (in this order) active miracles (6), unavailable talent usages (2), active spells (2) and unavailable miracles (16). */
enum {
	INGAME_TIMER_RONDRA_NO_SPELLS        =  0, /* Rondra miracle active: block spells (6 hours) */
	INGAME_TIMER_EFFERD_FIND_WATER       =  1, /* Efferd miracle active: search for water in wildcamp will always be successful (3 days) */
	INGAME_TIMER_EFFERD_SAFE_PASSAGE     =  2, /* Efferd miracle active: no incidences on sea traveling (3 days) */
	INGAME_TIMER_FIRUN_HUNT              =  3, /* Firun miracle active: hunting in wildcamps will always be successful (3 or 7 days) */
	INGAME_TIMER_TRAVIA_SAFE_REST        =  4, /* Travia miracle active: safety during the night (no camp attacks during sleep) active (7 days) */
	INGAME_TIMER_BORON_UNDEAD            =  5, /* Boron miracle active: protection from undead (but apparently, does not have an impact anywhere...) (3 days) */
	INGAME_TIMER_AKROBATIK               =  6, /* talent usage unavailable: Akrobatik (8 hours) */
	INGAME_TIMER_MUSIZIEREN              =  7, /* talent usage unavailable: Musizieren (8 hours) */
	INGAME_TIMER_FLIM_FLAM               =  8, /* spell 'Flim Flam' is active */
	INGAME_TIMER_DARKNESS                =  9, /* spell 'Dunkelheit' is active */
	INGAME_TIMER_PRAIOS_MU               = 10, /* Praios miracle unavailable: MU +1 (1 day) */
	INGAME_TIMER_PRAIOS_MR               = 11, /* Praios miracle unavailable: MR +99 (3 days) */
	INGAME_TIMER_RONDRA_SWORDS           = 12, /* Rondra miracle unavailable: Schwerter +1, AT + 1 (3 days) */
	INGAME_TIMER_RONDRA_MAGIC_WEAPON     = 13, /* Rondra miracle unavailable: make weapon magic (1 day) */
	INGAME_TIMER_EFFERD_SWIM             = 14, /* Efferd miracle unavailable: Schwimmen +2 (4 days) */
	INGAME_TIMER_BORON_TA                = 15, /* Boron miracle unavailable: TA -1 (4 days) */
	INGAME_TIMER_HESINDE_ANALUES         = 16, /* Hesinde miracle unavailable: 'Analues Arkanstruktur +1' (5 days) */
	INGAME_TIMER_HESINDE_MR              = 17, /* Hesinde miracle unavailable: MR +5 (4 days) */
	INGAME_TIMER_FIRUN_SATED             = 18, /* Firun miracle unavailable: no hunger or thirst (7 days) */
	INGAME_TIMER_PHEX_THIEF              = 19, /* Phex miracle unavailable: talents Schleichen +1, Taschendiebstahl +1 (3 days) */
	INGAME_TIMER_PHEX_FEILSCHEN          = 20, /* Phex miracle unavailable: Feilschen +1 (3 days) */
	INGAME_TIMER_PHEX_FF                 = 21, /* Phex miracle unavailable: FF +1 (3 days) */
	INGAME_TIMER_INGERIMM_MAGIC_WEAPON   = 22, /* Ingerimm miracle unavailable: make weapon magic (1 day) */
	INGAME_TIMER_RAHJA_TALENTS           = 23, /* Rahja miracle unavailable: Tanzen +1, Betoeren +1 (7 days) */
	INGAME_TIMER_RAHJA_CH                = 24, /* Rahja miracle unavailable: CH +1 (3 days) */
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
	HERO_TYPE_NONE		=  0,
	HERO_TYPE_GAUKLER	=  1,	/* juggler */
	HERO_TYPE_JAEGER	=  2,	/* hunter */
	HERO_TYPE_KRIEGER	=  3,	/* warrior */
	HERO_TYPE_STREUNER	=  4,	/* stray */
	HERO_TYPE_THORWALER	=  5,	/* Thorwalian */
	HERO_TYPE_ZWERG		=  6,	/* dwarf */
	/* magic users have id > 66 */
	HERO_TYPE_HEXE		=  7,	/* witch */
	HERO_TYPE_DRUIDE	=  8,	/* druid */
	HERO_TYPE_MAGIER	=  9,	/* mage */
	HERO_TYPE_AUELF		= 10,	/* green elf */
	HERO_TYPE_FIRNELF	= 11,	/* ice elf */
	HERO_TYPE_WALDELF	= 12,	/* sylvan elf */
};

enum {
	ACTOR_SPRITE_ID_NONE              =  0,
	ACTOR_SPRITE_ID_GAUKLER__MALE     =  1,
	ACTOR_SPRITE_ID_JAEGER__MALE      =  2,
	ACTOR_SPRITE_ID_KRIEGER__MALE     =  3,
	ACTOR_SPRITE_ID_STREUNER__MALE    =  4,
	ACTOR_SPRITE_ID_THORWALER__MALE   =  5,
	ACTOR_SPRITE_ID_ZWERG__MALE       =  6,
	ACTOR_SPRITE_ID_HEXE__MALE        =  7,
	ACTOR_SPRITE_ID_DRUIDE__MALE      =  8,
	ACTOR_SPRITE_ID_MAGIER__MALE      =  9,
	ACTOR_SPRITE_ID_ELF__MALE         = 10,
	ACTOR_SPRITE_ID_GAUKLER__FEMALE   = 12,
	ACTOR_SPRITE_ID_JAEGER__FEMALE    = 13,
	ACTOR_SPRITE_ID_KRIEGER__FEMALE   = 14,
	ACTOR_SPRITE_ID_STREUNER__FEMALE  = 15,
	ACTOR_SPRITE_ID_THORWALER__FEMALE = 16,
	ACTOR_SPRITE_ID_ZWERG__FEMALE     = 17,
	ACTOR_SPRITE_ID_HEXE__FEMALE      = 18,
	ACTOR_SPRITE_ID_DRUIDE__FEMALE    = 19,
	ACTOR_SPRITE_ID_MAGIER__FEMALE    = 20,
	ACTOR_SPRITE_ID_ELF__FEMALE       = 21,
	ACTOR_SPRITE_ID_OGER              = 23,
	ACTOR_SPRITE_ID_ORK               = 24,
	ACTOR_SPRITE_ID_GOBLIN            = 25,
	ACTOR_SPRITE_ID_HESHTHOT          = 26,
	ACTOR_SPRITE_ID_TIGER             = 27,
	ACTOR_SPRITE_ID_SKELETT           = 28,
	ACTOR_SPRITE_ID_WALDSCHRAT        = 29,
	ACTOR_SPRITE_ID_MUMIE             = 30,
	ACTOR_SPRITE_ID_HARPYIE           = 31, /* Strangely, MONSTER_ID_LINDWURM and MONSTER_ID_HOEHLENDRACHE have this sprite_id. But they are not found in the game. */
	ACTOR_SPRITE_ID_SPINNE            = 32,
	ACTOR_SPRITE_ID_KAEFER            = 33,
	ACTOR_SPRITE_ID_ZOMBIE            = 34,
	ACTOR_SPRITE_ID_PIRAT             = 35,
	ACTOR_SPRITE_ID_RAEUBER           = 36,
	ACTOR_SPRITE_ID_ZAUBERER          = 37,
	ACTOR_SPRITE_ID_HUND              = 38,
	ACTOR_SPRITE_ID_LOEWE             = 39,
	ACTOR_SPRITE_ID_WOLF              = 40
};

enum {
	FIG_ACTION_NONE			=  0,
	FIG_ACTION_MOVE			=  1,
	FIG_ACTION_MELEE_ATTACK		=  2,
	FIG_ACTION_GUARD		=  3,
	FIG_ACTION_SPELL		=  4,
	FIG_ACTION_USE_ITEM		=  5,
	FIG_ACTION_DROP_ITEM		=  6,
	FIG_ACTION_EXCHANGE_WEAPON	=  7,
	FIG_ACTION_EXCHANGE_ITEM	=  8,
	FIG_ACTION_CHECK_VALUES		=  9,
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
	ATTRIB_MU   =  0,
	ATTRIB_KL   =  1,
	ATTRIB_CH   =  2,
	ATTRIB_FF   =  3,
	ATTRIB_GE   =  4,
	ATTRIB_IN   =  5,
	ATTRIB_KK   =  6,
	ATTRIB_AG   =  7,
	ATTRIB_HA   =  8,
	ATTRIB_RA   =  9,
	ATTRIB_GG   = 10,
	ATTRIB_TA   = 11,
	ATTRIB_NG   = 12,
	ATTRIB_JZ   = 13,
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
	MONSTER_ID_NONE                 =  0,
	MONSTER_ID_OGER                 =  1,
	MONSTER_ID_ORK__LVL_1           =  2,
	MONSTER_ID_GOBLIN__LVL_1        =  3,
	MONSTER_ID_HESHTHOT__LVL_1      =  4,
	MONSTER_ID_SAEBELZAHNTIGER      =  5,
	MONSTER_ID_SKELETT__LVL_1       =  6,
	MONSTER_ID_WALDSCHRAT           =  7,
	MONSTER_ID_MUMIE__LVL_1         =  8,
	MONSTER_ID_HARPYIE              =  9,
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
	MONSTER_ID_HESHTOT__WEAK        = 77,
	MONSTER_ID__END                 = 78
};

enum {
	MONSTER_SPELL_NONE                    =  0, // 0x00
	MONSTER_SPELL_VERWANDLUNG_BEENDEN     =  1, // 0x01
	MONSTER_SPELL_BANNBALADIN             =  2, // 0x02
	MONSTER_SPELL_BOESER_BLICK            =  3, // 0x03
	MONSTER_SPELL_HORRIPHOBUS             =  4, // 0x04
	MONSTER_SPELL_AXXELERATUS             =  5, // 0x05
	MONSTER_SPELL_BALSAM                  =  6, // 0x06
	MONSTER_SPELL_BLITZ                   =  7, // 0x07
	MONSTER_SPELL_EISENROST               =  8, // 0x08
	MONSTER_SPELL_FULMINICTUS             =  9, // 0x09
	MONSTER_SPELL_IGNIFAXIUS              = 10, // 0x0a
	MONSTER_SPELL_PLUMBUMBARUM            = 11, // 0x0b
	MONSTER_SPELL_SAFT_KRAFT_MONSTERMACHT = 12, // 0x0c
	MONSTER_SPELL_ARMATRUTZ               = 13, // 0x0d
	MONSTER_SPELL_PARALUE                 = 14, // 0x0e
	MONSTER_SPELL__END                    = 15
};

enum {
	HERO_INVENTORY_SLOT_HEAD	=  0,
	HERO_INVENTORY_SLOT_ARMS	=  1,
	HERO_INVENTORY_SLOT_BODY	=  2,
	HERO_INVENTORY_SLOT_RIGHT_HAND	=  3,
	HERO_INVENTORY_SLOT_LEFT_HAND	=  4,
	HERO_INVENTORY_SLOT_LEGS	=  5,
	HERO_INVENTORY_SLOT_FEET	=  6,
	HERO_INVENTORY_SLOT_KNAPSACK_1	=  7,
	HERO_INVENTORY_SLOT_KNAPSACK_2	=  8,
	HERO_INVENTORY_SLOT_KNAPSACK_3	=  9,
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
	HERB_POTION_TYPE_POTION          = 1
};

enum {
	 WEAPON_STATS_ID_SCHWERT               =  0,
	 WEAPON_STATS_ID_KNUEPPEL              =  1,
	 WEAPON_STATS_ID_SAEBEL                =  2,
	 WEAPON_STATS_ID_MESSER                =  3,
	 WEAPON_STATS_ID_SPEER                 =  4,
	 WEAPON_STATS_ID_KURZSCHWERT           =  5,
	 WEAPON_STATS_ID_KRIEGSBEIL__SPECIAL   =  6,
	 WEAPON_STATS_ID_KURZBOGEN             =  7,
	 WEAPON_STATS_ID_STREITAXT             =  8,
	 WEAPON_STATS_ID_ARMBRUST              =  9,
	 WEAPON_STATS_ID_DOLCH                 = 10, /* WEAPON_STATS_ID_DOLCH, WEAPON_STATS_ID_KUKRISDOLCH */
	 WEAPON_STATS_ID_WURFBEIL              = 11,
	 WEAPON_STATS_ID_WURFSTERN             = 12,
	 WEAPON_STATS_ID_ZWEIHAENDER           = 13,
	 WEAPON_STATS_ID_LANGBOGEN             = 14,
	 WEAPON_STATS_ID_MORGENSTERN           = 15,
	 WEAPON_STATS_ID_VULKANGLASDOLCH       = 16,
	 WEAPON_STATS_ID_WURFAXT               = 17,
	 WEAPON_STATS_ID_PIKE                  = 18,
	 WEAPON_STATS_ID_STREITKOLBEN          = 19, /* WEAPON_STATS_ID_STREITKOLBEN, WEAPON_STATS_ID_SILBERSTREITKOLBEN */
	 WEAPON_STATS_ID_DEGEN                 = 20,
	 WEAPON_STATS_ID_FLORETT               = 21,
	 WEAPON_STATS_ID_KAMPFSTAB             = 22,
	 WEAPON_STATS_ID_PEITSCHE              = 23,
	 WEAPON_STATS_ID_WURFMESSER            = 24,
	 WEAPON_STATS_ID_SICKLE                = 25, /* WEAPON_STATS_ID_SICKLE, WEAPON_STATS_ID_SICKLE_MAGIC */
	 WEAPON_STATS_ID_SENSE                 = 26,
	 WEAPON_STATS_ID_KRIEGSHAMMER          = 27,
	 WEAPON_STATS_ID_DREIZACK              = 28,
	 WEAPON_STATS_ID_HELLEBARDE            = 29,
	 WEAPON_STATS_ID_DRESCHFLEGEL          = 30,
	 WEAPON_STATS_ID_ZWEILILIEN            = 31,
	 WEAPON_STATS_ID_OCHSENHERDE           = 32,
	 WEAPON_STATS_ID_BASILISKENZUNGE       = 33,
	 WEAPON_STATS_ID_OGERFAENGER           = 34,
	 WEAPON_STATS_ID_MENGBILAR             = 35, /* WEAPON_STATS_ID_MENGBILAR, WEAPON_STATS_ID_KUKRIS_MENGBILAR */
	 WEAPON_STATS_ID_SCHWERER_DOLCH        = 36,
	 WEAPON_STATS_ID_RONDRAKAMM            = 37,
	 WEAPON_STATS_ID_ENTERMESSER           = 38,
	 WEAPON_STATS_ID_BASTARDSCHWERT        = 39,
	 WEAPON_STATS_ID_TUZAKMESSER           = 40,
	 WEAPON_STATS_ID_RABENSCHNABEL         = 41,
	 WEAPON_STATS_ID_BRABAKBENGEL          = 42,
	 WEAPON_STATS_ID_RAPIER                = 43,
	 WEAPON_STATS_ID_KUNCHOMER             = 44,
	 WEAPON_STATS_ID_DOPPELKUNCHOMER       = 45,
	 WEAPON_STATS_ID_SCHLEUDER             = 46,
	 WEAPON_STATS_ID_SKRAJA                = 47,
	 WEAPON_STATS_ID_KRIEGSBEIL            = 48,
	 WEAPON_STATS_ID_ORKNASE               = 49,
	 WEAPON_STATS_ID_SCHNEIDZAHN           = 50,
	 WEAPON_STATS_ID_ROBBENTOETER          = 51,
	 WEAPON_STATS_ID_WOLFSMESSER           = 52,
	 WEAPON_STATS_ID_PROJECTILE            = 53, /* WEAPON_STATS_ID_PFEIL, WEAPON_STATS_ID_BOLZEN */
	 WEAPON_STATS_ID_BASTARDSCHWERT__MAGIC = 54,
	 WEAPON_STATS_ID_ORKNASE__MAGIC        = 55,
	 WEAPON_STATS_ID_KURZSCHWERT__MAGIC    = 56,
	 WEAPON_STATS_ID_SAEBEL__MAGIC         = 57,
	 WEAPON_STATS_ID_ZWEIHAENDER__MAGIC    = 58,
	 WEAPON_STATS_ID_DAS_SCHWERT_GRIMRING  = 59,
	 WEAPON_STATS_ID_SCHWERT__MAGIC        = 60,
	 WEAPON_STATS_ID_SPEER__MAGIC          = 61,
	 WEAPON_STATS_ID_WURFDOLCH__MAGIC      = 62,
	 WEAPON_STATS_ID_ZAUBERSTAB_HEXENBESEN = 63, /* WEAPON_STATS_ID_ZAUBERSTAB, WEAPON_STATS_ID_HEXENBESEN */
	 WEAPON_STATS_ID__END                  = 64
};

enum {
	ARMOR_STATS_ID_SCHILD             =  0,
	ARMOR_STATS_ID_EISENSCHILD        =  1, /* ITEM_ID_EISENSCHILD, ITEM_ID_GOLDENER_SCHILD */
	ARMOR_STATS_ID_EISENHELM          =  2, /* ITEM_ID_EISENHELM, ITEM_ID_SILBERHELM */
	ARMOR_STATS_ID_HEMD               =  3,
	ARMOR_STATS_ID_HOSE               =  4,
	ARMOR_STATS_ID_SCHUHE             =  5,
	ARMOR_STATS_ID_STIEFEL            =  6,
	ARMOR_STATS_ID_SCHNEESCHUHE       =  7,
	ARMOR_STATS_ID_LEDERHARNISCH      =  8,
	ARMOR_STATS_ID_SCHUPPENRUESTUNG   =  9,
	ARMOR_STATS_ID_ROBE__GREEN        = 10, /* ITEM_ID_ROBE__GREEN_1, ITEM_ID_ROBE__GREEN_2 */
	ARMOR_STATS_ID_ROBE__RED          = 11,
	ARMOR_STATS_ID_TOPFHELM           = 12,
	ARMOR_STATS_ID_LEDERHELM          = 13,
	ARMOR_STATS_ID_WAFFENROCK         = 14,
	ARMOR_STATS_ID_KETTENHEMD         = 15,
	ARMOR_STATS_ID_KROETENHAUT        = 16,
	ARMOR_STATS_ID_PLATTENZEUG        = 17,
	ARMOR_STATS_ID_KETTENZEUG         = 18,
	ARMOR_STATS_ID_LEDERZEUG          = 19,
	ARMOR_STATS_ID_WINTERMANTEL       = 20,
	ARMOR_STATS_ID_HELM__CURSED       = 21,
	ARMOR_STATS_ID_KETTENHEMD__CURSED = 22,
	ARMOR_STATS_ID_KETTENHEMD__MAGIC  = 23,
	ARMOR_STATS_ID__END               = 24
};

enum {
	USABLE_ITEM_STATS_ID_NONE                  =  0,
	USABLE_ITEM_STATS_ID_ARCANO_PSYCHOSTABILIS =  1,
	USABLE_ITEM_STATS_ID_RECIPE                =  2,
	USABLE_ITEM_STATS_ID_DOCUMENT              =  3,
	USABLE_ITEM_STATS_ID_ARMATRUTZ             =  4,
	USABLE_ITEM_STATS_ID_FLIM_FLAM             =  5,
	USABLE_ITEM_STATS_ID_SCHULDBUCH            =  6,
	USABLE_ITEM_STATS_ID_ORKDOKUMENT           =  7,
	USABLE_ITEM_STATS_ID_WEAPON_POISON         =  8,
	USABLE_ITEM_STATS_ID_MIASTHMATICUM         =  9,
	USABLE_ITEM_STATS_ID_HYLAILER_FEUER        = 10,
	USABLE_ITEM_STATS_ID_SPELLBOOK_HEPTAGON    = 11,
	USABLE_ITEM_STATS_ID_LIGHTING              = 12,
	USABLE_ITEM_STATS_ID_MAGISCHER_BEUTEL      = 13,
	USABLE_ITEM_STATS_ID__END                  = 14,
};

enum {
	LOCTYPE_NONE		=  0,
	LOCTYPE_UNKN1		=  1,
	LOCTYPE_TEMPLE		=  2,
	LOCTYPE_TAVERN		=  3,
	LOCTYPE_HEALER		=  4,
	LOCTYPE_MERCHANT	=  5,
	LOCTYPE_WILDCAMP	=  6,
	LOCTYPE_INN		=  7,
	LOCTYPE_SMITH		=  8,
	LOCTYPE_MARKET		=  9,
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
	MERCHANT_HERBS   = 2,
	MERCHANT_GENERAL = 3
};

enum {
	MERCHANT_TYPEINDEX_MARKET           = 90,
	MERCHANT_TYPEINDEX_TREBORN_KOLBERG  = 91,
	MERCHANT_TYPEINDEX_THORWAL_ZEUGHAUS = 92,
	MERCHANT_TYPEINDEX_STREET_MERCHANT  = 93
};

enum {
	GAME_STATE_MAIN		=  0,
	GAME_STATE_DEAD		=  1, /* all heroes dead */
	GAME_STATE_UNKNOWN	=  2,
	GAME_STATE_QUIT		=  3, /* ordinary quit */
	GAME_STATE_TIMEUP	=  4, /* max. game time is up */
	GAME_STATE_OUTRO	=  5, /* after (successful) outro */
	GAME_STATE_FIGQUIT	=  7, /* quit game during fight */
	GAME_STATE_VICTORY	= 99 /* final fight won (before outro) */
};

enum {
	GOD_ID_PRAIOS		=  1,
	GOD_ID_RONDRA		=  2,
	GOD_ID_EFFERD		=  3,
	GOD_ID_TRAVIA		=  4,
	GOD_ID_BORON		=  5,
	GOD_ID_HESINDE		=  6,
	GOD_ID_FIRUN		=  7,
	GOD_ID_TSA		=  8,
	GOD_ID_PHEX		=  9,
	GOD_ID_PERAINE		= 10,
	GOD_ID_INGERIMM		= 11,
	GOD_ID_RAHJA		= 12,
	GOD_ID_SWAFNIR		= 13,
	GOD_ID_IFIRN		= 14,
	GOD_ID__END		= 15
};

enum {
	INFORMER_ID_JURGE	=  0,
	INFORMER_ID_HJORE	=  1,
	INFORMER_ID_YASMA	=  2,
	INFORMER_ID_UMBRIK	=  3,
	INFORMER_ID_ISLEIF	=  4,
	INFORMER_ID_RAGNA	=  5,
	INFORMER_ID_BEORN	=  6,
	INFORMER_ID_ASGRIMM	=  7,
	INFORMER_ID_ELIANE	=  8,
	INFORMER_ID_OLVIR	=  9,
	INFORMER_ID_SWAFNILD	= 10,
	INFORMER_ID_TREBORN	= 11,
	INFORMER_ID_UNICORN	= 12,
	INFORMER_ID_ALGRID	= 13,
	INFORMER_ID_TIOMAR	= 14,
	INFORMER_ID__END	= 15
};

enum { /* identifiers are based on the entries of FIGHT.LST */
	FIGHT_ID_ZUFALL1_LAND  =   0,
	FIGHT_ID_LAND_FIGHT1_1 =   1,
	FIGHT_ID_ZUFALL2_LAND  =   2,
	FIGHT_ID_ZUFALL3_LAND  =   3,
	FIGHT_ID_ZUFALL4_LAND  =   4,
	FIGHT_ID_SHIP3         =   5,
	FIGHT_ID_SHIP4         =   6,
	FIGHT_ID_SHIP5         =   7,
	FIGHT_ID_SHIP6         =   8,
	FIGHT_ID_SHIP8         =   9,
	FIGHT_ID_SHIP9         =  10,
	FIGHT_ID_SHIP12        =  11,
	FIGHT_ID_SHIP14        =  12,
	FIGHT_ID_SHIP15        =  13,
	FIGHT_ID_SHIP17        =  14,
	FIGHT_ID_SHIP18        =  15,
	FIGHT_ID_SHIP19        =  16,
	FIGHT_ID_SHIP21        =  17,
	FIGHT_ID_SHIP22        =  18,
	FIGHT_ID_SHIP23A       =  19,
	FIGHT_ID_SHIP23B       =  20,
	FIGHT_ID_SHIP24        =  21,
	FIGHT_ID_SHIP10        =  22,
	FIGHT_ID_F061_2        =  23,
	FIGHT_ID_F061_3        =  24,
	FIGHT_ID_F061_4B       =  25,
	FIGHT_ID_F061_4A       =  26,
	FIGHT_ID_F061_5        =  27,
	FIGHT_ID_F061_6A       =  28,
	FIGHT_ID_F108_1        =  29,
	FIGHT_ID_F108_2        =  30,
	FIGHT_ID_F108_3A       =  31,
	FIGHT_ID_F108_3B       =  32,
	FIGHT_ID_F108_3C       =  33,
	FIGHT_ID_F108_4        =  34,
	FIGHT_ID_F108_6        =  35,
	FIGHT_ID_F108_9        =  36,
	FIGHT_ID_F108_7        =  37,
	FIGHT_ID_F051_05_4     =  38,
	FIGHT_ID_F051_05_3     =  39,
	FIGHT_ID_F051_05_2     =  40,
	FIGHT_ID_F051_05_1     =  41,
	FIGHT_ID_F051_07       =  42,
	FIGHT_ID_F051_09       =  43,
	FIGHT_ID_F051_13       =  44,
	FIGHT_ID_F108_15       =  45,
	FIGHT_ID_F051_15       =  46,
	FIGHT_ID_F051_16       =  47,
	FIGHT_ID_F051_16_1     =  48,
	FIGHT_ID_F051_03       =  49,
	FIGHT_ID_F051_04       =  50,
	FIGHT_ID_F051_17       =  51,
	FIGHT_ID_F051_18       =  52,
	FIGHT_ID_F051_19       =  53,
	FIGHT_ID_F051_20_2     =  54,
	FIGHT_ID_F051_18_3     =  55,
	FIGHT_ID_F046_01       =  56,
	FIGHT_ID_F046_04       =  57,
	FIGHT_ID_F046_06       =  58,
	FIGHT_ID_F046_07       =  59,
	FIGHT_ID_F046_10       =  60,
	FIGHT_ID_F046_12       =  61,
	FIGHT_ID_F046_13       =  62,
	FIGHT_ID_F046_15       =  63,
	FIGHT_ID_F046_16       =  64,
	FIGHT_ID_F046_18       =  65,
	FIGHT_ID_F046_22       =  66,
	FIGHT_ID_F046_24       =  67,
	FIGHT_ID_F046_25       =  68,
	FIGHT_ID_F046_26       =  69,
	FIGHT_ID_F046_26_5     =  70,
	FIGHT_ID_F046_27       =  71,
	FIGHT_ID_F046_28       =  72,
	FIGHT_ID_F046_31       =  73,
	FIGHT_ID_F076_04       =  74,
	FIGHT_ID_F076_06       =  75,
	FIGHT_ID_F076_07       =  76,
	FIGHT_ID_F076_10       =  77,
	FIGHT_ID_F076_11       =  78,
	FIGHT_ID_F076_13       =  79,
	FIGHT_ID_F094_02       =  80,
	FIGHT_ID_F094_05       =  81,
	FIGHT_ID_F094_10       =  82,
	FIGHT_ID_F094_13       =  83,
	FIGHT_ID_F094_17       =  84,
	FIGHT_ID_F094_19       =  85,
	FIGHT_ID_F094_22       =  86,
	FIGHT_ID_F100_01       =  87,
	FIGHT_ID_F100_03       =  88,
	FIGHT_ID_F100_05       =  89,
	FIGHT_ID_F100_12       =  90,
	FIGHT_ID_F100_13       =  91,
	FIGHT_ID_F126_03       =  92,
	FIGHT_ID_F126_07       =  93,
	FIGHT_ID_F126_08       =  94, /* fleeing cultist */
	FIGHT_ID_F126_09       =  95,
	FIGHT_ID_F126_11       =  96,
	FIGHT_ID_F126_12       =  97,
	FIGHT_ID_F126_13       =  98,
	FIGHT_ID_F126_17       =  99,
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
	FIGHT_ID_DCAMPFIGHT4   = 257,
	FIGHT_ID__END          = 258
};

/* a SCENARIO is basically the board of a fight.
 * The identifiers are based on the entries of SCENARIO.LST.
 * Note that the IDs are 1-based, this is seen comparing FIGHT.LST and SCENARIO.LST. */
enum {
	SCENARIO_ID_NONE          =   0,
	SCENARIO_ID_BLANK_DUNGEON =   1,
	SCENARIO_ID_BLANK_CITY    =   2,
	SCENARIO_ID_BLANK_LAND    =   3,
	SCENARIO_ID_BLANK_SHIP    =   4,
	SCENARIO_ID_ZUFALL1_LAND  =   5,
	SCENARIO_ID_LAND_ZUFALL1  =   6,
	SCENARIO_ID_LAND_ZUFALL2  =   7,
	SCENARIO_ID_LAND_ZUFALL3  =   8,
	SCENARIO_ID_SHIP3         =   9,
	SCENARIO_ID_SHIP4         =  10,
	SCENARIO_ID_SHIP5         =  11,
	SCENARIO_ID_SHIP6         =  12,
	SCENARIO_ID_SHIP8         =  13,
	SCENARIO_ID_SHIP9         =  14,
	SCENARIO_ID_SHIP12NACKT   =  15,
	SCENARIO_ID_SHIP12        =  16,
	SCENARIO_ID_SHIP14NACKT   =  17,
	SCENARIO_ID_SHIPNORM      =  18,
	SCENARIO_ID_SHIP14        =  19,
	SCENARIO_ID_SHIPBLACK1    =  20,
	SCENARIO_ID_SHIPBLACK2    =  21,
	SCENARIO_ID_SHIPBLACK3    =  22,
	SCENARIO_ID_SHIP15        =  23,
	SCENARIO_ID_SHIP17        =  24,
	SCENARIO_ID_SHIP18        =  25,
	SCENARIO_ID_SHIP19        =  26,
	SCENARIO_ID_SHIP21        =  27,
	SCENARIO_ID_SHIP22        =  28,
	SCENARIO_ID_SHIP23        =  29,
	SCENARIO_ID_SHIP23B       =  30,
	SCENARIO_ID_SHIP23A       =  31,
	SCENARIO_ID_SHIP23C       =  32,
	SCENARIO_ID_SHIP24        =  33,
	SCENARIO_ID_SHIP10        =  34,
	SCENARIO_ID_F061_2        =  35,
	SCENARIO_ID_F061_3        =  36,
	SCENARIO_ID_LANDBLACK1    =  37,
	SCENARIO_ID_F0614         =  38,
	SCENARIO_ID_F061_5        =  39,
	SCENARIO_ID_F061_6        =  40,
	SCENARIO_ID_DUNGBLACK1    =  41,
	SCENARIO_ID_DUNGBLACK2    =  42,
	SCENARIO_ID_F108_1        =  43,
	SCENARIO_ID_F108_2        =  44,
	SCENARIO_ID_F108_3        =  45,
	SCENARIO_ID_F108_3A       =  46,
	SCENARIO_ID_F108_3B       =  47,
	SCENARIO_ID_F108_3C       =  48,
	SCENARIO_ID_F108_4        =  49,
	SCENARIO_ID_F108_6        =  50,
	SCENARIO_ID_F108_9        =  51,
	SCENARIO_ID_F108_7        =  52,
	SCENARIO_ID_F051_2        =  53,
	SCENARIO_ID_F051_3        =  54,
	SCENARIO_ID_F051_4        =  55,
	SCENARIO_ID_F051_5_1      =  56,
	SCENARIO_ID_F051_5_2      =  57,
	SCENARIO_ID_F051_5_3      =  58,
	SCENARIO_ID_F051_5_4      =  59,
	SCENARIO_ID_F051_05_4     =  60,
	SCENARIO_ID_F051_07       =  61,
	SCENARIO_ID_F051_09       =  62,
	SCENARIO_ID_F051_13       =  63,
	SCENARIO_ID_F108_01       =  64,
	SCENARIO_ID_F108_02       =  65,
	SCENARIO_ID_F108_03       =  66,
	SCENARIO_ID_F108_04       =  67,
	SCENARIO_ID_F108_06       =  68,
	SCENARIO_ID_F108_07       =  69,
	SCENARIO_ID_F108_09       =  70,
	SCENARIO_ID_F051_02       =  71,
	SCENARIO_ID_F051_03       =  72,
	SCENARIO_ID_F051_04       =  73,
	SCENARIO_ID_F051_05_1     =  74,
	SCENARIO_ID_F051_05_2     =  75,
	SCENARIO_ID_F051_05_3     =  76,
	SCENARIO_ID_F051_14       =  77,
	SCENARIO_ID_F051_15       =  78,
	SCENARIO_ID_F051_16       =  79,
	SCENARIO_ID_F051_16_1     =  80,
	SCENARIO_ID_F051_17       =  81,
	SCENARIO_ID_F051_18       =  82,
	SCENARIO_ID_F051_18_1     =  83,
	SCENARIO_ID_F051_118_1    =  84,
	SCENARIO_ID_F051_19       =  85,
	SCENARIO_ID_F051_20_2     =  86,
	SCENARIO_ID_F051_21_2     =  87,
	SCENARIO_ID_F046_01       =  88,
	SCENARIO_ID_F046_04       =  89,
	SCENARIO_ID_F046_06       =  90,
	SCENARIO_ID_F046_07       =  91,
	SCENARIO_ID_F046_10       =  92,
	SCENARIO_ID_F046_12       =  93,
	SCENARIO_ID_F046_13       =  94,
	SCENARIO_ID_F046_15       =  95,
	SCENARIO_ID_F046_16       =  96,
	SCENARIO_ID_F046_18       =  97,
	SCENARIO_ID_F051_18_3     =  98,
	SCENARIO_ID_F046_22       =  99,
	SCENARIO_ID_F046_24       = 100,
	SCENARIO_ID_F046_25       = 101,
	SCENARIO_ID_F046_26       = 102,
	SCENARIO_ID_F046_26_5     = 103,
	SCENARIO_ID_F046_27       = 104,
	SCENARIO_ID_F046_28       = 105,
	SCENARIO_ID_F046_31       = 106,
	SCENARIO_ID_F076_04       = 107,
	SCENARIO_ID_F076_06       = 108,
	SCENARIO_ID_F076_07       = 109,
	SCENARIO_ID_F076_10       = 110,
	SCENARIO_ID_F076_11       = 111,
	SCENARIO_ID_F076_13       = 112,
	SCENARIO_ID_F094_02       = 113,
	SCENARIO_ID_F094_05       = 114,
	SCENARIO_ID_F094_10       = 115,
	SCENARIO_ID_F094_13       = 116,
	SCENARIO_ID_F094_17       = 117,
	SCENARIO_ID_F094_19       = 118,
	SCENARIO_ID_F094_22       = 119,
	SCENARIO_ID_F100_01       = 120,
	SCENARIO_ID_F100_03       = 121,
	SCENARIO_ID_F100_05       = 122,
	SCENARIO_ID_F100_12       = 123,
	SCENARIO_ID_F100_13       = 124,
	SCENARIO_ID_F126_03       = 125,
	SCENARIO_ID_F126_07       = 126,
	SCENARIO_ID_F126_08       = 127,
	SCENARIO_ID_F126_09       = 128,
	SCENARIO_ID_F126_11       = 129,
	SCENARIO_ID_F126_13       = 130,
	SCENARIO_ID_F126_17       = 131,
	SCENARIO_ID_F126_18       = 132,
	SCENARIO_ID_F126_20       = 133,
	SCENARIO_ID_F126_22       = 134,
	SCENARIO_ID_F126_23       = 135,
	SCENARIO_ID_F126_25       = 136,
	SCENARIO_ID_F126_27       = 137,
	SCENARIO_ID_F126_28       = 138,
	SCENARIO_ID_F129_05       = 139,
	SCENARIO_ID_F129_08       = 140,
	SCENARIO_ID_F129_09       = 141,
	SCENARIO_ID_F129_18       = 142,
	SCENARIO_ID_F129_21       = 143,
	SCENARIO_ID_F129_23       = 144,
	SCENARIO_ID_F129_24       = 145,
	SCENARIO_ID_F129_25       = 146,
	SCENARIO_ID_F129_27       = 147,
	SCENARIO_ID_F129_28       = 148,
	SCENARIO_ID_F129_29       = 149,
	SCENARIO_ID_F129_30       = 150,
	SCENARIO_ID_F131_01       = 151,
	SCENARIO_ID_F131_04       = 152,
	SCENARIO_ID_F131_05       = 153,
	SCENARIO_ID_F131_06       = 154,
	SCENARIO_ID_F131_07       = 155,
	SCENARIO_ID_F131_08       = 156,
	SCENARIO_ID_F131_10       = 157,
	SCENARIO_ID_F131_11_1     = 158,
	SCENARIO_ID_F131_14       = 159,
	SCENARIO_ID_F131_14A      = 160,
	SCENARIO_ID_F131_16       = 161,
	SCENARIO_ID_DFIN12        = 162,
	SCENARIO_ID_DFIN16        = 163,
	SCENARIO_ID_DFIN18        = 164,
	SCENARIO_ID_DFIN26        = 165,
	SCENARIO_ID_DFIN046       = 166,
	SCENARIO_ID_DFIN46        = 167,
	SCENARIO_ID_DFIN27        = 168,
	SCENARIO_ID_DFIN28        = 169,
	SCENARIO_ID_DFIN30        = 170,
	SCENARIO_ID_DPRE10_1      = 171,
	SCENARIO_ID_DOBE07        = 172,
	SCENARIO_ID_DOBE09        = 173,
	SCENARIO_ID_DOBE11        = 174,
	SCENARIO_ID_DOBE20        = 175,
	SCENARIO_ID_DOBE19        = 176,
	SCENARIO_ID_DOBE22        = 177,
	SCENARIO_ID_DTHO03        = 178,
	SCENARIO_ID_DTHO05        = 179,
	SCENARIO_ID_DTHO06        = 180,
	SCENARIO_ID_DTHO09        = 181,
	SCENARIO_ID_DTHO10        = 182,
	SCENARIO_ID_DTHO13        = 183,
	SCENARIO_ID_DTHO15        = 184,
	SCENARIO_ID_DTHO16        = 185,
	SCENARIO_ID_DTHO18        = 186,
	SCENARIO_ID_DTHO19        = 187,
	SCENARIO_ID_DTHO20        = 188,
	SCENARIO_ID_DTHO21A       = 189,
	SCENARIO_ID_DTHO21B       = 190,
	SCENARIO_ID_DTHO23        = 191,
	SCENARIO_ID_DTHO25        = 192,
	SCENARIO_ID_DTHO27        = 193,
	SCENARIO_ID_DTHO43        = 194,
	SCENARIO_ID_DTHO48_1      = 195,
	SCENARIO_ID_DTHO49        = 196,
	SCENARIO_ID_DTHO50        = 197,
	SCENARIO_ID_DTHO53        = 198,
	SCENARIO_ID_DTHO55        = 199,
	SCENARIO_ID_DTHO56        = 200,
	SCENARIO_ID_DTHO57        = 201,
	SCENARIO_ID_DTHO58        = 202,
	SCENARIO_ID_DTHO59        = 203,
	SCENARIO_ID_DTHO60        = 204,
	SCENARIO_ID_DTHO61        = 205,
	SCENARIO_ID_F031          = 206,
	SCENARIO_ID_F035          = 207,
	SCENARIO_ID_F064          = 208,
	SCENARIO_ID_F066          = 209,
	SCENARIO_ID_F074          = 210,
	SCENARIO_ID_F075          = 211,
	SCENARIO_ID_F077          = 212,
	SCENARIO_ID_F080          = 213,
	SCENARIO_ID_F084          = 214,
	SCENARIO_ID_F099          = 215,
	SCENARIO_ID_F098          = 216,
	SCENARIO_ID_F101          = 217,
	SCENARIO_ID_F122          = 218,
	SCENARIO_ID_F144          = 219,
	SCENARIO_ID_CITY1         = 220,
	SCENARIO_ID_CITYBLACK     = 221,
	SCENARIO_ID_CITYBLACK1    = 222,
	SCENARIO_ID_CITY2         = 223,
	SCENARIO_ID_CITY3         = 224,
	SCENARIO_ID_CITY4         = 225,
	SCENARIO_ID_CAMP1         = 226,
	SCENARIO_ID_CAMP2         = 227,
	SCENARIO_ID_CAMPFIGHT4    = 228,
	SCENARIO_ID_SHIP10_1      = 229,
	SCENARIO_ID_F128_20       = 230,
	SCENARIO_ID_F131_01A      = 231,
	SCENARIO_ID_DTHO14        = 232,
	SCENARIO_ID_DTHO18A       = 233,
	SCENARIO_ID_DTHO20A       = 234,
	SCENARIO_ID_DTHO21        = 235,
	SCENARIO_ID_DTH021A       = 236,
	SCENARIO_ID_DFIN27B       = 237,
	SCENARIO_ID_F046_1B       = 238,
	SCENARIO_ID_F061_4B       = 239,
	SCENARIO_ID_WILD1         = 240,
	SCENARIO_ID_WILD2         = 241,
	SCENARIO_ID_WILD3         = 242,
	SCENARIO_ID_WILD4         = 243,
	SCENARIO_ID_WILD5         = 244,
	SCENARIO_ID_WILD6         = 245,
	SCENARIO_ID_WILD7         = 246,
	SCENARIO_ID_WILD8         = 247,
	SCENARIO_ID_S001          = 248,
	SCENARIO_ID_F129_17       = 249,
	SCENARIO_ID_F108_15       = 250,
	SCENARIO_ID_F129_124      = 251,
	SCENARIO_ID__END          = 252
};

enum {
	FIG_BACKGROUND_ID_NONE = 0,
	FIG_BACKGROUND_ID_1    = 1,
	FIG_BACKGROUND_ID_2    = 2,
	FIG_BACKGROUND_ID_3    = 3,
	FIG_BACKGROUND_ID_4    = 4,
	FIG_BACKGROUND_ID_6    = 6,
	FIG_BACKGROUND_ID_7    = 7
};

enum {
	AREA_TYPE_DUNGEON = 0,
	AREA_TYPE_TOWN    = 1,
	AREA_TYPE_NONE    = -1
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
	TOWN_TILE_STREET        =  0,
	TOWN_TILE_TEMPLE        =  1,
	TOWN_TILE_HOUSE_1       =  2, /* usually: stone wall, 3 stairs at the entrance, with a hatch under the window. */ /* dark brown */
	TOWN_TILE_HOUSE_2       =  3, /* usually: wooden blockhouse. red-white signs on the window shutters. */ //dark blue
	TOWN_TILE_HOUSE_3       =  4, /* usually: timber framework, window half-open. */ //olive green
	TOWN_TILE_HOUSE_4       =  5, /* usually: stone wall, red roof, trapezial ornament at the door. (there are exceptions in the city of Thorwal) */ /* dark purple */
	TOWN_TILE_WATER         =  6,
	TOWN_TILE_GRASS         =  7,
	TOWN_TILE_SIGNPOST      =  8,
	TOWN_TILE_INN_OR_TAVERN =  9,
	TOWN_TILE_MERCHANT      = 10, /* The lighthouse on Runin is also displayed in Grey */
	TOWN_TILE_SMITH         = 11,
	TOWN_TILE_HEALER        = 12,
	TOWN_TILE_BLACK_FINGER  = 13 /* The black tower in the city of Thorwal */
};

enum {
	DUNGEON_ID_NONE                       =  0,
	DUNGEON_ID_TOTENSCHIFF                =  1, /* random encounter in high seas */ /* wood */
	DUNGEON_ID_VERFALLENE_HERBERGE        =  2, /* Ottarje-Daspota */ /* marble */
	DUNGEON_ID_SPINNENHOEHLE              =  3, /* Ottarje-Skjal */ /* stone */
	DUNGEON_ID_WOLFSHOEHLE                =  4, /* Ottarje-Orvil */ /* stone */
	DUNGEON_ID_GOBLINHOEHLE               =  5, /* Felsteyn-Orkanger */ /* stone */
	DUNGEON_ID_DASPOTASCHATZ              =  6, /* Rybon-Thoss */ /* stone */
	DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS   =  7, /* Rybon-Thoss */ /* marble */
	DUNGEON_ID_ORKBEHAUSUNG               =  8, /* Phexcaer-Skelellen */ /* stone */
	DUNGEON_ID_KULTSTAETTE_DES_NAMENLOSEN =  9, /* on island Hjalland */ /* marble */
	DUNGEON_ID_DRACHENHORT                = 10, /* on island Runin */ /* stone */
	DUNGEON_ID_PIRATENHOEHLE              = 11, /* on island Manrek */ /* marble */
	DUNGEON_ID_ZWERGENFESTE               = 12, /* in town Oberorken */ /* marble */
	DUNGEON_ID_VERLASSENE_MINE            = 13, /* in town Prem */ /* marble */
	DUNGEON_ID_ZWINGFESTE                 = 14, /* in town Thorwal */ /* marble */
	DUNGEON_ID_HYGGELIKS_RUINE            = 15 /* Phexcaer-Vilnheim / Phexcaer-Skelellen */ /* marble */
};

enum {
	DNG_MENU_MODE_PLAIN       = 0,
	DNG_MENU_MODE_OPEN_DOOR   = 1,
	DNG_MENU_MODE_OPEN_CHEST  = 2,
	DNG_MENU_MODE_CLOSE_DOOR  = 3,
	DNG_MENU_MODE_LEVER       = 4,
	DNG_MENU_MODE_UNLOCK_DOOR = 5
};

enum {
	/* https://github.com/shihan42/BrightEyesWiki/wiki/DNG */
	DNG_TILE_CORRIDOR           =  0,
	DNG_TILE_CLOSED_DOOR        =  1, /* flag 1: 0 = 'locked' / 1 = 'unlocked' */
	DNG_TILE_OPEN_DOOR          =  2,
	DNG_TILE_STAIR_DOWN         =  3,
	DNG_TILE_STAIR_UP           =  4,
	DNG_TILE_PIT_IN_CEILING     =  5,
	DNG_TILE_PIT                =  6,
	DNG_TILE_BLOCKER            =  7, /* what is this? */
	DNG_TILE_CHEST              =  8, /* flag 1: 0 = 'closed' / 1 = 'opened' */
	DNG_TILE_SMASHED_DOOR       =  9,
	DNG_TILE_REMOVABLE_WALL     = 10,
	DNG_TILE_SEMIPERMEABLE_WALL = 11, /* 4 flags for the 4 directions. 0 = 'blocked', 1 = 'accessible' */
	DNG_TILE_FAKE_WALL          = 12,
	DNG_TILE_WALL               = 15,
};

enum {
	MAP_TILE_BLACK        =  0,
	MAP_TILE_RED          =  1,
	MAP_TILE_BRIGHT_GREEN =  2,
	MAP_TILE_BLUE         =  3,
	MAP_TILE_YELLOW_ARROW =  4,
	MAP_TILE_PINK         =  5,
	MAP_TILE_LIGHT_BLUE   =  6,
	MAP_TILE_CROSS        =  7,
	MAP_TILE_LIGHT_PURPLE =  8, /* unused? */
	MAP_TILE_DARK_GREEN   =  9,
	MAP_TILE_DARK_PURPLE  = 10,
	MAP_TILE_DARK_RED     = 11,
	MAP_TILE_ORANGE       = 12,
	MAP_TILE_DARK_PINK    = 13, /* unused? */
	MAP_TILE_OLIVE_GREEN  = 14, /* unused? */
	MAP_TILE_GREY         = 15,
	MAP_TILE_PURPLE_ARROW = 16,
	MAP_TILE_BROWN        = 17,
	MAP_TILE_GREEN        = 18,
	MAP_TILE_DARK_GREY    = 19,
	MAP_TILE_DARK_BLUE    = 20, /* unused? */
	MAP_TILE_DARK_BROWN   = 21 /* unused? */
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

enum { // 1-based
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
	JOURNEY_DIRECTION_CHANGE_TO_FORWARD  = -1,
	JOURNEY_DIRECTION_FORWARD            =  0,
	JOURNEY_DIRECTION_CHANGE_TO_BACKWARD =  1,
	JOURNEY_DIRECTION_BACKWARD           =  2
};

enum {
	CROSSLINK_STATUS_NONE           = 0,
	CROSSLINK_STATUS_FROM_KRAVIK    = 1,
	CROSSLINK_STATUS_FROM_PEILINEN  = 2,
	CROSSLINK_STATUS_FROM_SKELELLEN = 3,
	CROSSLINK_STATUS_FROM_ROVAMUND  = 4
};

enum {
	ARCHIVE_FILE_PLAYM_UK     =   0,
	ARCHIVE_FILE_KDBACK_DAT   =   1,
	ARCHIVE_FILE_KCBACK_DAT   =   2,
	ARCHIVE_FILE_KLBACK_DAT   =   3,
	ARCHIVE_FILE_KSBACK_DAT   =   4,
	ARCHIVE_FILE_KARTE_DAT    =   5,
	ARCHIVE_FILE_BICONS       =   6,
	ARCHIVE_FILE_OBJECTS_NVF  =   7,
	ARCHIVE_FILE_LROUT_DAT    =   8,
	ARCHIVE_FILE_SROUT_DAT    =   9,
	ARCHIVE_FILE_HSROUT_DAT   =  10,
	ARCHIVE_FILE_FONT6        =  11,
	ARCHIVE_FILE_FONT8        =  12,
	ARCHIVE_FILE_GGSTS_NVF    =  13,
	ARCHIVE_FILE_TEXT_LTX     =  14,
	ARCHIVE_FILE_ICONS        =  15,
	ARCHIVE_FILE_MONSTER      =  16,
	ARCHIVE_FILE_MONSTER_TAB  =  17,
	ARCHIVE_FILE_COMPASS      =  18,
	ARCHIVE_FILE_MAPTEXT_LTX  =  19,
	ARCHIVE_FILE_ZUSTA_UK     =  20,
	ARCHIVE_FILE_FIGHTTXT_LTX =  21,
	ARCHIVE_FILE_ANIS         =  22,
	ARCHIVE_FILE_ANIS_TAB     =  23,
	ARCHIVE_FILE_MFIGS        =  24,
	ARCHIVE_FILE_MFIGS_TAB    =  25,
	ARCHIVE_FILE_CITY_DAT     =  26,
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
	ARCHIVE_FILE_CITY_LTX     =  77,
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
	ARCHIVE_FILE_TAVERN_TLK   = 130,
	ARCHIVE_FILE_SPSTAR_NVF   = 131,
	ARCHIVE_FILE_GUERTEL_NVF  = 132,
	ARCHIVE_FILE_HAFEN_LTX    = 133,
	ARCHIVE_FILE_FACE_NVF     = 134,
	ARCHIVE_FILE_HYGBACK_NVF  = 135,
	ARCHIVE_FILE_HYGGELIK_NVF = 136,
	ARCHIVE_FILE_SKULL_NVF    = 137,
	ARCHIVE_FILE_OUTRO1_NVF   = 138,
	ARCHIVE_FILE_OUTRO2_NVF   = 139,
	ARCHIVE_FILE_OUTRO3_NVF   = 140,
	ARCHIVE_FILE_TOWNPAL_DAT  = 141,
	ARCHIVE_FILE_THORWAL_XMI  = 142,
	ARCHIVE_FILE_SUMMARY_XMI  = 143,
	ARCHIVE_FILE_TEMPLE_XMI   = 144,
	ARCHIVE_FILE_TERMS_XMI    = 145,
	ARCHIVE_FILE_SMITH_XMI    = 146,
	ARCHIVE_FILE_INN_XMI      = 147,
	ARCHIVE_FILE_HEALER_XMI   = 148,
	ARCHIVE_FILE_CAMP_XMI     = 149,
	ARCHIVE_FILE_VICTORY_XMI  = 150,
	ARCHIVE_FILE_SAMPLE_AD    = 151,
	ARCHIVE_FILE_ATTIC        = 152,
	ARCHIVE_FILE_POPUP_DAT    = 153,
	ARCHIVE_FILE_154          = 154,
	ARCHIVE_FILE_155          = 155,
	ARCHIVE_FILE_DIALOGS_TLK  = 156,
	/*
		F092.TLK, HERBERG.TLK, 158, DASPOTA1.TLK, DASPOTA2.TLK, DASPOTA3.TLK,
		INFO1.TLK, INFO2.TLK, INFO3.TLK, INFO4.TLK, INFO5.TLK, INFO6.TLK,
		INFO7.TLK, SCHMIED.TLK, GHANDEL.TLK, KHANDEL.TLK, WHANDEL.TLK, F115.TLK,
		EREMIT.TLK, HYGGELIK.TLK
	*/
	ARCHIVE_FILE_WEAPONS_NVF  = 176,
	ARCHIVE_FILE_BUCH_DAT     = 177,
	ARCHIVE_FILE_SPELLOBJ_NVF = 178,
	ARCHIVE_FILE_IN_HEADS_NVF = 179,
	ARCHIVE_FILE_TEMPICON     = 180,
	ARCHIVE_FILE_SHIPSL_NVF   = 181,
	ARCHIVE_FILE_STONESL_NVF  = 182,
	ARCHIVE_FILE_MARBLESL_NVF = 183,
	ARCHIVE_FILE_TFLOOR1_NVF  = 184,
	ARCHIVE_FILE_TDIVERSE_NVF = 185,
	ARCHIVE_FILE_HOUSE1_NVF   = 186,
	ARCHIVE_FILE_HOUSE2_NVF   = 187,
	ARCHIVE_FILE_HOUSE3_NVF   = 188,
	ARCHIVE_FILE_HOUSE4_NVF   = 189,
	ARCHIVE_FILE_FEATURE_LTX  = 190,
	ARCHIVE_FILE_FEATURE1_LTX = 191,
	ARCHIVE_FILE_FEATURE2_LTX = 192,
	ARCHIVE_FILE_FEATURE3_LTX = 193,
	ARCHIVE_FILE_FEATURE4_LTX = 194,
	ARCHIVE_FILE_FEATURE5_LTX = 195,
	ARCHIVE_FILE_FEATURE6_LTX = 196,
	ARCHIVE_FILE_FEATURE7_LTX = 197,
	ARCHIVE_FILE_FEATURE8_LTX = 198,
	ARCHIVE_FILE_FIGHTOBJ_NVF = 199,
	ARCHIVE_FILE_SCENARIO_LST = 200,
	ARCHIVE_FILE_KDLBACK_DAT  = 201,
	ARCHIVE_FILE_KCLBACK_DAT  = 202,
	ARCHIVE_FILE_KLLBACK_DAT  = 203,
	ARCHIVE_FILE_KSLBACK_DAT  = 204,
	ARCHIVE_FILE_FIGHT_LST    = 205,
	ARCHIVE_FILE_SKARTE_NVF   = 206,
	ARCHIVE_FILE_GAMES_NAM    = 207,
	ARCHIVE_FILE_ITEMNAME     = 208,
	ARCHIVE_FILE_MONNAMES     = 209,
	ARCHIVE_FILE_THORWAL1_LTX = 210,
	ARCHIVE_FILE_THORWAL2_LTX = 211,
	ARCHIVE_FILE_DUNGEON_XMI  = 212,
	ARCHIVE_FILE_COMBAT_XMI   = 213,
	ARCHIVE_FILE_PLAYM_US     = 214,
	ARCHIVE_FILE_ZUSTA_US     = 215,
	ARCHIVE_FILE_ANI_DAT      = 216,
	ARCHIVE_FILE_WEAPANI_DAT  = 217,
	ARCHIVE_FILE_MONSTER_DAT  = 218,
	ARCHIVE_FILE_BSKILLS_DAT  = 219,
	ARCHIVE_FILE_ITEMS_DAT    = 220,
	ARCHIVE_FILE_CHARTEXT_LTX = 221,
	ARCHIVE_FILE_SPELLTXT_LTX = 222,
	ARCHIVE_FILE_WONDER_LTX   = 223,
	ARCHIVE_FILE_STRASSE_LTX  = 224,
	ARCHIVE_FILE_NSC_LTX      = 225,
	ARCHIVE_FILE_NPCS         = 226,
	/*
		NARIELL.NPC, HARIKA.NPC, CURIAN.NPC, ARDORA.NPC, GARSVIK.NPC, ERWO.NPC
	*/
	ARCHIVE_FILE_FINGER_NVF   = 232,
	ARCHIVE_FILE_LTURM_NVF    = 233,
	ARCHIVE_FILE_WFIGS        = 234,
	ARCHIVE_FILE_WFIGS_TAB    = 235,
	ARCHIVE_FILE_SPLASHES_DAT = 236,
	ARCHIVE_FILE_FEATURE9_LTX = 237,
	ARCHIVE_FILE_PHEX1_LTX    = 238,
	ARCHIVE_FILE_PHEX2_LTX    = 239,
	ARCHIVE_FILE_WILD_LTX     = 240,
	ARCHIVE_FILE_WAFFINFO_LTX = 241,
	ARCHIVE_FILE_DNGS         = 242,
	/*
		SHIP.DNG, F046.DNG, F051.DNG, F061.DNG, F076.DNG, F094.DNG, F100.DNG, F108.DNG,
		F126.DNG, F129.DNG, F131.DNG, OBER.DNG, PREM.DNG, THORWAL.DNG, FINAL.DNG
	*/
	ARCHIVE_FILE_DNGS_DDT     = 256,
	/*
		SHIP.DDT, F046.DDT, F051.DDT, F061.DDT, F076.DDT, F094.DDT, F100.DDT, F108.DDT,
		F126.DDT, F129.DDT, F131.DDT, OBER.DDT, PREM.DDT, THORWAL.DDT, FINAL.DDT
	*/
	ARCHIVE_FILE_DNGS_DTX     = 271,
	/*
		SHIP.DTX, F046.DTX, F051.DTX, F061.DTX, F076.DTX, F094.DTX, F100.DTX, F108.DTX,
		F126.DTX, F129.DTX, F131.DTX, OBER.DTX, PREM.DTX, THORWAL.DTX, FINAL.DTX
	*/
	ARCHIVE_FILE_TFLOOR2_NVF  = 287,
	ARCHIVE_FILE_STORY_LTX    = 288,
	ARCHIVE_FILE_FX1_VOC      = 289,
	ARCHIVE_FILE_FX2_VOC      = 290,
	ARCHIVE_FILE_FX3_VOC      = 291,
	ARCHIVE_FILE_FX4_VOC      = 292,
	ARCHIVE_FILE_FX5_VOC      = 293,
	ARCHIVE_FILE_FX6_VOC      = 294,
	ARCHIVE_FILE_FX7_VOC      = 295,
	ARCHIVE_FILE_FX8_VOC      = 296,
	ARCHIVE_FILE_FX9_VOC      = 297,
	ARCHIVE_FILE_FX10_VOC     = 298,
	ARCHIVE_FILE_FX11_VOC     = 299,
	ARCHIVE_FILE_FX12_VOC     = 300,
	ARCHIVE_FILE_FX13_VOC     = 301,
	ARCHIVE_FILE_FX14_VOC     = 302,
	ARCHIVE_FILE_FX15_VOC     = 303,
	ARCHIVE_FILE_FX16_VOC     = 304,
	ARCHIVE_FILE_FX17_VOC     = 305,
	ARCHIVE_FILE_FX18_VOC     = 306,
	ARCHIVE_FILE_OUTRO_XMI    = 307
};

enum {
	ITEM_ID_NONE                              =   0, /* nothing / Bloße Hand (bare hand) */
	ITEM_ID_SCHWERT                           =   1, /* sword */
	ITEM_ID_KNUEPPEL                          =   2, /* club */
	ITEM_ID_SAEBEL                            =   3, /* saber */
	ITEM_ID_MESSER                            =   4, /* knife */
	ITEM_ID_SPEER                             =   5, /* spear */
	ITEM_ID_KURZSCHWERT                       =   6, /* short sword */
	ITEM_ID_SCHILD                            =   7, /* shield */
	ITEM_ID_KRIEGSBEIL__SPECIAL               =   8, /* hatchet [special "Schicksalsklinge" variant, also for witches, druids and mages, 1D6 + 4, KK+14, BF 5, AT-0, PA-3, weight 120, price 45] */
	ITEM_ID_KURZBOGEN                         =   9, /* shortbow */
	ITEM_ID_PFEIL                             =  10, /* arrow */
	ITEM_ID_STREITAXT                         =  11, /* battleaxe */
	ITEM_ID_ARMBRUST                          =  12, /* crossbow */
	ITEM_ID_BOLZEN                            =  13, /* bolt */
	ITEM_ID_DOLCH                             =  14, /* dagger */
	ITEM_ID_EISENSCHILD                       =  15, /* iron shield */
	ITEM_ID_WURFBEIL                          =  16, /* francesca */
	ITEM_ID_WURFSTERN                         =  17, /* throwing star */
	ITEM_ID_ZWEIHAENDER                       =  18, /* two-handed sword */
	ITEM_ID_LANGBOGEN                         =  19, /* longbow */
	ITEM_ID_MORGENSTERN                       =  20, /* morning star */
	ITEM_ID_VULKANGLASDOLCH                   =  21, /* volcanic glass dagger */
	ITEM_ID_FACKEL__LIT                       =  22, /* torch [lit] */
	ITEM_ID_BIER                              =  23, /* beer */
	ITEM_ID_WURFHAKEN                         =  24, /* grapple */
	ITEM_ID_LATERNE__UNLIT                    =  25, /* lantern [unlit] */
	ITEM_ID_BRECHEISEN                        =  26, /* crowbar */
	ITEM_ID_HAMMER                            =  27, /* hammer */
	ITEM_ID_ANGELHAKEN                        =  28, /* fishhook */
	ITEM_ID_SCHRIFTROLLE                      =  29, /* scroll */
	ITEM_ID_WASSERSCHLAUCH                    =  30, /* waterskin */
	ITEM_ID_GLASFLASCHE                       =  31, /* flask glass */
	ITEM_ID_STRICKLEITER                      =  32, /* rope ladder */
	ITEM_ID_WURFAXT                           =  33, /* throwing axe */
	ITEM_ID_MESSINGSPIEGEL                    =  34, /* brass mirror */
	ITEM_ID_DIETRICHE                         =  35, /* picklocks */
	ITEM_ID_SCHREIBZEUG                       =  36, /* writing material */
	ITEM_ID_HARFE                             =  37, /* harp */
	ITEM_ID_TRINKHORN                         =  38, /* drinking horn */
	ITEM_ID_SILBERSCHMUCK                     =  39, /* silver jewelry */
	ITEM_ID_KLETTERHAKEN                      =  40, /* climbing hooks */
	ITEM_ID_OEL                               =  41, /* oil */
	ITEM_ID_BRONZEFLASCHE                     =  42, /* flask bronze */
	ITEM_ID_EISENHELM                         =  43, /* iron helmet */
	ITEM_ID_PIKE                              =  44, /* pike */
	ITEM_ID_PROVIANTPAKET                     =  45, /* food package */
	ITEM_ID_FLOETE                            =  46, /* flute */
	ITEM_ID_ALCHIMIESET                       =  47, /* alchemy kit */
	ITEM_ID_HEMD                              =  48, /* shirt */
	ITEM_ID_HOSE                              =  49, /* trousers */
	ITEM_ID_SCHUHE                            =  50, /* shoes */
	ITEM_ID_STIEFEL                           =  51, /* boots */
	ITEM_ID_SCHNEESCHUHE                      =  52, /* laceup shoe */
	ITEM_ID_LEDERHARNISCH                     =  53, /* leather armor */
	ITEM_ID_SCHUPPENRUESTUNG                  =  54, /* scale armor */
	ITEM_ID_SHURINKNOLLEKNOLLENGIFT           =  55, /* Shurin poison */
	ITEM_ID_ARAXGIFT                          =  56, /* Arax poison */
	ITEM_ID_ANGSTGIFT                         =  57, /* angst gift */
	ITEM_ID_SCHLAFGIFT                        =  58, /* sleep poison */
	ITEM_ID_GOLDLEIM                          =  59, /* Goldleim */
	ITEM_ID_EINBEERE                          =  60, /* Einbeere */
	ITEM_ID_WIRSELKRAUT                       =  61, /* whirlweed */
	ITEM_ID_EITRIGER_KROETENSCHEMEL           =  62, /* Eitriger Krötenschemel */
	ITEM_ID_GULMOND_BLATT                     =  63, /* Gulmond leaf */
	ITEM_ID_TARNELE                           =  64, /* Tarnele */
	ITEM_ID_FACKEL__UNLIT                     =  65, /* torch [unlit] */
	ITEM_ID_STREITKOLBEN                      =  66, /* mace */
	ITEM_ID_DEGEN                             =  67, /* epee */
	ITEM_ID_FLORETT                           =  68, /* floret */
	ITEM_ID_KAMPFSTAB                         =  69, /* quarterstaff */
	ITEM_ID_KRISTALLKUGEL                     =  70, /* crystal ball */
	ITEM_ID_PEITSCHE                          =  71, /* whip */
	ITEM_ID_DECKE                             =  72, /* blanket */
	ITEM_ID_SCHAUFEL                          =  73, /* shovel */
	ITEM_ID_GOLDSCHMUCK                       =  74, /* gold jewelry */
	ITEM_ID_ROBE__GREEN_1                     =  75, /* robe [green, cheap] */
	ITEM_ID_ROBE__RED                         =  76, /* robe [red, expensive] */
	ITEM_ID_TOPFHELM                          =  77, /* pot helmet */
	ITEM_ID_LEDERHELM                         =  78, /* leather helmet */
	ITEM_ID_WAFFENROCK                        =  79, /* surcoat */
	ITEM_ID_KETTENHEMD                        =  80, /* chain mail */
	ITEM_ID_KROETENHAUT                       =  81, /* toad skin */
	ITEM_ID_PLATTENZEUG                       =  82, /* plate gear */
	ITEM_ID_KETTENZEUG                        =  83, /* chain gear */
	ITEM_ID_LEDERZEUG                         =  84, /* leather gear */
	ITEM_ID_ZUNDERKAESTCHEN                   =  85, /* tinderbox */
	ITEM_ID_SCHLEIFSTEIN                      =  86, /* grindstone */
	ITEM_ID_ESSBESTECK                        =  87, /* cutlery */
	ITEM_ID_ESSGESCHIRR                       =  88, /* dishes */
	ITEM_ID_LAKRITZE                          =  89, /* licorice */
	ITEM_ID_BONBONS                           =  90, /* bonbons */
	ITEM_ID_WEINFLASCHE                       =  91, /* wine */
	ITEM_ID_SCHNAPSFLASCHE                    =  92, /* brandy */
	ITEM_ID_HACKE                             =  93, /* pickaxe */
	ITEM_ID_PRAIOS_AMULETT                    =  94, /* Praios amulet [magic; found in Spinnenhoehle] */
	ITEM_ID_LAUTE                             =  95, /* lute */
	ITEM_ID_WINTERMANTEL                      =  96, /* winter coat */
	ITEM_ID_NETZ                              =  97, /* net */
	ITEM_ID_WURFMESSER                        =  98, /* throwing knife */
	ITEM_ID_SICHEL                            =  99, /* sickle */
	ITEM_ID_SENSE                             = 100, /* scythe */
	ITEM_ID_KRIEGSHAMMER                      = 101, /* war hammer */
	ITEM_ID_DREIZACK                          = 102, /* trident */
	ITEM_ID_HELLEBARDE                        = 103, /* halberd */
	ITEM_ID_DRESCHFLEGEL                      = 104, /* flail */
	ITEM_ID_ZWEILILIEN                        = 105, /* Zweililien */
	ITEM_ID_OCHSENHERDE                       = 106, /* oxen herd */
	ITEM_ID_BASILISKENZUNGE                   = 107, /* basilisk tongue */
	ITEM_ID_OGERFAENGER                       = 108, /* ogre catcher */
	ITEM_ID_MENGBILAR                         = 109, /* Mengbilar */
	ITEM_ID_SCHWERER_DOLCH                    = 110, /* heavy dagger */
	ITEM_ID_RONDRAKAMM                        = 111, /* Rondra comb */
	ITEM_ID_ENTERMESSER                       = 112, /* cutlass */
	ITEM_ID_BASTARDSCHWERT                    = 113, /* bastard sword */
	ITEM_ID_TUZAKMESSER                       = 114, /* Tuzak knife */
	ITEM_ID_RABENSCHNABEL                     = 115, /* raven's beak */
	ITEM_ID_BRABAKBENGEL                      = 116, /* Brabakbengel */
	ITEM_ID_RAPIER                            = 117, /* rapier */
	ITEM_ID_KHUNCHOMER                        = 118, /* Khunchomer */
	ITEM_ID_DOPPELKHUNCHOMER                  = 119, /* double Khunchomer */
	ITEM_ID_SCHLEUDER                         = 120, /* sling */
	ITEM_ID_SEIL                              = 121, /* rope */
	ITEM_ID_SHURINKNOLLE                      = 122, /* Shurink bulb */
	ITEM_ID_BELMART_BLATT                     = 123, /* Belmart leaf */
	ITEM_ID_DONFSTENGEL                       = 124, /* Donf spring */
	ITEM_ID_MENCHALKAKTUS                     = 125, /* Menchal cactus */
	ITEM_ID_ALRAUNE                           = 126, /* mandrake */
	ITEM_ID_ATMONBLUETE                       = 127, /* Atmon bloom */
	ITEM_ID_ILMENBLATT                        = 128, /* Ilmen leaf */
	ITEM_ID_FINAGEBAEMCHEN                    = 129, /* Finage tree */
	ITEM_ID_JORUGAWURZEL                      = 130, /* Joruga root */
	ITEM_ID_THONNYSBLUETE                     = 131, /* Thonnys bloom */
	ITEM_ID_LOTUSBLUETE                       = 132, /* Lotus bloom */
	ITEM_ID_ZAUBERSTAB                        = 133, /* magic wand */
	ITEM_ID_SKRAJA                            = 134, /* Skraja */
	ITEM_ID_KRIEGSBEIL                        = 135, /* hatchet [the common one, 1D6 + 4, KK+13, BF 2, AT-2, PA-4] */
	ITEM_ID_ORKNASE                           = 136, /* orc nose */
	ITEM_ID_SCHNEIDZAHN                       = 137, /* cutting tooth */
	ITEM_ID_ROBBENTOETER                      = 138, /* seal killer */
	ITEM_ID_WOLFSMESSER                       = 139, /* wolf knive */
	ITEM_ID_HEXENBESEN                        = 140, /* witches broom */
	ITEM_ID_LOTUSGIFT                         = 141, /* Lotus poison */
	ITEM_ID_KUKRIS                            = 142, /* Kukris */
	ITEM_ID_BANNSTAUB                         = 143, /* Bannstaub */
	ITEM_ID_KROETENSCHEMELGIFT                = 144, /* Krötenschemel poison */
	ITEM_ID_HEILTRANK                         = 145, /* LE potion */
	ITEM_ID_STARKER_HEILTRANK                 = 146, /* strong LE potion */
	ITEM_ID_MU_ELIXIER                        = 147, /* MU Elixier */
	ITEM_ID_KL_ELIXIER                        = 148, /* KL Elixier */
	ITEM_ID_CH_ELIXIER                        = 149, /* CH Elixier */
	ITEM_ID_FF_ELIXIER                        = 150, /* FF Elixier */
	ITEM_ID_GE_ELIXIER                        = 151, /* GE Elixier */
	ITEM_ID_IN_ELIXIER                        = 152, /* IN Elixier */
	ITEM_ID_KK_ELIXIER                        = 153, /* KK Elixier */
	ITEM_ID_ZAUBERTRANK                       = 154, /* AE potion */
	ITEM_ID_ZAUBERTRANK_STARK                 = 155, /* strong AE potion */
	ITEM_ID_OLGINWURZEL                       = 156, /* Olgin root */
	ITEM_ID_KAIRANHALM                        = 157, /* Kairan stem */
	ITEM_ID_BASTARDSCHWERT__MAGIC             = 158, /* bastard sword [magic: lower BF; found in Daspota] */
	ITEM_ID_ORKNASE__MAGIC                    = 159, /* orc nose [magic: damage +5, AT-1, PA-5; found in Daspota] */
	ITEM_ID_KURZSCHWERT__MAGIC                = 160, /* short sword [magic: AT + 2, unbreakable; found in Daspota ] */
	ITEM_ID_SICHEL__MAGIC                     = 161, /* sickle [magic, while in inventory: Pflanzenkunde + 3; found in Daspota] */
	ITEM_ID_AMULETT__ANTI_IGNIFAXIUS          = 162, /* amulet [6 blue gems; found at a corpse in tevent014]. Analues: "Dient der Abwehr von Ignifaxius". apparently, no effect is implemented. */
	ITEM_ID_AMULETT__MR_BONUS                 = 163, /* amulet [yellow star, magic: while in inventory MR + 5; found in FIGHT_ID_F100_13 in DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS and ruin of the black wizard and at FIGHT_ID_F035 on the route Phexcaer <-> Einsiedlersee] */
	ITEM_ID_KNOCHEN_MIT_RUNE                  = 164, /* bone with rune [found in Gorah's chest] */
	ITEM_ID_RING__RED                         = 165, /* ring [red, magic, MR + 2. additionally 5 charges of Arcano Psychostabilis (additionally MR + 2) when used; found in Gorah's chest ] */
	ITEM_ID_EXPURGICUM                        = 166, /* Expurgicum */
	ITEM_ID_REZEPT_FUER_EXPURGICUM            = 167, /* recipe for Expurgicum */
	ITEM_ID_VOMICUM                           = 168, /* Vomicum */
	ITEM_ID_REZEPT_FUER_VOMICUM               = 169, /* recipe for Vomicum */
	ITEM_ID_DOKUMENT__UNICORN                 = 170, /* unicorn letter [found in tevent073] */
	ITEM_ID_SILBERNER_STIRNREIF__1            = 171, /* silver coronet [magic, 3 charges, use -> Armatrutz +5, i.e. RS + 5] */
	ITEM_ID_SAEBEL__MAGIC                     = 172, /* sabre [magic, damage+1 to skeletons and zombies] */
	ITEM_ID_AMULETT__FEUERBANN                = 173, /* amulet [6 blue gems, magic, protection from fire; found at a corpse between Rovamund and Nordvest. Is there another one on the Totenschiff? ] */
	ITEM_ID_AMULETT__FLIM_FLAM                = 174, /* amulet [6 blue gems, magic, 6 charges: use -> Flim Flam; found in Totenschiff.] */
	ITEM_ID_AMULETT__TRAVIA                   = 175, /* amulet [golden cross, magic: no hunger or thirst; found in Wolfshoehle ] */
	ITEM_ID_SCHULDBUCH                        = 176, /* debt register [found in Wolfshoehle] */
	ITEM_ID_ROTE_MONDSCHEIBE                  = 177, /* red moon disk [found in Orkhoehle; red herring] */
	ITEM_ID_ZWEIHAENDER__MAGIC                = 178, /* two-handed sword [magic: unbreakable; found in Orkhoehle] */
	ITEM_ID_ORKDOKUMENT                       = 179, /* orc document [found in Orkhoehle] */
	ITEM_ID_GEGENGIFT                         = 180, /* antivenom */
	ITEM_ID_DAS_SCHWERT_GRIMRING                          = 181, /* Grimring [found at Hyggelik's ruin] */
	ITEM_ID_TOTENKOPFGUERTEL                  = 182, /* skull belt [magic: TA - 4; found in Verfallene Herberge ] */
	ITEM_ID_KRAFTGUERTEL                      = 183, /* girdle of might [magic: KK + 5; found in Drachenhoehle] */
	ITEM_ID_MAGISCHER_BROTBEUTEL              = 184, /* magic breadbag [cannot be found in the game] */
	ITEM_ID_MAGISCHER_WASSERSCHLAUCH          = 185, /* magic waterskin [cannot be found in the game] */
	ITEM_ID_REZEPT_FUER_HEILTRANK             = 186, /* recipe for heal potion */
	ITEM_ID_SCHREIBEN_VON_JADRA               = 187, /* writing of Jarda */
	ITEM_ID_AMULETT__ORDINARY                 = 188, /* amulet [golden cross. not magic. found in fight F094_02 in DUNGEON_ID_DASPOTASCHATZ ] */
	ITEM_ID_LOBPREISUNGEN                     = 189, /* praises of the nameless god [found in Tempel des Namenlosen and in Piratenhoehle] */
	ITEM_ID_MITGLIEDERLISTE                   = 190, /* member list [found in Tempel des Namenlosen] */
	ITEM_ID_DOKUMENT__2                       = 191, /* document [2, not readable. Depotschein?] */
	ITEM_ID_SEEKARTE                          = 192, /* sea chart [found in Piratenhoehle] */
	ITEM_ID_KUKRISDOLCH                       = 193, /* Kukris dagger */
	ITEM_ID_SCHLUESSEL__YELLOW                = 194, /* key [yellow] */ /* Found in FIGHT_ID_DTHO21A, FIGHT_ID_DTHO21B, FIGHT_ID_DTH021A in DUNGEON_ID_ZWINGFESTE */
	ITEM_ID_GOLDENER_SCHLUESSEL               = 195, /* golden key [found in the dungeon of the Daspota treasure] / note that there is also ITEM_ID_GOLDSCHLUESSEL */
	ITEM_ID_HELM__CURSED                      = 196, /* helmet [cursed: CH - 1; found in goblin cave and at NPC Ardora von Greifenfurt ] */
	ITEM_ID_KETTENHEMD__CURSED                = 197, /* chain mail [magic, cursed, protection +1 vs. skeletons and zombies (arising as damage-3, but protection-2), but protection -2 vs. everything else, additionally drains LE; found in Drachenhoehle] */
	ITEM_ID_KETTENHEMD__MAGIC                 = 198, /* chain mail [magic, AT+1, BE-1, RS+1; found in the dungeon of the Daspota treasure (Rybon-Thoss)] */
	ITEM_ID_SCHWERT__MAGIC                    = 199, /* sword [magic: AT + 2; found in Drachenhoehle] */
	ITEM_ID_KUKRIS_MENGBILAR                  = 200, /* Kukris mengbilar */
	ITEM_ID_PLATINSCHLUESSEL                  = 201, /* platin key */ /* found in FIGHT_ID_F129_29 in DUNGEON_ID_DRACHENHORT */
	ITEM_ID_REZEPT_FUER_GEGENGIFT             = 202, /* recipe for antivenom */
	ITEM_ID_REZEPT_FUER_HYLAILER_FEUER        = 203, /* recipe for Hylailer Feuer */
	ITEM_ID_REZEPT_FUER_KRAFTELIXIER          = 204, /* recipe for KK elixir */
	ITEM_ID_REZEPT_FUER_MUTELIXIER            = 205, /* recipe for MU elixir */
	ITEM_ID_REZEPT_FUER_ZAUBERTRANK           = 206, /* recipe for AE potion */
	ITEM_ID_RING__BLUE                        = 207, /* ring [blue, not magic, found in Zwingfeste] */
	ITEM_ID_SCHLUESSEL__SILVER                = 208, /* key [silver] */
	ITEM_ID_EISENSCHLUESSEL__1                = 209, /* iron key [1] */ /* Found in FIGHT_ID_F131_10 in DUNGEON_ID_PIRATENHOEHLE */
	ITEM_ID_EISENSCHLUESSEL__2                = 210, /* iron key [2] */ /* Found in FIGHT_ID_F131_14A and FIGHT_ID_F131_14B in DUNGEON_ID_PIRATENHOEHLE */
	ITEM_ID_ROTER_SCHLUESSEL                  = 211, /* red key */ /* found in FIGHT_ID_F126_28 in DUNGEON_ID_KULTSTAETTE_DES_NAMENLOSEN */
	ITEM_ID_BRONZESCHLUESSEL                  = 212, /* bronze key [red] */ /* found in FIGHT_ID_F108_9 in DUNGEON_ID_ORKBEHAUSUNG */
	ITEM_ID_SILBERHELM                        = 213, /* silver helmet [magic: can be worn by all hero classes; found in Wolfshoehle] */
	ITEM_ID_SILBERSTREITKOLBEN                = 214, /* silver mace [magic; found in Verfallene Herberge] */
	ITEM_ID_SILBERSCHMUCK__MAGIC              = 215, /* silver jewelry [magic: TA - 2; found in Verfallene Herberge] */
	ITEM_ID_SPEER__MAGIC                      = 216, /* spear [magic; found in Spinnenhoehle] */
	ITEM_ID_STIRNREIF__MR_BONUS               = 217, /* coronet [magic: MR + 2; found in Tempel des Namenlosen] */
	ITEM_ID_WURFDOLCH__MAGIC                  = 218, /* throwing dagger [magic, improved accuracy, damage 1D6 + 9; found in Drachenhoehle]. Note that there is no 'ordinary' throwing dagger in the game. See the discussion at https://crystals-dsa-foren.de/showthread.php?pid=170046 */
	ITEM_ID_GOLDSCHLUESSEL                    = 219, /* gold key. note that there is also ITEM_ID_GOLDENER_SCHLUESSEL (golden key) */
	ITEM_ID_RING__GREEN                       = 220, /* ring [green, magic: protection from magic fire; found in Drachenhoehle] */
	ITEM_ID_BEUTEL                            = 221, /* bag [found and used in the first level of the ruin of the black wizard to open a passage] */
	ITEM_ID_BUCH__PIRATE_ACCOUNTING           = 222, /* book [dacades old accounting book about loot income of Daspota pirates] */
	ITEM_ID_ANTIKRANKHEITSELIXIER             = 223, /* anti disease elixir */
	ITEM_ID_BUCH__KAISERSPRUECHE_HALS         = 224, /* book ["Kaisersprüche Hals"; found in the dungeon of the Daspota treasure (Rybon-Thoss)] */
	ITEM_ID_KRISTALL                          = 225, /* crystal */
	ITEM_ID_MU_ELIXIER__BAD                   = 226, /* MU elixir [bad] */
	ITEM_ID_KL_ELIXIER__BAD                   = 227, /* KL elixir [bad] */
	ITEM_ID_CH_ELIXIER__BAD                   = 228, /* CH elixir [bad] */
	ITEM_ID_FF_ELIXIER__BAD                   = 229, /* FF elixir [bad] */
	ITEM_ID_GE_ELIXIER__BAD                   = 230, /* GE elixir [bad] */
	ITEM_ID_IN_ELIXIER__BAD                   = 231, /* IN elixir [bad] */
	ITEM_ID_KK_ELIXIER__BAD                   = 232, /* KK elixir [bad] */
	ITEM_ID_ROBE__GREEN_2                     = 233, /* robe [green] */
	ITEM_ID_GOLDENER_SCHILD                   = 234, /* golden shield [magic: can be equiped by all hero classes; found in Tempel des Namenlosen ] */
	ITEM_ID_EMPFEHLUNGSSCHREIBEN__HETMAN      = 235, /* commendation letter [Hetman] */
	ITEM_ID_WUNDERKUR                         = 236, /* Wunderkur */
	ITEM_ID_SCHLAFTRUNK                       = 237, /* sleep potion */
	ITEM_ID_MIASTHMATICUM                     = 238, /* Miasthmaticum */
	ITEM_ID_HYLAILER_FEUER                    = 239, /* Hylailer Feuer */
	ITEM_ID_REZEPT_FUER_STARKEN_HEILTRANK     = 240, /* recipe for strong LE potion [given by Olimone, tevent_114 Phexcaer <-> Skelellen: a swamp ]*/
	ITEM_ID_REZEPT_FUER_WUNDERKUR             = 241, /* recipe for Wunderkur */
	ITEM_ID_REZEPT_FUER_SCHLAFTRUNK           = 242, /* recipe for sleeping potion */
	ITEM_ID_REZEPT_FUER_STARKER_ZAUBERTRANK   = 243, /* recipe for strong AE potion */
	ITEM_ID_REZEPT_FUER_MIASTHMATICUM         = 244, /* recipe for Miasthmaticum */
	ITEM_ID_SILBERNER_STIRNREIF__2            = 245, /* silver coronet [magic, 3 charges: use -> Armatrutz +5, i.e. RS + 5; found 2x in a chest in DUNGEON_ID_SPINNENHOEHLE ] */
	ITEM_ID_BUCH__HEPTAGON                    = 246, /* book [read -> 'Heptagon' spell +3; found in Spinnenhoehle] */
	ITEM_ID_EMPFEHLUNGSSCHREIBEN__SIEBENSTEIN = 247, /* commendation letter [Umbrik Siebenstein] */
	ITEM_ID_SCHWARZE_STATUETTE                = 248, /* black figurine, found in Tempel des Namenlosen and used as evidence to Eliane Windenbeck */
	ITEM_ID_LATERNE__LIT                      = 249, /* lantern [lit] */
	ITEM_ID_DUKATEN                           = 250, /* ducats */
	ITEM_ID_200_PFEILE                        = 251, /* 200 arrows */
	ITEM_ID_50_BOLZEN                         = 252, /* 50 bolts */
	ITEM_ID_20_KLETTERHAKEN                   = 253, /* 20 climbing hooks */
	ITEM_ID__END                              = 254
};

enum {
	ITEM_SPRITE_ID_SCHWERT                  =   0,
	ITEM_SPRITE_ID_KNUEPPEL                 =   1,
	ITEM_SPRITE_ID_SAEBEL                   =   2,
	ITEM_SPRITE_ID_MESSER                   =   3,
	ITEM_SPRITE_ID_SPEER                    =   4,
	ITEM_SPRITE_ID_KURZSCHWERT              =   5,
	ITEM_SPRITE_ID_SCHILD                   =   6,
	ITEM_SPRITE_ID_KRIEGSBEIL               =   7,
	ITEM_SPRITE_ID_KURZBOGEN                =   8,
	ITEM_SPRITE_ID_PFEIL                    =   9,
	ITEM_SPRITE_ID_STREITAXT                =  10,
	ITEM_SPRITE_ID_ARMBRUST                 =  11,
	ITEM_SPRITE_ID_BOLZEN                   =  12,
	ITEM_SPRITE_ID_DOLCH                    =  13,
	ITEM_SPRITE_ID_EISENSCHILD              =  14,
	ITEM_SPRITE_ID_AXE_UNUSED               =  15, /* not used */
	ITEM_SPRITE_ID_WURFSTERN                =  16,
	ITEM_SPRITE_ID_WURFSTERN_GREEN          =  17, /* not used */
	ITEM_SPRITE_ID_ZWEIHAENDER              =  18,
	ITEM_SPRITE_ID_LANGBOGEN                =  19,
	ITEM_SPRITE_ID_MORGENSTERN              =  20,
	ITEM_SPRITE_ID_VULKANGLASDOLCH          =  21,
	ITEM_SPRITE_ID_FACKEL__LIT              =  22,
	ITEM_SPRITE_ID_BIER                     =  23,
	ITEM_SPRITE_ID_WURFHAKEN                =  24,
	ITEM_SPRITE_ID_LATERNE__UNLIT           =  25,
	ITEM_SPRITE_ID_BRECHEISEN               =  26,
	ITEM_SPRITE_ID_HAMMER                   =  27,
	ITEM_SPRITE_ID_ANGELHAKEN               =  28,
	ITEM_SPRITE_ID_SCHRIFTROLLE             =  29,
	ITEM_SPRITE_ID_WASSERSCHLAUCH           =  30,
	ITEM_SPRITE_ID_GLASFLASCHE              =  31,
	ITEM_SPRITE_ID_STRICKLEITER             =  32,
	ITEM_SPRITE_ID_WURFBEIL                 =  33,
	ITEM_SPRITE_ID_MESSINGSPIEGEL           =  34,
	ITEM_SPRITE_ID_DIETRICHE                =  35,
	ITEM_SPRITE_ID_SCHREIBZEUG              =  36,
	ITEM_SPRITE_ID_HARFE                    =  37,
	ITEM_SPRITE_ID_TRINKHORN                =  38,
	ITEM_SPRITE_ID_SILBERSCHMUCK            =  39,
	ITEM_SPRITE_ID_KLETTERHAKEN             =  40,
	ITEM_SPRITE_ID_OEL                      =  41,
	ITEM_SPRITE_ID_BRONZEFLASCHE            =  42,
	ITEM_SPRITE_ID_EISENHELM                =  43,
	ITEM_SPRITE_ID_PIKE                     =  44,
	ITEM_SPRITE_ID_PROVIANTPAKET            =  45,
	ITEM_SPRITE_ID_FLOETE                   =  46,
	ITEM_SPRITE_ID_ALCHIMIESET              =  47,
	ITEM_SPRITE_ID_HEMD                     =  48,
	ITEM_SPRITE_ID_HOSE                     =  49,
	ITEM_SPRITE_ID_SCHUHE                   =  50,
	ITEM_SPRITE_ID_STIEFEL                  =  51,
	ITEM_SPRITE_ID_SCHNEESCHUHE             =  52,
	ITEM_SPRITE_ID_LEDERHARNISCH            =  53,
	ITEM_SPRITE_ID_SCHUPPENRUESTUNG         =  54,
	ITEM_SPRITE_ID_POTION_GREEN             =  55,
	ITEM_SPRITE_ID_POTION_BLUE              =  56,
	ITEM_SPRITE_ID_POTION_BROWN             =  57,
	ITEM_SPRITE_ID_POTION_YELLOW            =  58,
	ITEM_SPRITE_ID_POTION_GREY              =  59, /* not used */
	ITEM_SPRITE_ID_EINBEERE                 =  60,
	ITEM_SPRITE_ID_WIRSELKRAUT              =  61,
	ITEM_SPRITE_ID_EITRIGER_KROETENSCHEMEL  =  62,
	ITEM_SPRITE_ID_GULMOND_BLATT            =  63,
	ITEM_SPRITE_ID_TARNELE                  =  64,
	ITEM_SPRITE_ID_FACKEL__UNLIT            =  65,
	ITEM_SPRITE_ID_STREITKOLBEN             =  66,
	ITEM_SPRITE_ID_DEGEN                    =  67,
	ITEM_SPRITE_ID_FLORETT                  =  68,
	ITEM_SPRITE_ID_KAMPFSTAB                =  69,
	ITEM_SPRITE_ID_KRISTALLKUGEL            =  70,
	ITEM_SPRITE_ID_PEITSCHE                 =  71,
	ITEM_SPRITE_ID_DECKE                    =  72,
	ITEM_SPRITE_ID_SCHAUFEL                 =  73,
	ITEM_SPRITE_ID_GOLDSCHMUCK              =  74,
	ITEM_SPRITE_ID_ROBE__GREEN              =  75,
	ITEM_SPRITE_ID_ROBE__RED                =  76,
	ITEM_SPRITE_ID_TOPFHELM                 =  77,
	ITEM_SPRITE_ID_LEDERHELM                =  78,
	ITEM_SPRITE_ID_WAFFENROCK               =  79,
	ITEM_SPRITE_ID_KETTENHEMD               =  80,
	ITEM_SPRITE_ID_KROETENHAUT              =  81,
	ITEM_SPRITE_ID_PLATTENZEUG              =  82,
	ITEM_SPRITE_ID_KETTENZEUG               =  83,
	ITEM_SPRITE_ID_LEDERZEUG                =  84,
	ITEM_SPRITE_ID_ZUNDERKAESTCHEN          =  85,
	ITEM_SPRITE_ID_SCHLEIFSTEIN             =  86,
	ITEM_SPRITE_ID_ESSBESTECK               =  87,
	ITEM_SPRITE_ID_ESSGESCHIRR              =  88,
	ITEM_SPRITE_ID_LAKRITZE                 =  89,
	ITEM_SPRITE_ID_BONBONS                  =  90,
	ITEM_SPRITE_ID_WEINFLASCHE              =  91,
	ITEM_SPRITE_ID_SCHNAPSFLASCHE           =  92,
	ITEM_SPRITE_ID_HACKE                    =  93,
	ITEM_SPRITE_ID_AMULETT_CROSS_GREEN_GEM  =  94,
	ITEM_SPRITE_ID_LAUTE                    =  95,
	ITEM_SPRITE_ID_WINTERMANTEL             =  96,
	ITEM_SPRITE_ID_NETZ                     =  97,
	ITEM_SPRITE_ID_WURFMESSER               =  98,
	ITEM_SPRITE_ID_SICHEL                   =  99,
	ITEM_SPRITE_ID_SENSE                    = 100,
	ITEM_SPRITE_ID_KRIEGSHAMMER             = 101,
	ITEM_SPRITE_ID_DREIZACK                 = 102,
	ITEM_SPRITE_ID_HELLEBARDE               = 103,
	ITEM_SPRITE_ID_DRESCHFLEGEL             = 104,
	ITEM_SPRITE_ID_ZWEILILIEN               = 105,
	ITEM_SPRITE_ID_OCHSENHERDE              = 106,
	ITEM_SPRITE_ID_BASILISKENZUNGE          = 107,
	ITEM_SPRITE_ID_OGERFAENGER              = 108,
	ITEM_SPRITE_ID_MENGBILAR                = 109,
	ITEM_SPRITE_ID_SCHWERER_DOLCH           = 110,
	ITEM_SPRITE_ID_RONDRAKAMM               = 111,
	ITEM_SPRITE_ID_ENTERMESSER              = 112,
	ITEM_SPRITE_ID_BASTARDSCHWERT           = 113,
	ITEM_SPRITE_ID_TUZAKMESSER              = 114,
	ITEM_SPRITE_ID_RABENSCHNABEL            = 115,
	ITEM_SPRITE_ID_BRABAKBENGEL             = 116,
	ITEM_SPRITE_ID_RAPIER                   = 117,
	ITEM_SPRITE_ID_KHUNCHOMER               = 118,
	ITEM_SPRITE_ID_DOPPELKHUNCHOMER         = 119,
	ITEM_SPRITE_ID_SCHLEUDER                = 120,
	ITEM_SPRITE_ID_SEIL                     = 121,
	ITEM_SPRITE_ID_SHURINKNOLLE             = 122,
	ITEM_SPRITE_ID_BELMART_BLATT            = 123,
	ITEM_SPRITE_ID_DONFSTENGEL              = 124,
	ITEM_SPRITE_ID_MENCHALKAKTUS            = 125,
	ITEM_SPRITE_ID_ALRAUNE                  = 126,
	ITEM_SPRITE_ID_ATMONBLUETE              = 127,
	ITEM_SPRITE_ID_ILMENBLATT               = 128,
	ITEM_SPRITE_ID_FINAGEBAEMCHEN           = 129,
	ITEM_SPRITE_ID_JORUGAWURZEL             = 130,
	ITEM_SPRITE_ID_THONNYSBLUETE            = 131,
	ITEM_SPRITE_ID_LOTUSBLUETE              = 132,
	ITEM_SPRITE_ID_HEXENBESEN               = 133,
	ITEM_SPRITE_ID_SKRAJA                   = 134,
	ITEM_SPRITE_ID_SCHNEIDZAHN              = 135,
	ITEM_SPRITE_ID_ORKNASE                  = 136,
	ITEM_SPRITE_ID_ROBBENTOETER             = 137,
	ITEM_SPRITE_ID_WOLFSMESSER              = 138,
	ITEM_SPRITE_ID_KAIRANHALM               = 139,
	ITEM_SPRITE_ID_OLGINWURZEL              = 140,
	ITEM_SPRITE_ID_DAS_SCHWERT_GRIMRING     = 141,
	ITEM_SPRITE_ID_KRAFTGUERTEL             = 142,
	ITEM_SPRITE_ID_TOTENKOPFGUERTEL         = 143,
	ITEM_SPRITE_ID_RING__RED                = 144,
	ITEM_SPRITE_ID_RING__BLUE               = 145,
	ITEM_SPRITE_ID_RING__GREEN              = 146,
	ITEM_SPRITE_ID_KRISTALL                 = 147,
	ITEM_SPRITE_ID_AMULETT_CIRCULAR_YELLOW  = 148,
	ITEM_SPRITE_ID_AMULETT_CROSS_YELLOW     = 149,
	ITEM_SPRITE_ID_AMULETT_STAR_BLUE        = 150,
	ITEM_SPRITE_ID_CORONET_SILVER           = 151,
	ITEM_SPRITE_ID_CORONET_GOLD             = 152, /* not used */
	ITEM_SPRITE_ID_MAGISCHER_BROTBEUTEL     = 153,
	ITEM_SPRITE_ID_MAGISCHER_WASSERSCHLAUCH = 154,
	ITEM_SPRITE_ID_KEY_YELLOW               = 155,
	ITEM_SPRITE_ID_KEY_SILVER               = 156,
	ITEM_SPRITE_ID_KEY_RED                  = 157,
	ITEM_SPRITE_ID_SYRINGE                  = 158, /* not used */
	ITEM_SPRITE_ID_KNOCHEN_MIT_RUNE         = 159,
	ITEM_SPRITE_ID_MAGIC_LAMP               = 160, /* not used */
	ITEM_SPRITE_ID_DOCUMENT                 = 161,
	ITEM_SPRITE_ID_ROTE_MONDSCHEIBE         = 162,
	ITEM_SPRITE_ID_SCHWARZE_STATUETTE       = 163,
	ITEM_SPRITE_ID_LATERNE__LIT             = 164
};

enum {
	IGNITE_MODE_SPELL_OR_USE_TINDER = 0,
	IGNITE_MODE_USE_TORCH           = 1,
	IGNITE_MODE_USE_LANTERN         = 2
};

enum {
	GRAMMAR_GENDER_MASCULINE = 0,
	GRAMMAR_GENDER_FEMININE  = 1,
	GRAMMAR_GENDER_NEUTER    = 2
};

enum {
	GRAMMAR_ARTICLE_INDEFINITE = 0,
	GRAMMAR_ARTICLE_NONE       = 1,
	GRAMMAR_ARTICLE_DEFINITE   = 2
};

enum {
	GRAMMAR_NUMBER_SINGULAR = 0,
	GRAMMAR_NUMBER_PLURAL   = 1
};

enum {
	GRAMMAR_CASE_1ST = 0,
	GRAMMAR_CASE_2ND = 1,
	GRAMMAR_CASE_4TH = 2,
	GRAMMAR_CASE_3RD = 3
};

enum {
	INFLECT_NAME_TYPE_ITEM    = 0,
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
	SHIP_TYPE_LANGSCHIFF_HIGH_SEAS = 0,
	SHIP_TYPE_KARRACKE             = 1,
	SHIP_TYPE_SCHNELLSEGLER        = 2,
	SHIP_TYPE_SCHNELLSEGLER_LUXURY = 3,
	/* ships on costal routes */
	SHIP_TYPE_LANGSCHIFF_COSTAL    = 4,
	SHIP_TYPE_KUESTENSEGLER        = 5,
	SHIP_TYPE_KUTTER               = 6,
	SHIP_TYPE_FISCHERBOOT          = 7
};

enum {
	PASSAGE_TYPE_HEUER               = 0, // regen. -2
	PASSAGE_TYPE_BEGLEITSCHUTZFAHRT  = 1, // regen.  0
	PASSAGE_TYPE_LUXUSPASSAGE        = 2, // regen. +5
	PASSAGE_TYPE_KOMFORTABLE_PASSAGE = 3, // regen  +4
	PASSAGE_TYPE_KABINENPASSAGE      = 4, // regen. +3
	PASSAGE_TYPE_DECKPASSAGE         = 5, // regen. +1
	PASSAGE_TYPE_MITFAHRGELEGENHEIT  = 6  // regen.  0
};

enum {
	GAME_MODE_UNSPECIFIED	= -1,
	GAME_MODE_BEGINNER	=  1,
	GAME_MODE_ADVANCED	=  2
};

enum { /* note that the order differs from the one in HERO_TYPE... :( */
	SPELL_ORIGIN_DRUIDE  = 0,
	SPELL_ORIGIN_MAGIER  = 1,
	SPELL_ORIGIN_HEXE    = 2,
	SPELL_ORIGIN_AUELF   = 3,
	SPELL_ORIGIN_FIRNELF = 4,
	SPELL_ORIGIN_WALDELF = 5
};

enum {
	MENU_ICON_NONE			= -1,
	MENU_ICON_HIRE_HERO		=  0,
	MENU_ICON_DISMISS_HERO		=  1,
	MENU_ICON_DELETE_HERO		=  2,
	MENU_ICON_LOAD_GAME		=  3,
	MENU_ICON_SAVE_GAME		=  4,
	MENU_ICON_QUIT_GAME		=  5,
	MENU_ICON_PRAY			=  6,
	MENU_ICON_SACRIFICE		=  7,
	MENU_ICON_LEAVE			=  8,
	MENU_ICON_GUARDS		=  9,
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
