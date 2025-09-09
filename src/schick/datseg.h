#if !defined V302DE_DATSEG_H
#define V302DE_DATSEG_H

#include <time.h>

#if defined(__BORLANDC__)
extern char ds[1];
#endif

struct sample_idx {
	Bit8s patch;
	Bit8s bank;
	Bit32u offset;
};

struct struct_modification_timer {
	Bit32s time_left;
	Bit16u offset;
	Bit8s target;
	Bit8s modifier;
};

struct mouse_action {
	int x1;
	int y1;
	int x2;
	int y2;
	int action;
};

struct struct_pic_copy {
	unsigned char *dst;
	int x1;
	int y1;
	int x2;
	int y2;
	unsigned char *src;
	int v1;
	int v2;
	int v3;
	int v4;
};

struct struct_char_width {
	char c;
	char font_id;
	char width;
};

struct struct_char_height {
	char c;
	char font_id;
	char height;
};

struct struct_memslot_ani {
	signed int figure;
	signed int ems_handle;
	Bit32u length;
};

struct struct_memslot_fig {
	signed int figure;
	Bit8u *ptr;
	signed int ems_handle;
	Bit32s length;
};

struct point8s {
	Bit8s x;
	Bit8s y;
};

struct struct_point {
	signed short x;
	signed short y;
};

struct struct_rect {
	int y1;
	int x1;
	int y2;
	int x2;
};

struct struct_diary_entry {
	Bit16s day;
	Bit16s month;
	Bit16s year;
	Bit16s town;
};

/* seg041 */
struct ranged_weapon_descr {
	signed char damage_modifier[7];
	signed char base_handicap;
};

struct weapon_descr {
	Bit8s damage_d6;
	Bit8s damage_const;
	Bit8s damage_kk_bonus;		/* Koerperkraft-Zuschlag */
	Bit8s bf;		/* Bruchfaktor */
	Bit8s ranged_index;	/* related to ranged weapons */
	Bit8s at_mod;
	Bit8s pa_mod;
};

struct armors_descr {
	Bit8s rs;
	Bit8s be;
};

struct specialitem_descr {
	Bit8s unkn1;
	Bit8s quantity;
	Bit8s handler_id;
};

struct staffspell_descr {
	Bit8s attrib1;
	Bit8s attrib2;
	Bit8s attrib3;
	Bit8s handicap;
	Bit8s ae_cost;
	Bit8s ae_mod;
};

struct spell_descr {
	Bit8s herotype;	/* 0: druid spell; 1: mage spell; 2: witch spell; 3: green elf spell; 4: ice elf spell; 5: sylvan elf spell */
	Bit8s attrib1;
	Bit8s attrib2;
	Bit8s attrib3;
	Bit8s ae_cost;
	Bit8s where_to_use;	/* -1: not in fight; 0: normal and fight; 1: only in fight */
	Bit8s unkn6;
	Bit8s target_type;
	Bit8s range;
	Bit8s fight;
};

struct skill_descr {
	Bit8s attrib1;
	Bit8s attrib2;
	Bit8s attrib3;
	Bit8s max_incs;
};

struct trv_start_point {
	Bit8s town;
	Bit8s typeindex;
	Bit8s *end_points;
};

// A location is a point of interest in a town or a travel event. //
struct location {
	Bit16s pos;		// coordinates of the location within its town
	Bit8s loctype;		// the type of the location
	Bit8u typeindex;	// Index among the locations of the same type.
	Bit16s locdata;		// Additional data, depending on the LOCTYPE.
};

struct struct_msg {
	signed int id;
	signed int type;
};

struct market {
	Bit16s min_size;	/* value in {1, ..., 19} or -1 as terminator */
	Bit16s price_mod;
	Bit16s market_day;
	Bit16s size;		/* unused */
};

/* REMARK1: DOS-Version: struct is inside the game state! */
/* REMARK2: DOS-Version: states_offset is added with a 4-Byte pointer. Not portable! */
struct struct_dialog_partner {
	Bit32u states_offset; /* TODO: Mixed with pointer: not portable! */
	Bit16s txt_offset;
	char title[30];
	Bit16s head_id;
};

/* REMARK: Can stay in the game state, since it's protable, but should be mouved out of it */
struct struct_dialog_state {
	Bit16s txt_id;	/* main txt_id >= 0 */
	Bit8u txt_id1;
	Bit8u txt_id2;
	Bit8u txt_id3;
	Bit8u state1;
	Bit8u state2;
	Bit8u state3;
};

struct struct_fighter {
	signed short figure;
	signed char nvf_no;
	signed char cbx;
	signed char cby;
	signed char offsetx;
	signed char offsety;
	signed char height;
	signed char width;
	signed char x1;
	signed char y1;
	signed char x2;
	signed char y2;
	signed char reload; 	/* {0, -1	= update gfx data} */
	signed char sheet;	/* 0xe274, 0xe2a8, 0xd8ce */
	signed char wsheet;	/* 0xe274 */
	signed char id;		/* position in g_fig_list_array */
	signed char z;
	signed char visible;	/* {0, 1, 2, 3} */
	signed char twofielded;	/* -1 if fighter is not twofielded. for twofielded fighter:
				 * head part: FIGHTER_TWOFIELDED can be used as index for FIG_TWOFIELDED_TABLE which contains the FIGHTER_ID);
				 * tail part: entry is FIGHTER_TWOFIELDED+20 of the head part. */
	signed char obj_id;	/* stores the id of the cb_entry of the square before the fighter entered it */
	signed char is_enemy;	/* {0 = hero, 1	= enemy, 2 = hero} */ /* strangly, at one position in seg039.cpp the value 2 is written */
	signed char sprite_no;	/* 0x12c0, 0x1531, 0x1210 */
	Bit8u* gfxbuf;
	struct struct_fighter* next;	/* TODO: These should located at the start of the structure */
	struct struct_fighter* prev;
};

struct fight_monster {
	Bit8s id;
	Bit8s x;
	Bit8s y;
	Bit8s viewdir;
	Bit8s round_appear;
};

struct fight_hero {
	Bit8s x;
	Bit8s y;
	Bit8s viewdir;
	Bit8s round_appear;
};

struct fight {
	char name[19];
	Bit8s intro_seen;
	Bit16s scenario_id;
	struct fight_monster monsters[20];
	struct fight_hero heroes[7];
	Bit16s loot[30];
	Bit16s ducats;
	Bit16s silver;
	Bit16s heller;
};

struct mon_spell_description {
	Bit8s ae_cost;
	Bit8s mode;
	Bit8s unkn1;
	Bit8s attrib1;
	Bit8s attrib2;
	Bit8s attrib3;
	Bit8s vs_mr;
	Bit8s ani_id;
};

struct struct_smith_repairitems {
	Bit16s item_id;
	Bit32s pickup_time;
};

struct struct_informer_tab {
	Bit16s name_id;		/* in TEXT.LTX */
	Bit8s town;		/* -1 = no city, else city id */
	Bit8s unkn;		/* {0, 1} */
};

struct healer_descr {
	Bit8s price_mod;	/* range: [-50,..,50], absolute difference as percentage value */
	Bit8s quality;		/* range: [1,..,17]. the lower, the better healing quality */
};

struct smith_descr {
	Bit8s price_mod;	/* price modification, range [-50..50], absolute difference as percentage value */
	Bit8s quality;		/* range: [1..17], is unused. */
};

struct inn_descr {		/* INNS and TAVERNS */
	Bit16s quality;		/* range: [1..20]. the lower, the better food and sleep quality */
	Bit16s price_mod;	/* price modification, range [-40..70], absolute difference as percentage value */
};

struct shop_descr {
	Bit8s price_mod;
	Bit8s type;
	Bit8s sortiment;
	Bit16s extra_items[3];
};

struct gather_herbs {
	Bit8u item_id;
	Bit8s chance_max;
	Bit8s max_count;
	Bit8s handicap;
};

struct struct_shopping_cart {
	int item_id;
	int quantity;
};

struct struct_chest {
	signed short pos;
	union {
		signed char mod;
		unsigned char key;
	};
	void (*open)(struct struct_chest*);
	void (*trap)(void);
	union {
		void (*loot)(Bit8u*);
		Bit8u* content;
	};
	signed short ap;
	unsigned int money; //TODO: should become Int32 instead
	signed short food;
};

/* TODO: SHOULD BE IN GAME STATE */
/* rationale: write SEA_ROUTE_PASSAGE... if the value depends on the current passage on the route (and will change with time).
 * the other values are constants. */
struct sea_route {

	signed char town1;	/* one byte readonly */
				/* ID of the first town of the connection. Note that the routes are undirected; i.e. both endpoint towns are treated equal. */
	signed char town2;	/* one byte readonly */ /* ID of the second town of the connection */
	unsigned char distance; /* one byte readonly */ /* TODO: But should be 2 Byte for Prem <-> Manrin to avoid an integer overflow */
	signed char frequency;/* one byte readonly */ /* the higher this value, the rarer a passage on the route is offered */
	signed char passage_timer;	/* one byte rw */ /* after how many days will a ship do a passage on this route? */
	signed char coastal_route;	/* one byte readonly */ /* 0 = high seas route (= Hochseeroute), 1 = costal route */
	signed char ship_type;	/* one byte rw */ /* passage type of the next passage on this route */
	unsigned char price_mod;	/* one byte rw */ /* a number between 70 and 130 which serves as a percentage modifier to the price of the next ship on this route */
};

/* TODO: This structure makes an inconsistent game state */
struct harbor_option_obsolete {
	char *ship_name_ptr;
	struct sea_route *sea_route_ptr;
	Bit8s ship_timer;	/* 0 or 1, write only */
	Bit8s ship_type;	/* 0, read only */
	Bit8u destination;
	Bit8s route_id;		/* write only */
};

struct struct_route_tevent {
	Bit16s place;
	Bit16s tevent_id;
};

struct struct_land_route {
	/* Note: routes are undirected */
	Bit8u town1_id;
	Bit8u town2_id;
	Bit8u distance;
	Bit8s speed_mod;		/* {-4, ..., 7} */
	Bit8u encounters;
	unsigned char unkn1;		/* unused */
	unsigned char unkn2;		/* unused */
	Bit8u fights;
	unsigned char unkn3;		/* unused */
};

struct struct_tevent {
	Bit8s route_id;
	Bit8u place;
	Bit8u tevent_id;
};

struct struct_recipe {
	signed short item_id;		/* item id of the recipe */
	signed short ingredients[10];	/* list of ingredients (item_ids), terminated by -1 */
	signed short outcome;		/* item id of the outcome of the recipe */
	signed short ae_cost;		/* AE needed */
	signed char  handicap;		/* handicap for the alchemy skill test */
	signed char  duration;		/* time needed to brew the recipe in hours */
};

struct spells_index {
	Bit8s first;
	Bit8s length;
};

struct skills_index {
	Bit8s first;
	Bit8s length;
};

/* seg001 */
extern unsigned short g_cd_init_successful;
extern Bit32u g_cd_skipmagic;

extern signed short g_items_noplural[23];				// ds:0x0270; seg096
extern signed short g_items_pluralwords[7];				// ds:0x029e; seg106, seg107
extern signed char g_items_genders[254];				// ds:0x02ac; seg096
extern signed short* g_wearable_items_index[12];			// ds:0x0638; seg048, seg056, seg105
extern const struct ranged_weapon_descr g_ranged_weapons_table[9];	// ds:0x0668; seg041
extern struct weapon_descr g_weapons_table[65];				// ds:0x06b0; seg033, seg041, seg105
extern struct armors_descr g_armors_table[25];				// ds:0x0877; seg079, seg100, seg102, seg105
extern const struct specialitem_descr g_specialitems_table[14];		// ds:0x08a9; seg105, seg107
extern signed short g_poison_potions[10];				// ds:0x08d3; seg107, seg108
extern signed short g_herbs_toxic[5];					// ds:0x08e7; seg108
extern signed short g_herbs_uneatable[7];				// ds:0x08f1; seg108
extern signed short g_elixir_potions[8];				// ds:0x08ff; seg108
extern signed short g_bad_elixirs[8];					// ds:0x090f; seg108
extern signed short g_attack_items[3];					// ds:0x091f; seg033
extern signed char g_monname_genders[78];				// ds:0x0925; seg096
extern struct staffspell_descr g_staffspell_descriptions[7];		// ds:0x0973; seg098
extern struct spell_descr g_spell_descriptions[87];			// ds:0x099d; seg033, seg036, seg042, seg050, seg098
extern const struct spells_index g_spells_index[8];			// ds:0x0d03; seg046, seg098
extern const struct spells_index g_spells_index2[4];			// ds:0x0d13; seg046
extern const signed short* g_magic_schools_index[9];
extern void (*g_spell_handlers[86])(void);				// ds:0x0dbb; seg098
extern struct mon_spell_description g_mon_spell_descriptions[15];	// ds:0x0f13; seg037, seg043, seg102
extern Bit8s g_mon_spell_repertoire[11][5];				// ds:0x0f8b; seg037
extern void (*g_mon_spellhandlers[15])(void);				// ds:0x0fc2; seg102
extern const struct skill_descr g_skill_descriptions[52];		// ds:0x0ffe; seg050, seg103, seg104
extern const struct skills_index g_skills_index[7];			//ds:0x10ce; seg046, seg103
extern signed char g_nvftab_figures_rangeweapon[22][3][4];		//ds:0x10dc; seg002, seg033, seg039

extern signed short g_nvftab_figures_unconscious[22];	//ds:0x11e4; seg002, seg005, seg039

extern signed short g_nvftab_figures_dead[22];		//ds:0x1a13; seg005, seg039
extern Bit16s *g_gfx_ani_index[41];			//ds:0x2555; seg036, seg037, seg044
extern signed char g_two_fielded_sprite_id[5];		//ds:0x25f9; seg032, seg034, seg037, seg038, seg039, seg042, seg043, seg044
extern signed short g_weaponani_table[72];		//ds:0x25fe; seg044
extern signed char g_weaponani_types[22];		//ds:0x268e; seg044
extern signed char g_food_message_shown[7];		//ds:0x26a4; seg002-seg093
extern unsigned char g_ems_enabled;			//ds:0x26ab; seg027-seg028, seg120
extern signed char g_fig_initiative;			//ds:0x26ac; seg032,seg051,seg052,seg110-seg113
extern signed short g_fig_msg_counter;			//ds:0x26ad; seg041
extern unsigned short g_always_one;			//ds:0x26af; seg006
extern signed short g_fig_continue_print;		//ds:0x26b1; seg004,seg005,seg042,seg043
extern signed short g_fig_char_pic;			//ds:0x26b3; seg006,seg032
extern signed short g_fig_enemy_pic;			//ds:0x26b5; seg006,seg032
extern signed short g_gui_entering_savegame;		//ds:0x26b7; seg026,seg097
extern signed short g_check_disease;			//ds:0x26b9; seg002,seg026,seg063,seg094,seg119
extern signed short g_check_poison;			//ds:0x26bb; seg002,seg026,seg121
extern signed short g_text_file_index;			//ds:0x26bd; seg026-seg065
extern signed short g_tx_file_index;			//ds:0x26bf; seg026-seg107
extern signed short g_fig_discard;			//ds:0x26c1; seg030-seg117
extern Bit8u g_palette_allblack2[3 * 32];		//ds:0x26c3; seg025,seg029,seg049,seg066,seg120
extern Bit8u g_palette_unknown1[32][3];			//ds:0x2723; seg120
extern Bit8u g_palette_fight1[32][3];			//ds:0x2783; seg032
extern Bit8u g_palette_special[32][3];			//ds:0x27e3; seg029, seg093
extern signed char g_pp20_index;			//ds:0x2845; seg004-seg120
extern signed short g_request_refresh;			//ds:0x2846; seg002-seg118

extern signed short g_mouse_posy_min;			//ds:0x2988; seg002, seg049, seg097
extern signed short g_mouse_posx_min;			//ds:0x298a; seg002, seg049, seg097
extern signed short g_mouse_posy_max;			//ds:0x298c; seg002, seg049, seg097
extern signed short g_mouse_posx_max;			//ds:0x298e; seg002, seg049, seg097
extern struct struct_rect g_pic_copy_rect;		//ds:0x2990; seg004-seg008, seg074
extern signed short g_mouse_locked;			//ds:0x2998; seg002, seg049
extern signed short g_mouse_refresh_flag;		//ds:0x299a; seg002, seg032
extern signed short g_mouse_posx;			//ds:0x299c; seg002-seg097
extern signed short g_mouse_posy;			//ds:0x299e; seg002-seg097
extern signed short g_mouse_posx_bak;			//ds:0x29a0; seg002, seg004, seg049
extern signed short g_mouse_posy_bak;			//ds:0x29a2; seg002, seg004, seg049
extern signed short g_mouse_moved;			//ds:0x29a4; seg002, seg049
extern signed short g_mouse_pointer_offsetx;		//ds:0x29a6; seg002, seg004
extern signed short g_mouse_pointer_offsety;		//ds:0x29a8; seg002, seg004
extern signed short g_mouse_pointer_offsetx_bak;	//ds:0x29aa; seg002, seg004
extern signed short g_mouse_pointer_offsety_bak;	//ds:0x29ac; seg002, seg004
extern signed short g_ani_enabled;			//ds:0x29ae; seg004, seg026, seg027, seg097
extern signed short g_statusmenu_allowed;		//ds:0x29b4; seg048
extern signed short g_heroswap_allowed;			//ds:0x29b4; seg002, seg049, seg057
extern signed short g_intemple;				//ds:0x29b6; seg061
extern signed short g_intemple2;			//ds:0x29b8; seg061
extern signed short g_always_zero3;			//ds:0x29ba; seg002
extern struct mouse_action g_action_table_menu[2];	//ds:0x29cc; seg032, seg097
extern struct mouse_action *g_action_table_primary;	//ds:0x29e0; seg002, seg029, seg032, seg046
extern struct mouse_action *g_action_table_secondary;	//ds:0x29e4; seg002, seg025, seg029-seg097
extern struct mouse_action g_action_table_playmask[24];	//ds:0x29e8; seg029
extern struct mouse_action g_action_table_status[30];	//ds:0x2ad8; seg046
extern struct mouse_action g_action_table_merchant[2];	//ds:0x2c04; seg056, seg057, seg058


extern signed short g_disease_prices[8];		//ds:0x2c50; seg053, seg104
extern signed short g_disease_delays[8];		//ds:0x2c60; seg053
extern signed short g_poison_prices[10];		//ds:0x2c70; seg041, seg053, seg099, seg103
extern signed short g_poison_delays[10];		//ds:0x2c84; seg053;
extern signed char  g_dialogbox_lock;			//ds:0x2c98; seg002, seg004, seg097, seg109
extern signed short g_timers_disabled;			//ds:0x2c99; seg002-seg050
extern signed short g_status_page_mode;			//ds:0x2c9b; seg046, seg048, seg050
extern signed short g_status_page_hero;			//ds:0x2c9d; seg004, seg048, seg050
extern signed char  g_status_page_hunger;		//ds:0x2c9f; seg004, seg048, seg050
extern signed char  g_status_page_thirst;		//ds:0x2ca0; seg004, seg048, seg050
extern signed char  g_missing_file_guilock;		//ds:0x2ca1; seg002
extern signed short g_basepos_x;			//ds:0x2ca2; seg025-seg117
extern signed short g_basepos_y;			//ds:0x2ca4; seg025-seg117
extern signed char g_dng_area_loaded;			//ds:0x2ca6; seg028-seg106
extern signed char g_city_area_loaded;			//ds:0x2ca7; seg025-seg106
extern signed char g_travel_map_loaded;			//ds:0x2ca8; seg025, seg093
extern signed char g_ani_unknown_flag;			//ds:0x2cca; seg004 WRITEONLY
extern signed short g_area_prepared;			//ds:0x2ccb; seg024-seg089
extern unsigned char g_always_zero4;			//ds:0x2cce; seg032
extern signed short g_current_ani;			//ds:0x2ccf; seg004-seg120
extern signed short g_fig_figure1;			//ds:0x2cd1; seg025, seg027, seg032
extern signed short g_fig_figure2;			//ds:0x2cd3; seg025, seg027, seg032
extern signed short g_in_fight;				//ds:0x2cd5; seg002-seg107
extern signed short g_fight_round;			//ds:0x2cd7; seg032, seg036, seg037
extern signed short g_skilled_hero_pos;			//ds:0x2cdb; seg047, seg051, seg056, seg057, seg097, seg103
extern const struct struct_point g_gui_buttons_pos[9];	//ds:0x2cdd; seg029
extern const signed int g_hero_pic_posx[7];		//ds:0x2d01; seg002-seg098
extern signed char g_levelup_ta_rise[12];		//ds:0x2d0f; seg050
extern signed char g_levelup_sp_rise[6];		//ds:0x2d1b; seg050
extern signed char g_levelup_spta_conv[6];		//ds:0x2d21; seg050
extern signed char g_mr_modificators[13];		//ds:0x2d27; seg050

/* GameState written directly into SaveGame file starts at 0x2d34 */
extern Bit8s  gs_datseg_status_start;		//ds:0x2d34; seg002, seg026, seg065
extern Bit8s  gs_current_group;			//ds:0x2d35; seg002-seg120
extern Bit8s  gs_group_member_counts[6];	//ds:0x2d36; seg002-seg118
extern Bit8s  gs_total_hero_counter;		//ds:0x2d3c; seg002-seg113
extern Bit8s  gs_direction;			//ds:0x2d3d; seg002-seg120
extern Bit8s  gs_groups_direction[6];		//ds:0x2d3e; seg049, seg054, seg074
extern Bit16s gs_x_target;			//ds:0x2d44; seg002-seg120
extern Bit16s gs_y_target;			//ds:0x2d46; seg002-seg120
extern Bit16s gs_groups_x_target[6];		//ds:0x2d48; seg002-seg089
extern Bit16s gs_groups_y_target[6];		//ds:0x2d54; seg002-seg089
extern Bit8s  gs_current_loctype;		//ds:0x2d60; seg002-seg120
extern Bit8s  gs_groups_current_loctype[6];	//ds:0x2d61; seg002, seg049
extern Bit8s  gs_current_town;			//ds:0x2d67; seg002-seg120
extern Bit8s  gs_groups_town[6];		//ds:0x2d68; seg002-seg074
extern Bit8s  gs_dungeon_index;			//ds:0x2d6e; seg002-seg120
extern Bit8s  gs_groups_dng_index[6];		//ds:0x2d6f; seg002, seg049
extern Bit8s  gs_dungeon_level;			//ds:0x2d75; seg002-seg107
extern Bit8s  gs_groups_dng_level[6];		//ds:0x2d76; seg002-seg084
extern Bit8s  gs_direction_bak;			//ds:0x2d7c; seg03-seg120
extern Bit8s  gs_groups_direction_bak[6];	//ds:0x2d7d; seg049, seg054
extern Bit16s gs_x_target_bak;			//ds:0x2d83; seg025-seg120
extern Bit16s gs_y_target_bak;			//ds:0x2d85; seg025-seg120
extern Bit16s gs_groups_x_target_bak[6];	//ds:0x2d87; seg002, seg049, seg054
extern Bit16s gs_groups_y_target_bak[6];	//ds:0x2d93; seg002, seg049, seg054
extern Bit8s  gs_current_loctype_bak;		//ds:0x2d9f; seg002-seg120
extern Bit8s  gs_groups_current_loctype_bak[6];	//ds:0x2da0; seg002, seg049, seg054
extern Bit8s  gs_current_town_bak;		//ds:0x2da6; seg025, seg049, seg066, seg075, seg120
extern Bit8s  gs_groups_town_bak[6];		//ds:0x2da7; seg049, seg054
extern Bit8s  gs_dungeon_index_bak;		//ds:0x2dad; seg025, seg049, seg076
extern Bit8s  gs_groups_dng_index_bak[6];	//ds:0x2dae; seg049, seg054
extern Bit8s  gs_dungeon_level_bak;		//ds:0x2db4; seg032, seg049
extern Bit8s  gs_groups_dng_level_bak[6];	//ds:0x2db5; seg049, seg054
extern Bit32s gs_day_timer;			//ds:0x2dbb; seg002-seg120
extern Bit8s  gs_day_of_week;			//ds:0x2dbf; seg002, seg030, seg066, seg068, seg120
extern Bit8s  gs_day_of_month;			//ds:0x2dc0; seg002, seg024, seg030, seg073, seg116, seg120
extern Bit8s  gs_month;				//ds:0x2dc1; seg002, seg024, seg025, seg030, seg068, seg116, seg120
extern Bit8s  gs_year;				//ds:0x2dc2; seg002, seg024, seg025, seg030, seg068, seg116, seg120
extern Bit8s  gs_special_day;			//ds:0x2dc3; seg002, seg030
extern Bit32s gs_ingame_timers[26];		//ds:0x2dc4; seg002, seg042-seg103
extern struct struct_modification_timer gs_modification_timers[100];	//ds:0x2e2c; seg002
extern Bit32s gs_gods_estimation[15];		//ds:0x314e; seg003-seg114
extern Bit8s  gs_tav_cheated_flags[88];		//ds:0x318a; seg059, seg103
extern struct struct_smith_repairitems gs_smith_repairitems[50]; //ds:0x31e2; seg002, seg058
extern Bit16s gs_dng_handled_pos;		//ds:0x330e; seg077-seg091
extern Bit16s gs_deathtrap_steps;		//ds:0x3310; seg075, seg077, seg079
extern Bit16s gs_deathtrap;			//ds:0x3312; seg025, seg075, seg077, seg079
extern Bit8s  gs_dng01_amulet_taken;		//ds:0x3314; seg077
extern Bit8s  gs_dng01_sabre_taken;		//ds:0x3315; seg077
extern Bit8s  gs_dng01_crossbow_taken;		//ds:0x3316; seg077
extern Bit8s  gs_dng01_ardora_freed;		//ds:0x3317; seg077, seg095
extern Bit8s  gs_dng01_key_taken;		//ds:0x3319; seg077
extern Bit8s  gs_dng01_corpse_looted;		//ds:0x331a; seg077
extern Bit16u gs_weather1;			//ds:0x331b; seg002, seg064
extern Bit16u gs_weather2;			//ds:0x331d; seg002, seg064
extern Bit8u  gs_daspota_fightflags[19];	//ds:0x331f; seg030, seg066
extern Bit8s  gs_treasure_maps[10];		//ds:0x3332; seg025-seg116
extern Bit8s  gs_debtbook_read_flag;		//ds:0x333c; seg107
extern Bit8s  gs_orcdocument_read_flag;		//ds:0x333d; seg107, seg116
extern Bit16s gs_gardanium_used_ae;		//ds:0x333e; seg099;
extern Bit16s gs_min_map_parts;			//ds:0x3340; seg069
extern Bit16s gs_subvention;			//ds:0x3342; seg068
extern Bit16s gs_otta_windriders;		//ds:0x3344; seg069
extern Bit16s gs_otta_stormchildren;		//ds:0x3346; seg069
extern Bit16s gs_otta_garaldsson;		//ds:0x3348; seg069
extern Bit16s gs_heard_announce;		//ds:0x334a; seg069, seg073
extern Bit16s gs_got_main_quest;		//ds:0x334c; seg069, seg073
extern Bit16s gs_quested_months;		//ds:0x334e; seg002-seg094
extern Bit16s gs_monthly_credit;		//ds:0x3350; seg002, seg068
extern Bit16s gs_got_letter_jad;		//ds:0x3352; seg068
extern Bit16s gs_visited_miller;		//ds:0x3354; seg068
extern Bit16s gs_quest_ugdalf;			//ds:0x3356; seg068, seg087
extern Bit16s gs_academy_daily_curse;		//ds:0x3358; seg068
extern Bit16s gs_academy_daily_ident;		//ds:0x335a; seg002, seg068
extern Bit16s gs_bank_deposit;			//ds:0x335c; seg002, seg055, seg068
extern Bit16s gs_days_to_cens;			//ds:0x335e; seg002, seg068
extern Bit16s gs_debt_days;			//ds:0x3360; seg002, seg055, seg068
extern Bit16s gs_in_academy;			//ds:0x3362; seg068, seg099
extern Bit8u  gs_informer_flags[16];		//ds:0x3364; seg030-seg116
extern Bit8u  gs_tav_kicked_flags[88];		//ds:0x3374; seg060;
extern Bit8u  gs_town_outlawed_flags[52];	//ds:0x33cc; seg054, seg060;
extern Bit8u  gs_herberg_kicked_flags[74];	//ds:0x3400; seg060
extern Bit8u  gs_got_letter_het;		//ds:0x344a; seg069
extern Bit8u  gs_jurge_awaits_letter;		//ds:0x344c; seg069, seg072, seg073
extern Bit8u  gs_umbrik_quest_gorah;		//ds:0x344d; seg072, seg111
extern Bit8u  gs_isleif_jandas_regards;		//ds:0x344f; seg072, seg116
extern Bit8u  gs_isleif_map_given;		//ds:0x3450; seg072
extern Bit8u  gs_ragna_asked_map;		//ds:0x3451; seg072
extern Bit8u  gs_beorn_litt_bold;		//ds:0x3452; seg072
extern Bit8u  gs_beorn_medi_bold;		//ds:0x3453; seg072
extern Bit8u  gs_beorn_very_bold;		//ds:0x3454; seg072
extern Bit8u  gs_eliane_quest_nameless;		//ds:0x3455; seg072
extern Bit8u  gs_olvir_start_hetmann;		//ds:0x3456; seg072
extern Bit8u  gs_olvir_start_hyggelik;		//ds:0x3457; seg072
extern Bit8u  gs_olvir_start_hasgar;		//ds:0x3458; seg072
extern Bit8u  gs_olvir_interrupted;		//ds:0x3459; seg072
extern Bit8u  gs_olvir_singing_hetmann;		//ds:0x345a; seg072
extern Bit8u  gs_olvir_singing_hyggelik;	//ds:0x345b; seg072
extern Bit8u  gs_olvir_singing_hasgar;		//ds:0x345c; seg072
extern Bit8u  gs_swafnild_tp1;			//ds:0x345d; seg030, seg072
extern Bit8u  gs_swafnild_tp2;			//ds:0x345e; seg030, seg072
extern Bit8u  gs_swafnild_tp3;			//ds:0x345f; seg030, seg072
extern Bit8u  gs_swafnild_tp4;			//ds:0x3460; seg030, seg072
extern Bit8u  gs_swafnild_destination;		//ds:0x3461; seg030, seg072
extern Bit8u  gs_swafnild_nomap;		//ds:0x3462; seg072
extern Bit8u  gs_treborn_date;			//ds:0x3463; seg072
extern Bit8u  gs_unicorn_hero_pos;		//ds:0x3464; seg072
extern Bit8u  gs_unicorn_get_map;		//ds:0x3465; seg002, seg072, seg116
extern Bit8u  gs_unicorn_timer;			//ds:0x3466; seg002, seg072, seg116
extern Bit8u  gs_algrid_treborns_hint;		//ds:0x3467; seg072
extern Bit8u  gs_tiomar_drinkmate;		//ds:0x3468; seg030, seg072
extern Bit8u  gs_tiomar_awaits_letter;		//ds:0x3469; seg072
extern Bit8u *gs_unicorn_hero_ptr;		//ds:0x346a; seg030, seg072
extern Bit16s gs_npc_meet_tavern;		//ds:0x346e; seg060, seg095
extern Bit16s gs_npc_months;			//ds:0x3470; seg002-seg116
extern Bit8u  gs_smith_kicked_flags[50];	//ds:0x3472; seg058
extern Bit8u  gs_smith_flogged_flags[50];	//ds:0x34a4; seg058
extern Bit8u  gs_merchant_kicked_flags[94];	//ds:0x34d6; seg055, seg068
extern Bit8u  gs_merchant_kicked2_flags[94];	//ds:0x3534; seg055
extern Bit8u  gs_merchant_offended_flags[94];	//ds:0x3592; seg002, seg055-seg057
extern Bit8u  gs_quest_deadship;		//ds:0x35f0; seg063, seg073
extern Bit8u  gs_quest_deadship_done;		//ds:0x35f2; seg063, seg077
extern Bit8s  gs_always_one2;			//ds:0x35f3; seg078
extern Bit8u  gs_dng03_spidereggs_burned;	//ds:0x35f4; seg079
extern Bit8u  gs_dng06_dasptreas_found;		//ds:0x35f7; seg081
extern Bit8u  gs_quest_nameless_done;		//ds:0x35fa; seg072, seg084
extern Bit8u  gs_dng14_ugdalf_done;		//ds:0x35ff; seg068, seg087
extern Bit8u  gs_got_grimring;			//ds:0x3600; seg089
extern Bit8s  gs_npc_timers[7];			//ds:0x3601; seg002, seg095, seg113
extern Bit8u  gs_tlk_tav_firstinfo;		//ds:0x3608; seg060
extern Bit8u  gs_tlk_tav_infopromise;		//ds:0x3609; seg060
extern Bit8u  gs_tlk_tav_round;			//ds:0x360a; seg060
extern Bit8u  gs_tlk_tav_freebeer;		//ds:0x360b; seg060
extern Bit8u  gs_tlk_tav_drinkcount;		//ds:0x360c; seg060
extern Bit8u  gs_tlk_tav_informersex;		//ds:0x360d; seg060, seg073
extern Bit8u  gs_tlk_tav_followinformer;	//ds:0x360f; seg059, seg060, seg072
extern Bit8u  gs_tlk_tav_fullness;		//ds:0x3611; seg060
extern Bit8u  gs_tlk_tav_testdrunk;		//ds:0x3611; seg060
extern Bit8u  gs_current_informer;		//ds:0x3612; seg031, seg060, seg073
extern Bit8u  gs_dungeon_light;			//ds:0x3613; seg025, seg074, seg075, seg076
extern Bit8u  gs_show_travel_map;		//ds:0x3614; seg002, seg025, seg032, seg063, seg093, seg109, seg117
extern Bit8u  gs_hermit_visited;		//ds:0x3615; seg066
extern Bit8u  gs_dungeon_gfx_style;		//ds:0x3616; seg028, seg075, seg076
extern Bit8u  gs_estorik_known;			//ds:0x3617; seg081

extern struct struct_dialog_partner gs_dialog_partners[10];	// ds:0x3618; seg028, seg030, seg031
extern struct struct_dialog_state gs_dialog_states[160];	// ds:0x3794; seg028, seg060

extern Bit8u  gs_dng02_sphere_known;		//ds:0x3c94; seg087
extern Bit8u  gs_dng02_corpse_flag;		//ds:0x3c95; seg078
extern Bit8u  gs_dng02_apparature_destroyed;	//ds:0x3c96; seg078
extern Bit8u  gs_dng02_sphere_timer;		//ds:0x3c97; seg002, seg078
extern Bit8u  gs_dng02_sphere_active;		//ds:0x3c98; seg078
extern Bit8u  gs_dng02_raven_destroyed;		//ds:0x3c99; seg078
extern Bit8u  gs_dng02_secret_door1;		//ds:0x3c9a; seg078
extern Bit8u  gs_dng02_secret_door2;		//ds:0x3c9b; seg078
extern Bit8u  gs_dng02_secret_door3;		//ds:0x3c9c; seg078
extern Bit8u  gs_dng03_lever_trap;		//ds:0x3c9d; seg079
extern Bit8u  gs_dng03_lever_chest11;		//ds:0x3c9e; seg079
extern Bit8u  gs_dng03_sanctum_smashed;		//ds:0x3c9f; seg079
extern Bit8u  gs_dng04_corpse0_flag;		//ds:0x3ca0; seg080
extern Bit8u  gs_dng04_corpse1_flag;		//ds:0x3ca1; seg080
extern Bit8u  gs_dng04_corpse2_flag;		//ds:0x3ca2; seg080
extern Bit8u  gs_dng04_gap_flag;		//ds:0x3ca3; seg080
extern Bit8u  gs_dng04_helmet_flag;		//ds:0x3ca4; seg080
extern Bit8u  gs_dng04_lantern_flag;		//ds:0x3ca5; seg080
extern Bit8u  gs_dng04_deepgap_flag;		//ds:0x3ca6; seg080
extern Bit8u  gs_dng06_pitdoor_flag;		//ds:0x3ca7; seg076, seg081
extern Bit8u  gs_dng06_proviant_flag;		//ds:0x3ca8; seg081
extern Bit8u  gs_dng06_money_flag;		//ds:0x3ca9; seg081
extern Bit8u  gs_dng06_goldkey_flag;		//ds:0x3caa; seg081
extern Bit8s  gs_dng06_countdown_timer;		//ds:0x3cab; seg081
extern Bit8u  gs_dng06_lever_flag;		//ds:0x3cac; seg081
extern Bit8u  gs_dng06_book_flag;		//ds:0x3cad; seg081
extern Bit8u  gs_dng06_fight19_flag;		//ds:0x3cae; seg081
extern Bit8u  gs_dng05_proviant_flag;		//ds:0x3caf; seg080
extern Bit8u  gs_dng05_bats_flag;		//ds:0x3cb0; seg080
extern Bit8u  gs_dng05_god_flag;		//ds:0x3cb1; seg080
extern Bit8u  gs_dng07_muelixier_flag;		//ds:0x3cb2; seg082
extern Bit8u  gs_dng07_antimuelixier_flag;	//ds:0x3cb3; seg082
extern Bit8u  gs_dng07_flicker_flag;		//ds:0x3cb4; seg082
extern Bit8s  gs_dng07_poison_flag;		//ds:0x3cb5; seg082
extern Bit32s gs_dng07_poison_timer;		//ds:0x3cb6; seg002, seg082
extern Bit8u  gs_dng08_bed_00;			//ds:0x3cba; seg083
extern Bit8u  gs_dng08_bed_01;			//ds:0x3cbb; seg083
extern Bit8u  gs_dng08_bed_02;			//ds:0x3cbc; seg083
extern Bit8u  gs_dng08_bed_03;			//ds:0x3cbd; seg083
extern Bit8u  gs_dng08_bed_04;			//ds:0x3cbe; seg083
extern Bit8u  gs_dng08_bed_05;			//ds:0x3cbf; seg083
extern Bit8u  gs_dng08_bed_06;			//ds:0x3cc0; seg083
extern Bit8u  gs_dng08_bed_07;			//ds:0x3cc1; seg083
extern Bit8u  gs_dng08_bed_08;			//ds:0x3cc2; seg083
extern Bit8u  gs_dng08_bed_09;			//ds:0x3cc3; seg083
extern Bit8u  gs_dng08_bed_10;			//ds:0x3cc4; seg083
extern Bit8u  gs_dng08_bed_11;			//ds:0x3cc5; seg083
extern Bit8u  gs_dng08_timer1;			//ds:0x3cc6; seg002, seg083
extern Bit8u  gs_dng08_timer2;			//ds:0x3cc7; seg002, seg083
extern Bit8u  gs_dng08_bundles_done;		//ds:0x3cc8; seg083
extern Bit8u  gs_dng08_ballista_done;		//ds:0x3cc9; seg083
extern Bit8u  gs_dng08_chest2_looted;		//ds:0x3cca; seg083
extern Bit8u  gs_dng08_secret_door;		//ds:0x3ccb; seg083
extern Bit8u  gs_dng08_chest0_looted;		//ds:0x3ccc; seg083
extern Bit8u  gs_dng08_chest35_looted;		//ds:0x3ccd; seg083
extern Bit8u  gs_daspota_location01_loot[29];	//ds:0x3cce; seg???
extern Bit8u  gs_daspota_location02_loot[15];	//ds:0x3ceb; seg???
extern Bit8u  gs_daspota_location03_loot[15];	//ds:0x3cfa; seg???
extern Bit8u  gs_daspota_location04_loot[9];	//ds:0x3d09; seg???
extern Bit8u  gs_daspota_location05_loot[31];	//ds:0x3d12; seg???
extern Bit8u  gs_daspota_location06_loot[25];	//ds:0x3d31; seg???
extern Bit8u  gs_daspota_location07_loot[7];	//ds:0x3d4a; seg???
extern Bit8u  gs_daspota_location08_loot[15];	//ds:0x3d51; seg???
extern Bit8u  gs_daspota_location10_loot[7];	//ds:0x3d60; seg???
extern Bit8u  gs_daspota_location12_loot[21];	//ds:0x3d67; seg???
extern Bit8u  gs_daspota_location13_loot[9];	//ds:0x3d7c; seg???
extern Bit8u  gs_daspota_location14_loot[5];	//ds:0x3d85; seg???
extern Bit8u  gs_daspota_location16_loot[5];	//ds:0x3d8a; seg???
extern Bit8u  gs_daspota_location18_loot[19];	//ds:0x3d8f; seg???
extern Bit8u  gs_tevent004_flag;		//ds:0x3da2; seg073, seg109
extern Bit8u  gs_tevent005_flag;		//ds:0x3da3; seg073, seg109
extern Bit8u  gs_tevent008_flag;		//ds:0x3da4; seg073, seg109
extern Bit8u  gs_tevent009_flag;		//ds:0x3da5; seg073, seg109
extern Bit8u  gs_tevent011_flag;		//ds:0x3da6; seg073, seg110
extern Bit8u  gs_tevent013_flag;		//ds:0x3da7; seg073, seg110
extern Bit8u  gs_tevent014_flag;		//ds:0x3da8; seg110
extern Bit8u  gs_tevent016_flag;		//ds:0x3da9; seg115
extern Bit8u  gs_tevent017_flag;		//ds:0x3daa; seg073, seg110
extern Bit8u  gs_tevent020_flag;		//ds:0x3dab; seg073, seg110
extern Bit8u  gs_tevent021_flag;		//ds:0x3dac; seg073, seg110
extern Bit8u  gs_tevent022_flag;		//ds:0x3dad; seg073, seg110
extern Bit8u  gs_tevent022_track_flag;		//ds:0x3dae; seg110
extern Bit8u  gs_tevent024_flag;		//ds:0x3daf; seg073, seg110
extern Bit8u  gs_tevent025_flag;		//ds:0x3db0; seg073, seg110
extern Bit8u  gs_tevent028_flag;		//ds:0x3db1; seg073, seg110
extern Bit8u  gs_tevent030_flag;		//ds:0x3db2; seg073, seg110
extern Bit8u  gs_tevent031_flag;		//ds:0x3db3; seg110
extern Bit8u  gs_tevent032_flag;		//ds:0x3db4; seg073, seg110
extern Bit8u  gs_tevent032_herb_flag;		//ds:0x3db5; seg073, seg110
extern Bit8u  gs_tevent034_flag;		//ds:0x3db6; seg073, seg110
extern Bit8u  gs_tevent035_flag;		//ds:0x3db7; seg073, seg110
extern Bit8u  gs_tevent036_flag;		//ds:0x3db8; seg073, seg110
extern Bit8u  gs_tevent036_herb_flag;		//ds:0x3db9; seg110
extern Bit8u  gs_tevent037_flag;		//ds:0x3dba; seg118
extern Bit8u  gs_tevent039_flag;		//ds:0x3dbb; seg110
extern Bit8u  gs_tevent040_flag;		//ds:0x3dbc; seg073, seg110
extern Bit8u  gs_tevent041_flag;		//ds:0x3dbd; seg110
extern Bit8u  gs_tevent042_flag;		//ds:0x3dbe; seg110
extern Bit8u  gs_tevent043_flag;		//ds:0x3dbf; seg110
extern Bit8u  gs_tevent048_flag;		//ds:0x3dc0; seg073, seg110
extern Bit8u  gs_tevent048_track_flag;		//ds:0x3dc1; seg110
extern Bit8u  gs_tevent049_flag;		//ds:0x3dc2; seg073, seg110
extern Bit8u  gs_tevent050_flag;		//ds:0x3dc3; seg073, seg110
extern Bit8u  gs_tevent050_herb_flag;		//ds:0x3dc4; seg110
extern Bit8u  gs_tevent051_flag;		//ds:0x3dc5; seg118
extern Bit8u  gs_tevent053_flag;		//ds:0x3dc6; seg073, seg111
extern Bit8u  gs_tevent058_flag;		//ds:0x3dc7; seg073, seg111
extern Bit8u  gs_tevent061_flag;		//ds:0x3dc8; seg111
extern Bit8u  gs_tevent064_flag;		//ds:0x3dc9; seg111
extern Bit8u  gs_tevent066_flag;		//ds:0x3dca; seg111
extern Bit8u  gs_tevent066_track_flag;		//ds:0x3dcb; seg073, seg111
extern Bit8u  gs_tevent067_flag;		//ds:0x3dcc; seg112
extern Bit8u  gs_tevent069_flag;		//ds:0x3dcd; seg073, seg112
extern Bit8u  gs_tevent070_flag;		//ds:0x3dce; seg073, seg112
extern Bit8u  gs_tevent070_herb_flag;		//ds:0x3dcf; seg073, seg112
extern Bit8u  gs_tevent070_trail_flag;		//ds:0x3dd0; seg112
extern Bit8u  gs_tevent071_flag;		//ds:0x3dd1; seg112
extern Bit8u  gs_teventu01_flag;		//ds:0x3dd2; seg112
extern Bit8u  gs_tevent064_silent_flag;		//ds:0x3dd3; seg111
extern Bit8u  gs_teventu02_flag;		//ds:0x3dd5; seg073
extern Bit8u  gs_teventu03_flag;		//ds:0x3dd6; seg073
extern Bit8u  gs_teventu04_flag;		//ds:0x3dd7; seg073
extern Bit8u  gs_teventu05_flag;		//ds:0x3dd8; seg073
extern Bit8u  gs_teventu06_flag;		//ds:0x3dd9; seg073
extern Bit8u  gs_tevent071_orcstatue;		//ds:0x3dda; seg041, seg042, seg043, seg112
extern Bit8u  gs_met_unicorn_flag;		//ds:0x3ddb; seg116, seg117, seg118
extern Bit8u  gs_find_hyggelik;			//ds:0x3ddc; seg025, seg110, seg117
extern Bit8u  gs_tevent072_flag;		//ds:0x3ddd; seg073, seg112
extern Bit8u  gs_tevent073_flag;		//ds:0x3dde; seg112
extern Bit8u  gs_tevent074_flag;		//ds:0x3ddf; seg112
extern Bit8u  gs_tevent075_flag;		//ds:0x3de0; seg073, seg112
extern Bit8u  gs_tevent076_flag;		//ds:0x3de1; seg112
extern Bit8u  gs_tevent077_flag;		//ds:0x3de2; seg112
extern Bit8u  gs_tevent079_flag;		//ds:0x3de3; seg073, seg118
extern Bit8u  gs_tevent080_flag;		//ds:0x3de4; seg113
extern Bit8u  gs_tevent080_tatzelwurm;		//ds:0x3de5; seg113
extern Bit8u  gs_tevent081_flag;		//ds:0x3de6; seg073, seg113
extern Bit8u  gs_tevent083_flag;		//ds:0x3de7; seg073, seg113
extern Bit8u  gs_tevent084_flag;		//ds:0x3de8; seg113
extern Bit8u  gs_tevent085_flag;		//ds:0x3de9; seg113
extern Bit8u  gs_tevent085_herb_flag;		//ds:0x3dea; seg113
extern Bit8u  gs_tevent086_flag;		//ds:0x3deb; seg113
extern Bit8u  gs_tevent088_flag;		//ds:0x3dec; seg113, seg116
extern Bit8u  gs_tevent091_flag;		//ds:0x3ded; seg073, seg115, seg116
extern Bit8u  gs_tevent093_flag;		//ds:0x3dee; seg073, seg115, seg116
extern Bit8u  gs_tevent094_flag;		//ds:0x3def; seg110, seg115
extern Bit8u  gs_teventu07_flag;		//ds:0x3df0; seg073
extern Bit8u  gs_teventu08_flag;		//ds:0x3df1; seg073
extern Bit8u  gs_tevent099_flag;		//ds:0x3df2; seg113
extern Bit8u  gs_tevent100_flag;		//ds:0x3df3; seg072, seg115
extern Bit8u  gs_tevent101_flag;		//ds:0x3df4; seg113
extern Bit8u  gs_tevent106_flag;		//ds:0x3df5; seg073, seg113
extern Bit8u  gs_tevent108_flag;		//ds:0x3df6; seg113
extern Bit8u  gs_tevent109_flag;		//ds:0x3df7; seg113
extern Bit8u  gs_tevent111_flag;		//ds:0x3df8; seg114
extern Bit8u  gs_tevent112_flag;		//ds:0x3df9; seg073, seg114
extern Bit8u  gs_tevent112_herb_flag;		//ds:0x3dfa; seg073, seg114
extern Bit8u  gs_tevent115_flag;		//ds:0x3dfb; seg117
extern Bit8u  gs_tevent116_flag;		//ds:0x3dfc; seg073, seg114
extern Bit8u  gs_tevent118_flag;		//ds:0x3dfd; seg073, seg114
extern Bit8u  gs_tevent119_flag;		//ds:0x3dfe; seg073, seg114
extern Bit8u  gs_tevent120_flag;		//ds:0x3dff; seg073, seg118,
extern Bit8u  gs_tevent122_flag;		//ds:0x3e00; seg114
extern Bit8u  gs_tevent128_flag;		//ds:0x3e02; seg073, seg115
extern Bit8u  gs_tevent128_replen_flag;		//ds:0x3e03; seg115
extern Bit8u  gs_tevent129_flag;		//ds:0x3e04; seg115
extern Bit8u  gs_tevent131_flag;		//ds:0x3e05; seg116
extern Bit8u  gs_tevent132_flag;		//ds:0x3e06; seg073, seg116
extern Bit8u  gs_tevent132_herb_flag;		//ds:0x3e07; seg073, seg116
extern Bit8u  gs_tevent134_flag;		//ds:0x3e08; seg066, seg116
extern Bit8u  gs_tevent137_flag;		//ds:0x3e09; seg066, seg116
extern Bit8u  gs_tevent138_flag;		//ds:0x3e0a; seg087, seg116
extern Bit8u  gs_teventu09_flag;		//ds:0x3e0b; seg073
extern Bit8u  gs_tevent140_flag;		//ds:0x3e0c; seg116
extern Bit8u  gs_tevent140_herb_flag;		//ds:0x3e0d; seg116
extern Bit8u  gs_tevent141_flag;		//ds:0x3e0e; seg116
extern Bit8u  gs_tevent142_flag;		//ds:0x3e0f; seg116

extern Bit8u  gs_teventu10_flag;		//ds:0x3e11; seg073
extern Bit8u  gs_teventu11_flag;		//ds:0x3e12; seg073
extern Bit8u  gs_teventu12_flag;		//ds:0x3e13; seg073
extern Bit8u  gs_teventu13_flag;		//ds:0x3e14; seg073
extern Bit8u  gs_tevent114_olimone_flag;	//ds:0x3e15; seg114
extern Bit8u  gs_tevent074_fight_flag;		//ds:0x3e16; seg112
extern Bit8u  gs_tevent077_fight_flag;		//ds:0x3e17; seg112

#if defined(__BORLANDC__)
extern Bit8u *gs_random_tlk_hero;		//ds:0xe318; seg0030, seg116
#endif
extern Bit8u *gs_ruin_hero;			//ds:0xe31c; seg030, seg117
extern Bit8u *gs_main_acting_hero;		//ds:0xe320; seg002, seg033, seg039, seg051, seg092, seg113, seg116

extern Bit8u  gs_tevent073_corpse[3];		//ds:0x3e24; seg112
extern Bit8u  gs_tevent014_chest[4];		//ds:0x3e27; seg110
extern Bit8u  gs_tevent064_hut_content[19];	//ds:0x3e2b; seg111
extern Bit8u  gs_tevent064_chest[21];		//ds:0x3e3e; seg111
extern Bit8u  gs_palette_floor[3 * 32];		//ds:0x3e53; seg002
extern Bit8u  gs_palette_buildings[3 * 32];	//ds:0x3eb3; seg002, seg028, seg066
extern Bit8u  gs_palette_sky[3 * 32];		//ds:0x3f13; seg002, seg028, seg066, seg075
extern Bit8u  gs_need_letter;			//ds:0x3f73; seg068, seg069
extern Bit8u  gs_gremob_invited;		//ds:0x3f74; seg070, seg071
extern Bit8u  gs_harlot_date;			//ds:0x3f75; seg070
extern Bit8u  gs_drug_timer;			//ds:0x3f76; seg002, seg071
extern Bit8u  gs_ptemple_closed;		//ds:0x3f77; seg071
extern Bit8u  gs_alrik_derondan;		//ds:0x3f78; seg070, seg071, seg073
extern Bit8u  gs_alrik_derondan_insulted;	//ds:0x3f79; seg071
extern Bit8u  gs_dng13_lantern_flag;		//ds:0x3f7a; seg091
extern Bit8s  gs_dng13_corpse0_flag;		//ds:0x3f7b; seg091
extern Bit8s  gs_dng13_corpse1_flag;		//ds:0x3f7c; seg091
extern Bit8u  gs_dng13_passage1_flag;		//ds:0x3f7d; seg091
extern Bit8u  gs_dng13_passage2_flag;		//ds:0x3f7e; seg091
extern Bit8u  gs_dng13_passage3_flag;		//ds:0x3f7f; seg091
extern Bit8u  gs_dng13_passage4_flag;		//ds:0x3f80; seg091
extern Bit8u  gs_dng13_passage5_flag;		//ds:0x3f81; seg091
extern Bit8u  gs_dng13_money_flag;		//ds:0x3f82; seg091
extern Bit8u  gs_dng13_chest_equips[13];	//ds:0x3f83; seg091
extern Bit8u  gs_dng13_chest0_content[6];	//ds:0x3f90; seg091
extern Bit8u  gs_dng13_chest1_content[5];	//ds:0x3f96; seg091
extern Bit8s  gs_dng12_tunnel1;			//ds:0x3f9b; seg090
extern Bit8s  gs_dng12_tunnel2;			//ds:0x3f9c; seg090
extern Bit8s  gs_dng12_tunnel3;			//ds:0x3f9d; seg090
extern Bit8s  gs_dng12_tunnel4;			//ds:0x3f9e; seg090
extern Bit8u  gs_dng12_ingerimm_sacrifice;	//ds:0x3f9f; seg090, seg105
extern Bit8u  gs_dng12_ingerimm_hint;		//ds:0x3fa0; seg090
extern Bit8s  gs_dng12_watertrap_water_runs;	//ds:0x3fa1; seg090
extern Bit32s gs_dng12_watertrap_timer;		//ds:0x3fa2; seg002, seg090
extern Bit8u  gs_dng12_watertrap_active;	//ds:0x3fa6; seg090
extern Bit8s  gs_dng12_obstacle_hp;		//ds:0x3fa7; seg090
extern Bit8u  gs_dng12_obstacle_active;		//ds:0x3fa8; seg090
extern Bit8u  gs_dng12_speartrap_active;	//ds:0x3fa9; seg090
extern Bit8u  gs_dng12_chest1_content[12];	//ds:0x3faa; seg090
extern Bit8u  gs_dng14_money_flag;		//ds:0x3fb6; seg087
extern Bit8u  gs_dng14_alarm_flag;		//ds:0x3fb7; seg087
extern Bit8u  gs_dng14_secretdoor1;		//ds:0x3fb8; seg087
extern Bit8u  gs_dng14_secretdoor2;		//ds:0x3fb9; seg087
extern Bit8u  gs_dng14_secretdoor3;		//ds:0x3fba; seg087
extern Bit8u  gs_dng14_secretdoor4;		//ds:0x3fbb; seg087
extern Bit8u  gs_dng14_secretdoor5;		//ds:0x3fbc; seg087
extern Bit8u  gs_dng14_poisontrap;		//ds:0x3fbd; seg087
extern Bit8u  gs_dng14_lvl2_fight;		//ds:0x3fbe; seg087
extern Bit8u  gs_dng14_booty_flag;		//ds:0x3fbf; seg087
extern Bit8u  gs_dng14_torches_flag;		//ds:0x3fc0; seg087
extern Bit8u  gs_dng14_spear_flag;		//ds:0x3fc1; seg087
extern Bit8u  gs_dng14_hatchet_flag;		//ds:0x3fc2; seg087
extern Bit8u  gs_dng14_ring_flag;		//ds:0x3fc3; seg087
extern Bit8u  gs_dng14_orknase_flag;		//ds:0x3fc4; seg087
extern Bit8u  gs_dng14_spooky_flag;		//ds:0x3fc4; seg087
extern Bit8u  gs_dng14_cellarexit_flag;		//ds:0x3fc6; seg058, seg087
extern Bit8u  gs_dng14_chest_pantry[9];		//ds:0x3fc7; seg087
extern Bit8u  gs_dng14_chest_gear[13];		//ds:0x3fd0; seg087
extern Bit8u  gs_dng14_chest_barrel[7];		//ds:0x3fdd; seg087
extern Bit8u  gs_dng14_chest_x1[7];		//ds:0x3fe4; seg088
extern Bit8u  gs_dng14_chest_x2[16];		//ds:0x3feb; seg088
extern Bit8u  gs_dng14_chest_x3[11];		//ds:0x3ffb; seg088
extern Bit8u  gs_dng14_chest_x4[14];		//ds:0x4006; seg088
extern Bit8u  gs_dng14_chest_x5[10];		//ds:0x4014; seg088
extern Bit8u  gs_dng14_chest_x6[8];		//ds:0x401e; seg088
extern Bit8u  gs_dng14_chest_x7[7];		//ds:0x4026; seg088
extern Bit8u  gs_dng14_chest_x8[13];		//ds:0x402d; seg088
extern Bit8u  gs_dng14_fullmap_flag;		//ds:0x403a; seg088

extern Bit8u  gs_dng02_chest00_content[2];	//ds:0x403b; seg078
extern Bit8u  gs_dng02_chest01_content[4];	//ds:0x403d; seg078
extern Bit8u  gs_dng02_chest06_content[4];	//ds:0x4041; seg078
extern Bit8u  gs_dng02_chest02_content[8];	//ds:0x4045; seg078
extern Bit8u  gs_dng02_chest03_content[8];	//ds:0x404d; seg078
extern Bit8u  gs_dng02_chest05_content[8];	//ds:0x4055; seg078

extern Bit8u  gs_dng03_chest00_content[7];	//ds:0x405d; seg079
extern Bit8u  gs_dng03_chest01_content[8];	//ds:0x4064; seg079
extern Bit8u  gs_dng03_chest02_content[13];	//ds:0x406c; seg079
extern Bit8u  gs_dng03_chest03_content[3];	//ds:0x4079; seg079
extern Bit8u  gs_dng03_chest04_content[9];	//ds:0x407c; seg079
extern Bit8u  gs_dng03_chest06_content[8];	//ds:0x4085; seg079
extern Bit8u  gs_dng03_chest07_content[2];	//ds:0x408d; seg079
extern Bit8u  gs_dng03_chest08_content[4];	//ds:0x408f; seg079
extern Bit8u  gs_dng03_chest09_content[11];	//ds:0x4093; seg079
extern Bit8u  gs_dng03_chest12_content[7];	//ds:0x409e; seg079

extern Bit8u  gs_dng04_chest_corpse0[4];	//ds:0x40a5; seg080
extern Bit8u  gs_dng04_chest_corpse1[12];	//ds:0x40a9; seg080
extern Bit8u  gs_dng04_chest_corpse2[10];	//ds:0x40b5; seg080
extern Bit8u  gs_dng06_chest1_content[16];	//ds:0x40bf; seg081
extern Bit8u  gs_dng06_chest_dasptreas[17];	//ds:0x40cf; seg081
extern Bit8u  gs_dng08_chest1_content[11];	//ds:0x40e0; seg083
extern Bit8u  gs_dng08_chest2_content[2];	//ds:0x40eb; seg083
extern Bit8u  gs_dng08_chest4_content[2];	//ds:0x40ed; seg083
extern Bit8u  gs_dng08_chest5_content[6];	//ds:0x40ef; seg083

extern Bit8u  gs_dng09_lever1_flag;		//ds:0x40f5; seg084
extern Bit8u  gs_dng09_bolttrap1_flag;		//ds:0x40f6; seg084
extern Bit8u  gs_dng09_bolttrap2_flag;		//ds:0x40f7; seg084
extern Bit8u  gs_dng09_bolttrap3_flag;		//ds:0x40f8; seg084
extern Bit8u  gs_dng09_cultist_flag;		//ds:0x40f9; seg041, seg084
extern Bit8u  gs_dng09_lever2_flag;		//ds:0x40fa; seg084
extern Bit8u  gs_dng09_bolttrap4_flag;		//ds:0x40fb; seg084
extern Bit8u  gs_dng09_crystal_flag;		//ds:0x40fc; seg084
extern Bit8u  gs_dng09_pit_flag;		//ds:0x40fd; seg084
extern Bit8u  gs_dng09_altar_flag;		//ds:0x40ff; seg081, seg084
extern Bit8u  gs_dng09_secretdoor1;	//ds:0x4100; seg084
extern Bit8u  gs_dng09_secretdoor2;	//ds:0x4101; seg084
extern Bit8u  gs_dng09_chest0[12];	//ds:0x4102; seg084
extern Bit8u  gs_dng09_chest1[10];	//ds:0x410e; seg084
extern Bit8u  gs_dng09_chest2[8];	//ds:0x4118; seg084
extern Bit8u  gs_dng09_chest3[21];	//ds:0x4120; seg084
extern Bit8u  gs_dng09_chest5[4];	//ds:0x4135; seg084
extern Bit8u  gs_dng09_chest6[7];	//ds:0x4139; seg084
extern Bit8u  gs_dng10_corpse_flag;	//ds:0x4140; seg085
extern Bit8u  gs_dng10_lever_found;	//ds:0x4141; seg085
extern Bit8u  gs_dng10_hole_state;	//ds:0x4142; seg085
extern Bit8u  gs_dng10_lever_state;	//ds:0x4143; seg085
extern Bit8u  gs_dng10_hole_damage;	//ds:0x4144; seg085
extern Bit8u  gs_dng10_floorplate_found;//ds:0x4145; seg085
extern Bit8u  gs_dng10_floorplate_loads;//ds:0x4146; seg085
extern Bit8u  gs_dng10_mummy_lever;	//ds:0x4147; seg085
extern Bit8u  gs_dng10_heshtot;		//ds:0x4148; seg085
extern Bit8u  gs_dng10_dragon_quest;	//ds:0x4149; seg085
extern Bit8u  gs_dng10_hoard_plundered;	//ds:0x414a; seg085
extern Bit8u  gs_dng10_chest0_content[5];	//ds:0x414b; seg085
extern Bit8u  gs_dng10_chest1_content[4];	//ds:0x4150; seg085
extern Bit8u  gs_dng10_chest2_content[4];	//ds:0x4154; seg085
extern Bit8u  gs_dng10_chest3_content[3];	//ds:0x4158; seg085
extern Bit8u  gs_dng10_chest4_content[2];	//ds:0x415b; seg085

extern Bit8u  gs_dng11_waterbarrel1;		//ds:0x415d; seg086
extern Bit8u  gs_dng11_waterbarrel2;		//ds:0x415e; seg086
extern Bit8u  gs_dng11_efferd_sacrifice;	//ds:0x415f; seg086
extern Bit8u  gs_dng11_efferd_hint;		//ds:0x4160; seg086
extern Bit8u  gs_dng11_unknown1_flag;		//ds:0x4161; seg086
extern Bit8u  gs_dng11_firetrap1_flag;		//ds:0x4162; seg086
extern Bit8u  gs_dng11_unknown2_flag;		//ds:0x4163; seg086
extern Bit8u  gs_dng11_firetrap2_flag;		//ds:0x4164; seg086
extern Bit8u  gs_dng11_secretdoor1_flag;	//ds:0x4165; seg086
extern Bit8u  gs_dng11_secretdoor2_flag;	//ds:0x4166; seg086
extern Bit8u  gs_dng11_secretdoor3_flag;	//ds:0x4167; seg086
extern Bit8u  gs_dng11_lever_flag;		//ds:0x4168; seg076, seg086
extern Bit8u  gs_dng11_proviant1_flag;		//ds:0x4169; seg086
extern Bit8u  gs_dng11_proviant2_flag;		//ds:0x416a; seg086
extern Bit8u  gs_dng11_chest3[15];		//ds:0x416b; seg086
extern Bit8u  gs_dng11_chest4[11];		//ds:0x417a; seg086
extern Bit8u  gs_dng11_chest5[5];		//ds:0x4185; seg086
extern Bit8u  gs_dng11_chest6[7];		//ds:0x418a; seg086
extern Bit8u  gs_dng11_chest7;			//ds:0x4191; seg086
extern Bit8u  gs_dng11_chest8[6];		//ds:0x4192; seg086
extern Bit8u  gs_dng11_chest9[6];		//ds:0x4198; seg086
extern Bit8u  gs_dng11_chest10[11];		//ds:0x419e; seg086
extern Bit8u  gs_dng11_chest11[4];		//ds:0x41a9; seg086
extern Bit8u  gs_dng11_chest12[13];		//ds:0x41ad; seg086
extern Bit8u  gs_dng11_chest13[6];		//ds:0x41ba; seg086
extern Bit8u  gs_dng11_chest14[6];		//ds:0x41c0; seg086
extern Bit8u  gs_dng15_took_cursed_money;	//ds:0x41c6; seg032, seg089
extern Bit8u  gs_dng15_unknown_flag;		//ds:0x41c8; seg076, seg088
extern Bit8u  gs_dng15_reached_hands;		//ds:0x41c8; seg076, seg088
extern Bit8u  gs_dng15_lever_south;		//ds:0x41c9; seg076, seg088, seg089
extern Bit8u  gs_dng15_lever_north;		//ds:0x41ca; seg076, seg088, seg089
extern Bit8u  gs_dng15_took_hoe;		//ds:0x41cb; seg089
extern Bit8u  gs_dng15_ceilings[18];		//ds:0x41cc; seg089
extern Bit8u  gs_dng15_cursed_money[6];		//ds:0x41de; seg089
extern Bit8u  gs_dng15_undead_fight;		//ds:0x41e4; seg089
extern Bit8u  gs_dng15_chest_equips[7];		//ds:0x41e5; seg089
extern Bit8u  gs_dng01_chest1_content[8];	//ds:0x41ec; seg077
extern Bit8u  gs_dng01_chest2_content[10];	//ds:0x41f4; seg077
extern Bit8u  gs_dng01_chest3_content[2];	//ds:0x41fe; seg077
extern Bit8u  gs_dng01_chest5_content[13];	//ds:0x4200; seg077
extern Bit8u  gs_dng01_chest7_content[5];	//ds:0x420d; seg077
extern Bit8s  gs_unconscious_message[7];	//ds:0x4212; seg002, seg049
extern Bit8s  gs_food_message[7];		//ds:0x4212; seg002, seg049, seg059
extern Bit16s gs_current_locdata;		//ds:0x4222; seg025-seg109
extern Bit16s gs_current_typeindex;		//ds:0x4224; seg025-seg120
extern Bit8u  gs_dng03_highpriest_killed;	//ds:0x4226; seg079
extern Bit8s  gs_dng03_chest12_loads;		//ds:0x4227; seg079
extern Bit16s gs_trv_i;				//ds:0x4228; seg063, seg094
extern Bit16s gs_route_stepcount;		//ds:0x422a; seg063, seg094
extern Bit16s gs_forcedmarch_le_cost;		//ds:0x422c; seg094
extern Bit16s gs_route_total_steps;			//ds:0x422e; seg063, seg094
extern Bit16s gs_route_length;				//ds:0x4230; seg063, seg094
extern Bit16s gs_route_duration;			//ds:0x4232; seg063, seg094
extern Bit16s gs_route_timedelta;			//ds:0x4234; seg063, seg094
extern Bit16s gs_route_mousehover;			//ds:0x4236; seg063, seg094
extern Bit16s gs_route_progress;			//ds:0x4238; seg063, seg094
extern Bit16s gs_route_stepsize;			//ds:0x423a; seg063, seg094
extern Bit16s gs_route_dayprogress;			//ds:0x423c; seg094
extern Bit16s gs_sea_travel_passage_no;			//ds:0x423e; seg063
extern Bit16s gs_route_encounter_flag;			//ds:0x4240; seg094
extern Bit16s gs_route_encounter_time;			//ds:0x4242; seg094
extern Bit16s gs_route_informer_flag;			//ds:0x4244; seg094
extern Bit16s gs_route_informer_time;			//ds:0x4246; seg094
extern Bit16s gs_route_fight_flag;			//ds:0x4248; seg094, seg117
extern Bit16s gs_route_fight_time;			//ds:0x424a; seg094
extern Bit16s gs_travel_speed;				//ds:0x424c; seg063, seg094
extern Bit16s gs_passage_deadship_flag;			//ds:0x424e; seg063
extern Bit16s gs_passage_deadship_position;		//ds:0x4250; seg063
extern Bit16s gs_passage_octopus_flag;			//ds:0x4252; seg063
extern Bit16s gs_passage_octopus_position;		//ds:0x4254; seg063
extern Bit16s gs_passage_pirates_flag;			//ds:0x4256; seg063
extern Bit16s gs_passage_pirates_position;		//ds:0x4258; seg063
extern Bit8u *gs_route_course_ptr;			//ds:0x425a; seg063, seg094
extern Bit8u *gs_route_course_start;			//ds:0x425e; seg063, seg094
extern Bit8u *gs_route_course_ptr2;			//ds:0x4262; seg063, seg094
extern Bit8u *gs_sea_travel_courses;			//ds:0x4266; seg063
extern struct struct_tevent *gs_tevents_tab_ptr;	//ds:0x426a; seg094
extern struct struct_land_route *gs_travel_route_ptr;	//ds:0x426e; seg094
extern struct struct_route_tevent gs_route_tevents[15]; //ds:0x4272; seg094
extern Bit8u  gs_sea_travel_psgbooked_flag;	//ds:0x42ae; seg002, seg063
extern Bit8u  gs_sea_travel_psgbooked_timer;	//ds:0x42af; seg002, seg063
extern Bit8s  gs_sea_travel_passage_speed1;	//ds:0x42b0; seg063
extern Bit8u  gs_current_sea_route_id;		//ds:0x42b1; seg063, seg064
// TODO: HORBOUR OPTIONS: contains pointers
extern struct harbor_option_obsolete gs_harbor_options[10];	//ds:0x42b2; seg063, seg064
extern Bit16s gs_sea_travel_passage_price;	//ds:0x432a; seg063, seg064
extern Bit16s gs_sea_travel_passage_speed2;	//ds:0x432c; seg063, seg064
extern Bit8u *gs_travel_map_ptr;		//ds:0x432e; seg028, seg063, seg093, seg094
extern Bit8u  gs_forcedmarch_timer;		//ds:0x4332; seg002, seg094
extern Bit8u  gs_travel_detour;			//ds:0x4333; seg002-seg118
extern Bit16s gs_current_signpost;		//ds:0x4334; seg025, seg063, seg093
extern Bit16s gs_trv_return;			//ds:0x4336; seg093-seg116
extern Bit16s gs_travel_destination_town_id;	//ds:0x4338; seg063-seg094
extern Bit16s gs_travel_destination_x;		//ds:0x433a; seg063-seg094
extern Bit16s gs_travel_destination_y;		//ds:0x433c; seg063-seg094
extern Bit16s gs_travel_destination_viewdir;	//ds:0x433e; seg063-seg094

#if defined(__BORLANDC__)
extern struct trv_start_point *gs_tm_unused1_ptr;		//ds:0x4340; seg094
#else
extern Bit32u gs_tm_unused1_ptr_obsolete;	//ds:0x4340; UNUSED
#endif

extern Bit8s  gs_trv_menu_towns[6];		//ds:0x4344; seg002, seg093, seg094
extern Bit16s gs_trv_destination;		//ds:0x434a; seg078-seg118
extern Bit8u  gs_dng08_waterbarrel;		//ds:0x434c; seg083
extern Bit8u  gs_dng13_collapsecount;		//ds:0x434d; seg075, seg091
extern Bit8u  gs_dng13_herocount;		//ds:0x434d; seg075, seg091
extern Bit16s gs_camp_incident;			//ds:0x434f; seg051, seg052, seg11, seg111, seg116
extern Bit8s  gs_known_monsters[78];		//ds:0x434f; seg035
extern Bit8u  gs_nameless_destroyed;		//ds:0x439f; seg082
extern Bit8u  gs_deadship_final;		//ds:0x43a0; seg077
extern Bit8u  gs_dng09_lever_fast;		//ds:0x43a1; seg084
extern Bit8u  gs_dng11_soup_supply;		//ds:0x43a2; seg086
extern Bit16s gs_arsenal_money;			//ds:0x43a3; seg068
extern Bit8s  gs_announce_day;			//ds:0x43a5; seg073

extern Bit8s  gs_known_persons[20];		//ds:0x43a6; seg025
extern Bit16s gs_diary_entry_counter;		//ds:0x43ba; seg024
extern struct struct_diary_entry gs_diary_entries[23];	//ds:0x43bc; seg024

extern Bit8u  gs_datseg_status_end;		//ds:0x4474; seg026
/* GameState written directly into SaveGame file ends at 0x4474 */

extern unsigned char g_fading_state;		//ds:0x4475; seg002-seg095
extern signed char g_check_party;		//ds:0x4495; seg002, seg032, seg063, seg113
extern signed char g_food_mod;			//ds:0x4496; seg002, seg051, seg052, seg054
extern signed char g_travel_herokeeping;	//ds:0x4497; seg002, seg063
extern signed char g_special_screen;		//ds:0x45b8; seg002-seg109
extern signed char g_months_spring[4];		//ds:0x463a; seg002
extern signed char g_months_winter[4];		//ds:0x463e; seg002
extern signed char g_months_summer[4];		//ds:0x4642; seg002
extern signed short g_bank_heller;		//ds:0x4646; seg002
extern signed char g_consume_quiet;		//ds:0x4648; seg002, seg108
extern unsigned char g_herokeeping_flag;	//ds:0x4649; seg002
extern unsigned char g_delay_or_keypress_flag;	//ds:0x46a2; seg002
extern const signed short g_merchant_items_posx[15];	//ds:0x46a3; seg002
extern const signed short g_merchant_items_posy[15];	//ds:0x46c1; seg002
extern signed char g_refresh_status_line;	//ds:0x46df; seg002, seg095
extern char g_dsa_version_string[12];		//ds:0x46e0; seg026
extern char g_game_version[6];			//ds:0x46ec; seg002
extern char g_version_token1;			//ds:0x46f8; seg026
extern char g_version_token2;			//ds:0x46f9; seg026
extern char g_version_token3;			//ds:0x46fa; seg026
extern char g_version_token4;			//ds:0x46fb; seg026
extern const char g_questions_handbook[10][19];	//ds:0x46fc; seg002
extern const signed char g_questions_map[10][3];	//ds:0x47ba; seg002
extern char g_snd_menu_title[17];		//ds:0x47d8; seg002
extern char g_snd_menu_radio1[12];		//ds:0x479e; seg002
extern char g_snd_menu_radio2[10];		//ds:0x47f5; seg002
extern char g_snd_menu_radio3[12];		//ds:0x47ff; seg002
extern char g_snd_menu_radio4[18];		//ds:0x480b; seg002
extern char g_fname_sound_cfg[10];		//ds:0x481d; seg002
extern char g_fname_sound_adv2[10];		//ds:0x4827; seg002
extern char g_fname_digi_adv[9];		//ds:0x4831; seg002
extern char g_snd_txt_disabled_mem[51];		//ds:0x483a; seg002
extern char g_snd_txt_hw_not_found[31];		//ds:0x486d; seg002
extern char g_fname_sound_adv[10];		//ds:0x488c; seg002
extern char g_snd_txt_hw_not_found2[31];	//ds:0x4896; seg002
extern char g_fname_schick_dat[11];		//ds:0x48ca; seg002
extern char g_snd_txt_disabled_mem2[43];	//ds:0x48d5; seg002
extern char g_diary_string1[14];		//ds:0x4900; seg024
extern char g_diary_string2[17];		//ds:0x490e; seg024
extern char g_diary_string3[27];		//ds:0x491f; seg024
extern char g_diary_string4[6];			//ds:0x493a; seg024
extern char g_diary_string5[9];			//ds:0x4940; seg024
extern char g_diary_string6[19];		//ds:0x4949; seg024
extern char g_diary_string7[19];		//ds:0x495c; seg024

extern char   g_noway_string[40];		//ds:0x4a68; seg003
extern Bit16s g_ani_busy;			//ds:0x4a90; seg004
extern Bit16s g_pic_copy_flag;			//ds:0x4a92; seg004, seg008, seg074
extern Bit16u g_status_bar_colors[2];		//ds:0x4a94; seg004
extern Bit8s  g_status_page_hunger_max_counter;	//ds:0x4a9a; seg004
extern Bit8s  g_status_page_hunger_max_color;	//ds:0x4a9c; seg004
extern Bit8s  g_status_page_thirst_max_counter;	//ds:0x4a9d; seg004
extern Bit8s  g_status_page_thirst_max_color;	//ds:0x4a9e; seg004
extern Bit16s g_wallclock_pos;			//ds:0x4a9e; seg004;
extern Bit8s  g_wallclock_pos_y[81];		//ds:0x4aa0; seg004;
extern Bit8u  g_wallclock_palette_day[3][3];	//ds:0x4af1; seg004;
extern Bit8u  g_wallclock_palette_night[3][3];	//ds:0x4afa; seg004;

extern signed short g_delay_factor;		//ds:0x4b66; seg005, seg025
extern char *g_str_temp_xx_ptr;			//ds:0x4b68; seg005
extern signed char g_fig_star_colors[12];	//ds:0x4b6c; seg005
extern signed char g_fig_star_counter;		//ds:0x4b78; seg005
extern signed short g_fig_star_timer;		//ds:0x4b79; seg004, seg005
extern signed char g_fig_star_last_count;	//ds:0x4b7b; seg005
extern signed short g_fig_msg_dtps[12];		//ds:0x4b7c; seg005
extern char g_str_temp_xx[8];			//ds:0x4b95; seg005
extern signed char g_fig_star_printed;		//ds:0x4b94; seg004, seg005
extern signed short g_fight_figs_index;		//ds:0x4b9e; seg006, seg032
extern unsigned short g_random_schick_seed;	//ds:0x4ba0; seg007

extern Bit8u* g_ems_frame_ptr;			//ds:0x4baa; seg002, seg010, seg028
extern struct mouse_action g_action_table_options[10];	//ds:0x4bae; seg025

extern signed short g_tmap_x[10];		//ds:0x4c12; seg025
extern signed short g_tmap_y[10];		//ds:0x4c26; seg025
extern unsigned char g_renderbuf_in_use_flag;	//ds:0x4c3a; seg025, seg061
extern void (*g_location_handlers[19])(void);	//ds:0x4c3b; seg025

extern char *g_str_temp_xx_ptr2;		//ds:0x4c88; seg002, seg026, seg120

extern signed short g_loaded_head_id;		//ds:0x515c; seg026, seg029
extern char g_str_temp_xx2[8];			//ds:0x515e;

extern char g_chr_file_suffix[5];		//ds:0x5e3e; seg026
extern char g_savegame_suffix[5];		//ds:0x5e43; seg026
extern char g_all_files_wildcard[4];		//ds:0x5e48; seg026
extern char g_all_chr_wildcard[6];		//ds:0x5e4c; seg026
extern char g_empty_string1[1];			//ds:0x5e52; seg026
extern char g_empty_string2[1];			//ds:0x5e53; seg026
extern char g_savegame_suffix2[5];		//ds:0x5e54; seg026
extern char g_savegame_suffix3[5];		//ds:0x5e59; seg026
extern char g_all_chr_wildcard2[6];		//ds:0x5e5e; seg026
extern char g_all_chr_wildcard3[6];		//ds:0x5e64; seg026
extern Bit8u *g_pp20_buffers[9];		//ds:0x5e6a; seg027
extern signed long g_pp20_buffer_lengths[9];	//ds:0x5e8e; seg027
extern signed short g_current_fight_id;		//ds:0x5eb2; seg027
extern signed short g_locations_tab_size;	//ds:0x5eb8; seg028
extern signed short g_areadescr_dng_level;	//ds:0x5ebc; seg028
extern signed short g_areadescr_fileid;		//ds:0x5ebc; seg028
extern signed short g_areadescr_dng_flag;	//ds:0x5ebe; seg028
extern unsigned short g_ems_unused_lpage;	//ds:0x5ec0; seg028
extern signed short g_ems_unused_offset;	//ds:0x5ec2; seg028
extern signed char g_loaded_menu_icons[9];	//ds:0x5ecc; seg029
extern struct_informer_tab g_informer_tab[15];	//ds:0x5ed6; seg031
extern signed int g_fig_dropped_counter;	//ds:0x5f12; seg032, seg041
extern signed int g_fig_all_heroes_withdrawn;	//ds:0x5f14; seg032
extern signed int g_max_enemies;		//ds:0x5f16; seg032, seg080, seg117
extern const char g_str_error_on_obj_malloc[23];//ds:0x5f18; seg032
extern signed char g_finalfight_tumult;		//ds:0x5f30; seg033, seg041
extern signed char g_autofight_magic;		//ds:0x5f31; seg033, seg036
extern const char g_red_string1[5];		//ds:0x5f32; seg033
extern const char g_red_string2[5];		//ds:0x5f37; seg033
extern const char g_space_separated_strings[6];	//ds:0x5f3c; seg033
extern const char g_empty_string3[1];		//ds:0x5f42; seg033
extern const char g_empty_string4[1];		//ds:0x5f43; seg033
extern const char g_empty_string5[1];		//ds:0x5f44; seg033
extern signed short g_cb_obj_nonobstacle[27];	//ds:0x5f46; seg034, seg036, seg037

extern char g_string_14spaces[15];		//ds:0x5fab; seg034
extern signed char g_af_spell_list[11];		//ds:0x5fac; seg036

extern struct struct_point g_gfxtab_twofielded_extra_cb[4];	//ds:0x6018; seg039
extern signed char g_gfxtab_twofielded_extra_ox[4];		//ds:0x6028; seg005, seg039
extern signed char g_gfxtab_twofielded_extra_oy[4];		//ds:0x602c; seg005, seg039
extern signed char g_gfxtab_twofielded_x1[4];			//ds:0x6030; seg005, seg039, seg043
extern signed char g_gfxtab_twofielded_x2[4];			//ds:0x6034; seg005, seg039, seg043
extern signed char g_gfxtab_twofielded_extra_x1[4];		//ds:0x6038; seg005, seg039
extern signed char g_gfxtab_twofielded_extra_x2[4];		//ds:0x603c; seg005, seg039
extern struct point8s g_cb_rear_border[15];	//ds:0x6040; seg040
extern signed short g_fightobj_count;		//ds:0x605e; seg040
extern const signed short g_gfxtab_obj_offset_x[63];	//ds:0x6060; seg032, seg040
extern const signed short g_gfxtab_obj_offset_y[63];	//ds:0x6060; seg032, seg040
extern signed short g_force_weapons[9];		//ds:0x615c; seg041
extern signed short g_knive_weapons[5];		//ds:0x616e; seg041

extern char g_string_casts_spell[14];		//ds:0x6180; seg042
extern signed short g_mspell_awake_flag;	//ds:0x618e; seg043, seg102

extern const signed short g_gfxtab_shotbolt_ox[6][4];	//ds:0x6198; seg045
extern const signed short g_gfxtab_shotbolt_oy[6][4];	//ds:0x61c8; seg045
extern signed short g_gfxtab_shotbolt_nvfno[6];		//ds:0x61f8; seg045
extern signed short g_gfxtab_shotbolt_height[6];	//ds:0x6204; seg045
extern signed short g_gfxtab_shotbolt_width[6];		//ds:0x6210; seg045
extern signed char **g_anitab_shotbolt_index[6];	//ds:0x6324; seg045

extern signed short g_anitab_spell_ox[2];	//ds:0x633e; seg045
extern signed short g_anitab_spell_oy[2];	//ds:0x6342; seg045
extern signed char  g_anitab_spell_nvfno[2];	//ds:0x634c; seg045
extern signed char* g_anitab_spell_index[2];	//ds:0x6366; seg045

extern Bit8s g_statuspage_selitem1_no;		//ds:0x636e; seg048
extern Bit8s g_statuspage_selitem2_no;		//ds:0x636f; seg048
extern Bit8s g_statuspage_selitem3_no;		//ds:0x6370; seg048
extern Bit8s g_statuspage_selitem4_no;		//ds:0x6371; seg048
extern Bit8u g_statuspage_palette[32][3];	//ds:0x6372; seg046
extern const struct struct_point g_invslot_iconxy_table[23];	//ds:0x63d2; seg046

extern char g_empty_string6[1];			//ds:0x64a0; seg046
extern char g_empty_string7[1];			//ds:0x64a1; seg046
extern signed char g_hero_sel_exclude;		//ds:0x64a2; seg047, seg049, seg103, seg120
extern const struct struct_point g_invslot_borderxy_table[25];	//ds:0x64ce; seg048

extern signed short g_cheatskill_usable;	//ds:0x6532; seg048, seg103

extern char g_extraspace_separated_strings[7];	//ds:0x6534; seg048
extern char g_empty_string8[1];			//ds:0x653b; seg048
extern char g_extraspace_separated_strings2[7];	//ds:0x653c; seg048
extern char g_empty_string9[1];			//ds:0x6543; seg048
extern const signed short g_autoinc_spells_witch[3];	//ds:0x6544; seg050
extern const signed short g_autoinc_spells_druid[4];	//ds:0x654a; seg050
extern const signed short *g_autoinc_spells_mage_index[9];	//ds:0x662a; seg050
extern const signed short g_autoinc_spells_gelf[7];	//ds:0x664e; seg050
extern const signed short g_autoinc_spells_self[7];	//ds:0x665c; seg050
extern const signed short g_autoinc_spells_ielf[12];	//ds:0x666a; seg050

extern const signed short g_campfights[4];	//ds:0x6694; seg051
extern struct gather_herbs g_gather_herbs_table[13];	//ds:0x669c; seg051
extern Bit8s g_gather_herbs_special;		//ds:0x66d0; seg051, seg109, seg110, seg112, seg113, seg114, seg115, seg116
extern char g_gather_herbs_str_found[6];	//ds:0x66d1; seg051
extern char g_gather_herbs_str_comma[3];	//ds:0x66d7; seg051
extern char g_gather_herbs_str_and[6];		//ds:0x66da; seg051
extern char g_gather_herbs_str_dot[2];		//ds:0x66e0; seg051
extern const signed short g_dcampfights[4];	//ds:0x66e2; seg052
extern const healer_descr g_healer_descr_table[41];	//ds:0x66ea; seg053
extern const struct inn_descr g_inn_descr_table[77];	//ds:0x673c; seg054
extern struct shop_descr g_shop_descr_table[95];//ds:0x6870; seg055, seg065
extern char g_buy_screen_str_money_h[5];	//ds:0x6bc8; seg056

extern char g_buy_screen_str_comma_space[3];	//ds:0x6bed; seg056

extern struct smith_descr g_smith_descr_table[42];	//ds:0x6c10; seg058

extern struct inn_descr g_tavern_descr_table[89];	//ds:0x6c84; seg054, seg059, seg060

extern Bit8s *g_god_temples_index[15]; 		//ds:0x6e36; seg061
extern char g_str_temp_file_wildcard[8];	//ds:0x6e72; seg061
extern char g_str_no_save_in_temple[41];	//ds:0x6e7a; seg061

extern signed short g_passage_type_to_name[7];	//ds:0x6ec2; seg063

extern signed short g_sea_travel_tx_ship[8];	//ds:0x6ef0; seg063

extern struct sea_route g_sea_routes[46];	//ds:0x6f00; seg002, seg063, seg064 SHOULD BE IN GAME STATE
extern signed char g_travel_by_ship;		//ds:0x7070; seg049, seg063

extern char g_sea_travel_str_t[2];		//ds:0x708d; seg063
extern char g_sea_travel_str_en[3];		//ds:0x708f; seg063
extern char g_sea_travel_str_comma[3];		//ds:0x7092; seg063
extern char g_sea_travel_str_heller[10];	//ds:0x7096; seg064
extern char g_sea_travel_str_nothing[7];	//ds:0x70a0; seg064
extern struct market g_market_descr_table[34];	//ds:0x70a8; seg002, seg065, seg066
extern const Bit8u g_hyg_ani_x0[5];		//ds:0x71b8; seg065
extern const Bit8u g_hyg_ani_x1;		//ds:0x71b9; seg065
extern const Bit8u g_hyg_ani_x2;		//ds:0x71ba; seg065
extern const Bit8u g_hyg_ani_x3[3];		//ds:0x71bf; seg065
extern const Bit8u g_hyg_ani_x5;		//ds:0x71c2; seg065
extern const Bit8u g_hyg_ani_x6;		//ds:0x71c3; seg065
extern const Bit8u g_hyg_ani_x7[3];		//ds:0x71c4; seg065
extern const Bit8u g_hyg_ani_x9[3];		//ds:0x71c7; seg065
extern const Bit8u g_towns_cityindex_table[52];	//ds:0x71ca; seg066
extern Bit8u* g_daspota_locloot_index[18];	//ds:0x71fe; seg066
extern unsigned char g_mapval_to_loctype[12];	//ds:0x7246; seg066
extern signed char g_seg066_0bad_unkn0[29];	//ds:0x7252; seg066
extern signed char g_seg066_0bad_unkn1[29];	//ds:0x726f; seg066
extern signed char g_seg066_0bad_unkn2[29];	//ds:0x728c; seg066
extern signed char g_seg066_0bad_unkn3[29];	//ds:0x72a9; seg066
extern signed char g_seg066_0bad_unkn4[29];	//ds:0x72c6; seg066
extern signed char g_seg066_0bad_unkn5[29];	//ds:0x72e3; seg066
extern signed char g_seg066_0bad_unkn6[29];	//ds:0x7300; seg066
extern signed char g_seg066_0bad_unkn7[29];	//ds:0x731d; seg066
extern const struct struct_point g_visual_field_offsets_std[29];	//ds:0x733a; seg066
extern const struct struct_point g_visual_field_offsets_sign[29];	//ds:0x73ae; seg066
extern const struct struct_point g_visual_field_offsets_inn[29];	//ds:0x7422; seg066
extern const struct struct_point g_visual_field_offsets_grass[29];	//ds:0x7496; seg066
extern unsigned char g_tex_descr_table[101][18];		//ds:0x750a; seg066
extern const signed char g_visual_field_draw_order[29];		//ds:0x7c24; seg066
extern signed char g_can_merge_group;				//ds:0x7c41; seg066, seg076
extern signed char g_c_event_active;				//ds:0x7c42; seg002, seg067
extern const char g_str_bank_deposit_to_big[57];		//ds:0x7c44; seg068
extern const char g_str_obviously_closed[28];			//ds:0x7c7d; seg068
extern signed char g_tmap_double1;				//ds:0x7c9a; seg025, seg072
extern signed char g_tmap_double2;				//ds:0x7c9b; seg025, seg072, seg082
extern signed char g_swafnild_traveled;				//ds:0x7c9c; seg063, seg072
extern unsigned char g_towns_gossip_max[52];			//ds:0x7c9d; seg073
extern unsigned char g_towns_gossip_off[52];			//ds:0x7cd1; seg073
extern const char g_gossip_str_triple_wildcard[8];		//ds:0x7d06;
extern Bit8u g_palette_fight2[20][3];				//ds:0x7d0e; seg032, seg047
extern const unsigned char g_automap_bitmask[8];		//ds:0x7d4a; seg002, seg074
extern const unsigned char g_automap_tile_arrowup[49];		//ds:0x7d52; seg074
extern const unsigned char g_automap_tile_arrowright[49];	//ds:0x7d83; seg074
extern const unsigned char g_automap_tile_cross[49];		//ds:0x7d4b; seg074
extern signed short g_automap_selx;				//ds:0x7de5; seg074, seg099
extern signed short g_automap_sely;				//ds:0x7de7; seg074, seg099

extern unsigned char g_dng_stub3_unkn0[22];			//ds:0x907a; seg075
extern unsigned char g_dng_stub3_unkn1[22];			//ds:0x9090; seg075
extern unsigned char g_dng_stub3_unkn2[22];			//ds:0x90a6; seg075
extern unsigned char g_dng_stub3_unkn3[22];			//ds:0x90bc; seg075
extern unsigned char g_dng_stub3_unkn4[22];			//ds:0x90d2; seg075
extern unsigned char g_dng_stub3_unkn5[22];			//ds:0x90e8; seg075
extern unsigned char g_dng_stub3_unkn6[22];			//ds:0x90fe; seg075
extern unsigned char g_dng_stub3_unkn7[22];			//ds:0x9114; seg075
extern unsigned char g_dng_stub3_unkn8[22];			//ds:0x912a; seg075
extern unsigned char g_dng_stub3_unkn9[22];			//ds:0x9140; seg075
extern struct struct_point g_dng_stub4_unkn0[22];		//ds:0x9156; seg075
extern struct struct_point g_dng_stub4_unkn1[22];		//ds:0x91ae; seg075
extern struct struct_point g_dng_stub4_unkn2[22];		//ds:0x9206; seg075
extern struct struct_point g_dng_stub4_unkn3[22];		//ds:0x925e; seg075
extern signed char g_dng_stub4_unkn4[22];			//ds:0x92b6; seg075
extern char g_dng_stub6_str_comma[3];			//ds:0x92cc; seg075
extern char g_dng_stub6_str_and[6];			//ds:0x92cf; seg075
extern signed short (*g_dng_handlers[15])(void);	//ds:0x92d6; seg076
extern signed short g_dng_level_changed;	//ds:0x9312; seg075, seg076

extern const char g_dng01_str_marbo[6];		//ds:0x93d7; seg077
extern const char g_dng03_str_spinnennetz[12];	//ds:0x95a6; seg079
extern const char g_dng03_str_mactans[8];	//ds:0x95b2; seg079
extern Bit16s g_dng05_trash_flag;		//ds:0x960e; seg080 SHOULD BE IN GAME STATE

extern const char g_str_s_war[8];		//ds:0x964f; seg081
extern const char g_str_s_und_s_waren[17];	//ds:0x9657; seg081
extern const char g_dng08_str_tairach[8];	//ds:0x9725; seg083
extern signed int g_dng12_obstacle_tries;	//ds:0x9d43; seg090 SHOULD BE IN GAME STATE
extern signed int g_dng12_watertrap_bak;	//ds:0x9d45; seg090 SHOULD BE IN GAME STATE
extern struct struct_chest *g_dng_specialchest_index[15];	//ds:0x9d57; seg092
extern const char g_str_single_space[2];	//ds:0x9dc4; seg092

extern struct struct_land_route g_land_routes[59];	//ds:0x9dc6; seg093, seg094

extern struct trv_start_point g_signposts[106];			//ds:0xa0b4; seg093, se094
extern struct trv_start_point g_harbors[26];			//ds:0xa3a3; seg064
extern struct struct_point g_town_positions[52];		//ds:0xa43f; seg002, seg063, seg094, seg094
extern struct mouse_action g_action_table_travelmap[35];	//ds:0xa50f; seg093

extern struct struct_tevent g_tevents_tab[156]; //ds:0xa66d; seg094
extern signed char g_traveling;				//ds:0xa842; seg002, seg032, seg063, seg094
extern const signed char g_tevents_repeatable[145];	//ds:0xa843; seg094
extern char g_grammar_article_der[4];		//ds:0xa8d4; seg096
extern char g_grammar_article_die[4];		//ds:0xa8d8; seg096
extern char g_grammar_article_des[4];		//ds:0xa8e0; seg096
extern char g_grammar_article_den[4];		//ds:0xa8e4; seg096
extern char g_grammar_article_dem[4];		//ds:0xa8e8; seg096

extern char *g_grammar_articles_index[15];	//ds:0xa917; seg096
extern signed short g_grammar_def_table[24];	//ds:0xa953; seg096
extern signed short g_grammar_indef_table[24];	//ds:0xa983; seg096
extern signed short g_grammar_noarticle_table[24];	//ds:0xa9b3; seg096

extern char *g_str_s_s_ptr;			//ds:0xabe3; seg096
extern char *g_str_von_s_s_ptr;			//ds:0xabe7; seg096
extern signed short g_grammar_buf_no;		//ds:0xa9eb; seg096

extern char g_grammar_pronouns_er[3];		//ds:0xa9f3; seg096
extern char g_grammar_pronouns_sie[4];		//ds:0xa9f6; seg096
extern char g_grammar_pronouns_sein[5];		//ds:0xa9fd; seg096
extern char g_grammar_pronouns_ihr[4];		//ds:0xaa02; seg096
extern char g_grammar_pronouns_ihn[4];		//ds:0xaa06; seg096
extern char g_grammar_pronouns_ihm[4];		//ds:0xaa0a; seg096

extern char *g_grammar_pronouns_index[7];		//ds:0xaa14; seg096
extern const signed char g_grammar_pronouns_table2[33];	//ds:0xaa30; seg096
extern struct struct_char_width g_gui_char_width[75];	//ds:0xaa51; seg096
extern struct struct_char_height g_gui_char_height[67];	//ds:0xab42; seg097

extern signed short g_gui_bool_flag;		// ds:0xac0b; seg097
extern signed short g_spell_special_aecost;	// ds:0xac0e; seg098, seg099, seg100, seg101

extern unsigned char *g_hexenknoten_gfx_buf;	// ds:0xacc8; seg100
extern signed short g_monster_spell_ae_cost;	// ds:0xaccc; seg102
extern char g_select_skill_lvlup[6]; 		// ds:0xacce; seg103
extern signed char g_select_skill_defaults[6];	// ds:0xacd4; seg103
extern struct_recipe g_alchemy_recipes[13];	//ds:0xacda; seg104
extern signed char g_prevent_drop_equipped_items;	// ds:0xae46; seg106
extern signed short g_hero_startup_items[12][4];	// ds:0xae48; seg106
extern signed short g_hero_startup_items_all[4];	// ds:0xaea8; seg106
extern void (*g_use_special_item_handlers[14])(void);	//ds:0xaeb0; seg107
extern int g_light_type;			// ds:0xaee8; seg101, seg107
extern void (*g_travel_event_handlers[146])(void);	//ds:0xaeea; seg109

extern signed int g_tlk_ruin_hero_counter;	//ds:0xb21b; seg117
/* BSS starts at 0xbc38, variables in reverse/decreasing order */

extern signed char g_large_buf;		// ds:0xe5e4; seg120, seg028
extern HugePt g_global_buffer_ptr;	// ds:0xe5e0; seg120
extern Bit32u g_buffersize;		// ds:0xe5dc; seg120
extern unsigned short g_sample_ad_length;	// ds:0xbc5a; seg002
extern signed short g_basepos_x_bak;	// ds:0xe5d9; seg117
extern signed short g_basepos_y_bak;	// ds:0xe5d7; seg117
extern signed short g_textbox_width_bak; // ds:0xe5d5; seg117
extern signed short g_wallclock_update_bak; // ds:0xe5d3; seg117
extern unsigned char g_event_ani_busy;	// ds:0xe5d2; seg002, seg109-seg118

extern unsigned char *g_itemuser;	// ds:0xe5ce; seg107
extern signed int g_used_item_pos;	// ds:0xe5cc; seg107
extern signed int g_used_item_id;	// ds:0xe5ca; seg107
extern unsigned char *g_used_item_desc;	// ds:0xe5c6; seg107
extern signed int g_alchemy_missing_item;	// ds:0xe5c4; seg104
extern struct enemy_sheet *g_spelluser_e;	// ds:0xe5c0; seg102
extern unsigned char *g_spelluser;	// ds:0xe5bc; seg068, seg098, seg102, seg107
extern unsigned char *g_spelltarget;	// ds:0xe5b8; seg098-seg102
extern struct enemy_sheet *g_spelltarget_e;	// ds:0xe5b4; seg098-seg102
extern signed int g_spelltest_result;	// ds:0xe5b2; seg098, seg099, seg102
extern signed int g_menu_selected;	// ds:0xe5b0; seg002, seg097
extern signed int g_menu_input_busy;	// ds:0xe5ae; seg002, seg097
extern signed int g_menu_default_select;	// ds:0xe5ac; seg025, seg074, seg097
extern char g_grammar_bufs[5][40];	// ds:0xe4e3; seg096
extern signed int g_dialogbox_indent_width;	// ds:0xe4db; seg096, seg097
extern signed int g_dialogbox_indent_height;	// ds:0xe4d9; seg096, seg097
extern char g_gui_print_char;			// ds:0xe4d8; seg096
extern unsigned char g_route_tevent_flags[15];	// ds:0xe4c9; seg094
extern unsigned char g_good_camp_place;		// ds:0xe4c8; seg051, seg094, seg109
extern unsigned char g_trv_detour_pixel_bak[20];	// ds:0xe4b4; seg094
extern signed int g_current_town_anix;		// ds:0xe4b1; seg002, seg093, seg094
extern signed int g_current_town_aniy;		// ds:0xe4af; seg002, seg093, seg094
extern signed int g_selected_town_anix;		// ds:0xe4ad; seg002, seg093, seg094
extern signed int g_selected_town_aniy;		// ds:0xe4ab; seg002, seg093, seg094
extern signed int g_current_town_overx;		// ds:0xe4a9; seg002
extern signed int g_current_town_overy;		// ds:0xe4a7; seg002
extern signed int g_current_town_over;		// ds:0xe4a5; seg002, seg093, seg094
extern signed int g_trv_menu_selection;		// ds:0xe4a3; seg002, seg093, seg094
extern unsigned char g_route59_flag;		// ds:0xe4a2; seg093, seg094, seg110
extern signed int g_get_extra_loot;		// ds:0xe4a0; seg076, seg077, seg092
extern unsigned char *g_dungeon_doors_buf; 	// ds:0xe49c; seg028,seg076,seg098,seg120
extern unsigned char *g_dungeon_stairs_buf; 	// ds:0xe498; seg028,seg076,seg098,seg120
extern unsigned char *g_dungeon_fights_buf; 	// ds:0xe494; seg028,seg076,seg098,seg120
extern signed short g_lockpick_try_counter;	// ds:0xe492; seg076
extern unsigned char *g_dng_gfxtab;		// ds:0xe48d; seg075, seg076
extern signed char g_dng_floor_tex;		// ds:0xe48c; seg075
extern unsigned char *g_dng_map_ptr;		// ds:0xe488; seg076, seg098, seg099
extern signed short g_dng_refresh_x_target;	// ds:0xe486; seg075, seg076
extern signed short g_dng_refresh_y_target;	// ds:0xe484; seg075, seg076
extern signed short g_dng_refresh_direction;	// ds:0xe482; seg075, seg076, seg082, seg088
extern unsigned char g_automap_buf[64];		// ds:0xe442; seg002, seg028, seg074
extern char g_str_beorn_hjall[20];		// ds:0xe42e; seg072
extern signed short g_combo_mode;		// ds:0xe42c; seg054, seg059, seg069
extern unsigned char *g_tex_floor[6];		// ds:0xe414; seg028, seg066
extern signed short g_entrance_angle;		// ds:0xe412; seg066
extern signed short g_always_zero2;		// ds:0xe410; seg066
extern signed short g_always_zero1;		// ds:0xe40e; seg066
extern signed short g_city_refresh_x_target;	// ds:0xe40c; seg066
extern signed short g_city_refresh_y_target;	// ds:0xe40a; seg066
extern signed short g_city_refresh_direction;	// ds:0xe408; seg066
extern unsigned char *g_buffer11_ptr;		// ds:0xe404; seg028, seg075
extern unsigned char g_city_house_count[4];	// ds:0xe400; seg028, seg066
extern HugePt g_buffer9_ptr4;			// ds:0xe3fc; seg028
extern signed char g_sea_travel_sleep_quality;	// ds:0xe3fa; seg063
extern signed short g_temple_god;		// ds:0xe3f8; seg061, seg062
extern signed short g_price_modificator;	// ds:0xe3f6; seg055, seg057, seg058
extern struct struct_shopping_cart *g_buy_shopping_cart;	//ds:0xe3f2; seg056
extern signed char g_sleep_quality;		// ds:0xe3f1; seg054, seg104
extern signed char g_booked_inn_days;		// ds:0xe3f0; seg054
extern signed char g_citycamp_guardstatus[8];	// ds:0xe3e8; seg052
extern signed char g_citycamp_magicstatus[7];	// ds:0xe3e1; seg052
extern signed char g_citycamp_guards[3];	// ds:0xe3de; seg052
extern signed char g_wildcamp_guardstatus[8];	// ds:0xe3d6; seg049, seg051, seg109
extern signed char g_wildcamp_magicstatus[7];	// ds:0xe3cf; seg049, seg051, seg109
extern signed char g_wildcamp_replstatus[7];	// ds:0xe3c8; seg052
extern signed char g_wildcamp_herbstatus[7];	// ds:0xe3c1; seg052
extern signed char g_wildcamp_guards[3];	// ds:0xe3be; seg052
extern unsigned char *g_skills_buffer;		// ds:0xe3ba; seg050
extern unsigned char *g_inc_skills_counter;	// ds:0xe3b6; seg050
extern unsigned char *g_inc_spells_counter;	// ds:0xe3b2; seg050
extern unsigned char *g_current_cursor_bak;	// ds:0xe3ae; seg048
extern signed short g_attacker_attacks_again;	// ds:0xe3ac; seg042, seg043, seg044
extern signed short g_defender_attacks;		// ds:0xe3aa; seg042, seg043, seg044
extern signed short g_attacker_dead;		// ds:0xe3a8; seg042, seg043, seg044
extern signed short g_defender_dead;		// ds:0xe3a6; seg042, seg043, seg044, seg098, seg102
extern signed short g_spell_illusionen;		// ds:0xe3a4; seg042, seg043, seg099
extern signed char g_fig_cb_selector_id[21];	// ds:0xe38f; seg032, seg034, seg040
extern signed char g_fig_cb_marker_id;		// ds:0xe38e; seg032, seg033, seg036, seg037, seg038, seg040
extern signed char g_fig_shot_bolt_id;		// ds:0xe38d; seg040, seg042, seg043, seg045
extern signed char g_fig_spellgfx_id;		// ds:0xe38c; seg040, seg042, seg043, seg045
extern unsigned char **g_figobj_gfxbuf_table;	// ds:0xe388; seg032, seg040
extern signed short *g_figobj_gfxheight_table;	// ds:0xe384; seg032, seg040
extern signed short *g_figobj_gfxwidth_table;	// ds:0xe380; seg032, seg040
extern struct struct_fighter *g_fig_list_buffer;	// ds:0xe37c; seg006, seg040
extern unsigned char *g_buffer_anidat;		// ds:0xe378; seg036, seg037, seg040, seg044
extern unsigned char *g_buffer_weapanidat;	// ds:0xe374; seg036, seg037, seg040, seg044
extern Bit32s g_fightobj_buf_freespace;		// ds:0xe370; seg032-seg100
extern signed char g_fig_twofielded_count;	// ds:0xe356; seg039, seg040
extern signed char g_fig_twofielded_table[21];	// ds:0xe35a; seg005-seg099
extern signed char *g_chessboard_cpy;		// ds:0xe356; seg038
extern signed int g_fig_dropped_weapons[30];	// ds:0xe31a; seg032, seg041
extern signed int g_autofight;		// ds:0xe318; seg004-seg105
extern signed int g_current_fight_no;	// ds:0xe316; seg002-seg042
extern signed int g_tlk_id;		// ds:0xe314; seg030, seg031
extern signed int g_dialog_state;	// ds:0xe312; seg030, seg031, seg060
extern signed int g_dialog_done;	// ds:0xe310; seg030, seg031, seg060
extern signed int g_dialog_next_state;	// ds:0xe30e; seg030-seg117
extern signed int g_dialog_informer;	// ds:0xe30c; seg030, seg031
extern char *g_dialog_title;		// ds:0xe308; seg030, seg072

extern char g_savegame_names[5][9];	// ds:0xe2da; seg026, seg027
extern time_t g_last_save_time;		// ds:0xe2d6; seg026, seg059
extern unsigned char *g_saved_files_buf;// ds:0xe2d2; seg026;
extern signed short g_delay_timer;	// ds:0xe2d0; seg004, seg005
extern signed short g_figobj_unkn_x2_bak;	// ds:0xe2ce; seg005
extern signed short g_figobj_unkn_x2;		// ds:0xe2cc; seg005
extern signed short g_figobj_unkn_y2_bak;	// ds:0xe2ca; seg005
extern signed short g_figobj_unkn_y2;		// ds:0xe2c8; seg005
extern signed short g_figobj_unkn_x1_bak;	// ds:0xe2c6; seg005
extern signed short g_figobj_unkn_x1;		// ds:0xe2c4; seg005
extern signed short g_figobj_unkn_y1_bak;	// ds:0xe2c2; seg005
extern signed short g_figobj_unkn_y1;		// ds:0xe2c0; seg005
extern struct struct_msg g_fig_target_grammar;	// ds:0xe2be; seg005, seg042, seg043
extern struct struct_msg g_fig_actor_grammar;	// ds:0xe2ba; seg005, seg042, seg043
extern signed short g_fig_ani_state[8];		// ds:0xe2a8; seg005
extern signed short g_fig_figlist_readd[8];	// ds:0xe298; seg005
extern unsigned char* g_fig_gfxbuffers[8];	// ds:0xe278; seg005
#if defined(__BORLANDC__)
extern void interrupt far(*g_bc_timer)(...);		// ds:0xe274; seg004
#endif
extern signed short g_ani_area_timeout[10];	// ds:0xe260; seg004
extern signed short g_ani_area_status[10];	// ds:0xe24c; seg004
extern signed short g_ani_change_dir[10];	// ds:0xe238; seg004
extern Bit32s g_gfx_spinlock;		// ds:0xe234; seg004
extern char **g_itemsname;		// ds:0xe22f; seg026, seg120
extern unsigned char *g_itemsdat;	// ds:0xe22b; seg002, seg027, seg105, seg107, seg120
extern signed char g_market_itemsaldo_table[254]; // ds:0xe12d; seg056, seg057, seg120
extern char **g_monnames_index;		// ds:0xe129; seg026, seg120
extern unsigned char *g_monster_dat_buf;// ds:0xe125; seg032, seg039
extern struct struct_memslot_ani *g_memslots_anis;	// ds:0xe121; seg027, seg120
extern struct struct_memslot_fig *g_memslots_mfig;	// ds:0xe11d; seg027, seg120
extern struct struct_memslot_fig *g_memslots_wfig;	// ds:0xe119; seg027, seg120
extern struct struct_memslot_fig *g_memslots_mon;	// ds:0xe115; seg027, seg120
extern signed short g_wallclock_update;	// ds:0xe113; seg004-seg117, seg120
extern signed short g_wallclock_x;	// ds:0xe111; seg004, seg029, seg063, seg094
extern signed short g_wallclock_y;	// ds:0xe10f; seg004, seg029, seg063, seg094
extern signed short g_wallclock_redraw;	// ds:0xe10e; seg004, seg029
extern signed char  g_location_market_flag;	// ds:0xe10c; seg066
extern struct struct_fighter *g_fig_list_head;	// ds:0xe108; seg005, seg006, seg040
extern signed char g_fig_list_array[127];	// ds:0xe089; seg006, seg040
extern struct struct_fighter g_fig_list_elem;	// ds:0xe066; seg006, seg034, seg038, seg039, seg040, seg042, seg043, seg045, seg100

extern signed char g_fightobj_list[90];	// ds:0xd874; seg040
extern signed int g_nr_of_enemies;	// ds:0xd872; seg032, seg034, seg039, seg100
extern unsigned char *g_fightobj_buf_seek_ptr; // ds:0xd86e; seg005-seg100
extern Bit8u* g_weapons_nvf_buf;	// ds:0xd86a; seg005, seg032, seg040
extern Bit8u* g_spellobj_nvf_buf;	// ds:0xd868; seg005, seg032, seg040, seg045, seg100
extern Bit8u* g_fig_cb_marker_buf;	// ds:0xd862; seg038, seg040
extern Bit8u* g_fig_cb_selector_buf;	// ds:0xd85e; seg034, seg040
extern Bit8u* g_fig_shot_bolt_buf;	// ds:0xd85a; seg040, seg045
extern Bit8u* g_fig_spellgfx_buf;	// ds:0xd856; seg040, seg045
extern signed char *g_chessboard;	// ds:0xd852;
extern signed char g_hero_is_target[7];	// ds:0xd84b; seg032, seg042, seg043, seg044
extern signed char g_fig_actors_unkn[30];	// ds:0xd82d; seg032, seg042, seg043, seg044
extern signed char g_fig_move_pathdir[10];	// ds:0xd823; seg034, seg036, seg037, seg038
extern struct enemy_sheet g_enemy_sheets[20];	// ds:0xd34b; seg005, seg006, seg032-seg102

extern signed short g_replenish_stocks_mod;	// ds:0xd331; seg051, seg109
extern signed short g_gather_herbs_mod;		// ds:0xd32f; seg051, seg109
extern signed short g_wildcamp_sleep_quality;	// ds:0xd32d; seg051, seg094, seg109, seg114, seg118
extern signed short g_fig_flee_position[4];	// ds:0xd325; seg005, seg076-seg089
extern unsigned char *g_townpal_buf;		// ds:0xd312; seg002, seg027, seg120
extern signed short g_txt_tabpos[7];		// ds:0xd313; seg024, seg029, seg33, seg046, seg096, seg097
#if defined (__BORLANDC__)
extern signed short g_video_mode_bak;		// ds:0xd30d; seg002
extern signed short g_video_page_bak;		// ds:0xd30b; seg002, seg004
extern signed short g_gameinit_flag;		// ds:0xd309; seg002
#endif
extern unsigned char *g_renderbuf_ptr;	// ds:0xd303; seg002-seg120
extern unsigned char *g_vga_memstart;	// ds:0xd2ff; seg002-seg120 aka FRAMEBUF_PTR
extern unsigned char *g_vga_backbuffer;	// ds:0xd2fb; seg002-seg120 aka PRINT_STRING_BUFFER
extern unsigned char *g_icon;		// ds:0xd2f7; seg002-seg058
extern char *g_dtp2;			// ds:0xd2f3: seg002-seg121
extern char *g_text_input_buf;		// ds:0xd2ef: seg002-seg120
extern char *g_text_output_buf;		// ds:0xd2eb: seg002-seg120
extern unsigned char *g_buf_icon;	// ds:0xd2e7; seg029, seg120
extern unsigned char *g_objects_nvf_buf;// ds:0xd2e3; seg004, seg032, seg040, seg120
extern unsigned char *g_fig_figure1_buf;// ds:0xd2df; seg027, seg055, seg056, seg057, seg058, seg120
extern unsigned char *g_fig_figure2_buf;// ds:0xd2db; seg027, seg040, seg120
extern signed short g_textline_posx;	// ds:0xd2d9; seg024, seg025, seg029, seg096, seg097
extern signed short g_textline_posy;	// ds:0xd2d7; seg024, seg025, seg029, seg096, seg097
extern signed short g_textline_maxlen;	// ds:0xd2d5; seg024, seg025, seg029, seg096, seg097
extern signed short g_textline_unknown;	// ds:0xd2d3; seg029
extern signed short g_gui_text_centered;// ds:0xd2d1; seg002, seg096, seg097
extern signed short g_textcolor_fg[4];	// ds:0xd2c9; seg096, seg120
extern signed short g_textcolor_bg;	// ds:0xd2c7; seg096
extern signed short g_textcolor_index;	// ds:0xd2c5; seg096, seg097
extern unsigned char *g_buf_font6;	// ds:0xd2c1; seg026, seg096, seg120
extern unsigned char *g_splash_buffer;	// ds:0xd2bd; seg002, seg028, seg097, seg120
extern char *g_text_ltx_buffer;		// ds:0xd2b9; seg026, seg120
extern char *g_buffer7_ptr;		// ds:0xd2b5; seg026, seg028, seg066, seg120
extern unsigned char *g_buffer6_ptr;	// ds:0xd2b1; seg002, seg027, seg063, seg093, seg094, seg120
extern unsigned char *g_popup;		// ds:0xd2ad; seg027, seg096, seg120
extern unsigned char *g_buffer10_ptr;	// ds:0xd2a9; seg026-seg120
extern char *g_buffer5_ptr;		// ds:0xd2a5; seg026, seg120
extern char *g_monnames_buffer;		// ds:0xd2a1; seg026, seg120
extern unsigned char *g_fig_star_gfx;	// ds:0xd29d; seg005, seg040
extern unsigned char *g_trv_track_pixel_bak;	// ds:0xd299; seg063, seg094, seg120
extern Bit32s g_buffer_anis_tab[37];	// ds:0xd205; seg027
extern Bit32u g_buffer_mfigs_tab[43];	// ds:0xd159; seg027
extern Bit32u g_buffer_wfigs_tab[43];	// ds:0xd0ad; seg027
extern Bit32u g_buffer_monster_tab[36];	// ds:0xd01d; seg027
extern HugePt g_buffer9_ptr3;		// ds:0xd019; seg003-seg120
extern unsigned char *g_buffer9_ptr2;	// ds:0xd015; seg120
extern signed short g_redraw_menuicons;	// ds:0xd013; seg066, seg076, seg099
extern signed short g_dng_extra_action;	// ds:0xd011; seg076, seg099
extern signed short g_dng_init_flag;	// ds:0xd00f; seg076,
extern unsigned char g_mouse_bg_bak[256];	// ds:0xcf0f; seg004
extern unsigned short g_ggst_cursor[16];	// ds:0xcecf; seg002, seg048
extern unsigned short g_ggst_mask[16];		// ds:0xceef; seg002
extern unsigned short *g_current_cursor;	// ds:0xcecb; seg002, seg004, seg024, seg25, seg048
extern unsigned short *g_last_cursor;		// ds:0xcec7; seg002
extern unsigned char g_gui_text_buffer[64];	// ds:0xce87; seg096
extern Bit32s g_ani_unknown4;		// ds:0xce43; seg027, seg120
extern signed int g_ani_posx;		// ds:0xce41; seg004, seg029, seg066, seg074, seg075
extern signed int g_ani_posy;		// ds:0xce41; seg004, seg029, seg066, seg074, seg075
extern HugePt g_ani_palette;		// ds:0xce3b; seg004, seg025, seg027
extern unsigned char g_ani_palette_size;	// ds:0xce3a; seg027
extern unsigned char g_ani_compr_flag;		// ds:0xce39; seg027
extern unsigned char *g_ani_main_ptr;	// ds:0xce35; seg004, seg027

extern signed char g_ani_areacount;	// ds:0xc3ee; seg004, seg027
extern unsigned char g_ani_height;	// ds:0xc3ed; seg004, seg027
extern unsigned short g_ani_unknown1;	// ds:0xc3eb; seg027
extern unsigned short g_ani_unknown2;	// ds:0xc3e9; seg027
extern signed short g_ani_width;	// ds:0xc3e7; seg004
extern HugePt g_buffer9_ptr;		// ds:0xc3db; seg004-seg120
extern signed short g_action;		// ds:0xc3d9; seg002, seg025-seg097
extern signed short g_bioskey_event;	// ds:0xc3d7; seg002, seg097
extern signed short g_mouse1_event2;	// ds:0xc3d5; seg002, seg004, seg025, seg049, seg066, seg097
extern signed short g_mouse2_event;	// ds:0xc3d3; seg002, seg025-seg097
extern signed short g_mouse1_event1;	// ds:0xc3d1; seg002, seg034, seg093, seg097
extern signed short g_mouse1_doubleclick;	// ds:0xc3cf; seg002, seg048
extern signed short g_update_statusline;	// ds:0xc3cb; seg002, seg004, seg026, seg029, seg046, seg097
extern signed short g_unused_spinlock_flag;	// ds:0xc3cb; seg002, seg004
extern signed short g_have_mouse;	// ds:0xc3c7; seg002-seg058
extern signed short g_bioskey_event10;	// ds:0xc3c5; seg002, seg097
extern signed short g_game_state;	// ds:0xc3c1; seg002-seg116
extern signed short g_random_schick_seed2;	// ds:0xc3bf; seg004, seg007
extern char **g_text_ltx_index;		// ds:0xc3b5;
extern char **g_tx_index;		// ds:0xc3b1; seg026, seg028, seg031, seg064, seg120
extern char **g_tx2_index;		// ds:0xc3ad; seg
extern unsigned char *g_buffer8_ptr;	// ds:0xc3a9; seg005, seg026, seg028, seg032, seg042, seg050, seg061, seg098, seg120
extern struct location g_locations_tab[150];	//ds:0xc025; seg028, seg64, seg066, seg74, seg094
extern struct_pic_copy g_pic_copy;	// ds:0xc00d; seg002-seg120

extern Bit8u *g_buyitems;		// ds:0xc009; seg055, seg056
extern Bit8u *g_sellitems;		// ds:0xc005; seg056, seg057, seg058
extern signed short g_game_mode;	// ds:0xc003; seg002, seg046, seg048, seg050, seg097, seg098, seg103, seg120
extern signed short g_textbox_pos_y;	// ds:0xc001; seg096, seg097
extern signed short g_textbox_pos_x;	// ds:0xbfff; seg096, seg097
extern signed short g_textbox_width;	// ds:0xbffd; seg002-seg120
extern unsigned char *g_gui_buffer_unkn;// ds:0xbff9; seg025, seg032, seg097, seg120
extern char *g_radio_name_list[25];	// ds:0xbf95; seg033, seg047, seg048, seg098, seg103, seg104, seg105
extern unsigned char g_dng_map[512];	// ds:0xbd95; seg002-seg107
extern unsigned char g_dng_map_size;	// ds:0xbd94; seg002, seg003, seg028, seg066, seg074, seg100
extern signed short g_ems_unused_handle;	// ds:0xbd92; seg028, seg120
extern signed short g_ems_travelmap_handle;	// ds:0xbd90; seg028, seg120
extern Bit8u *g_ems_unused_tab;		// ds:0xbd8c; seg028
extern unsigned char g_visual_field_vals[29];	// ds:0xbd6e; seg003, seg066,seg075, seg077, seg080
extern signed char g_visual_fields_tex[29];	// ds:0xbd50; seg066, seg075
extern signed char g_direction_unkn;		// ds:0xbd4f; seg003
extern unsigned char g_steptarget_back;		// ds:0xbd4e; seg003, seg066, seg076
extern unsigned char g_steptarget_front;	// ds:0xbd4d; seg003, seg066, seg076, seg099
extern signed char g_new_menu_icons[9];		// ds:0xbd38; seg002, seg026, seg066, seg076, seg099
extern unsigned char *g_heroes;		// ds:0xbd34
extern unsigned char *g_fightobj_buf;	// ds:0xbd30; seg032, seg040
extern signed char *g_scenario_buf;	// ds:0xbd2c; seg027, seg032, seg034, seg038, seg040
extern struct fight *g_current_fight;	// ds:0xbd28; seg027, seg032, seg043, seg035, seg039, seg040, seg100
extern signed char g_citycamp_city;	// ds:0xbd27; seg052, seg066, seg076
extern signed short g_pregame_state;	// ds:0xbd25; seg002, seg097
#if defined(__BORLANDC__)
extern signed short g_ail_music_driver_id;	// ds:0xbd23; seg002
extern signed short g_ail_sequence;		// ds:0xbd21; seg002
extern Bit8u *g_ail_music_driver_descr;		// ds:0xbd1d; seg002
extern Bit8u *g_ail_music_driver_buf;		// ds:0xbd19; seg002
extern Bit8u *g_ail_state_table;		// ds:0xbd15; seg002
extern Bit8u *g_ail_timbre_cache;		// ds:0xbd11; seg002
extern Bit8u *g_ail_midi_buffer;		// ds:0xbd0d; seg002
extern Bit8u *g_ail_music_driver_buf2;		// ds:0xbd09; seg002
extern signed long g_ail_state_table_size;	// ds:0xbd05; seg002
extern unsigned short g_ail_timbre_cache_size;	// ds:0xbd03; seg002
extern signed short g_sample_ad_handle;		// ds:0xbd01; seg002
extern signed short g_load_sound_driver;	// ds:0xbcff; seg002
extern signed short g_use_cdaudio_flag;		// ds:0xbcfd; seg002
extern signed short g_ail_digi_driver_id;	// ds:0xbcfb; seg002
extern Bit8u *g_ail_digi_driver_descr;		// ds:0xbcf7; seg002
extern Bit8u *g_ail_digi_driver_buf;		// ds:0xbcf7; seg002
extern Bit8u *g_ail_voc_buffer;			// ds:0xbcef; seg002
extern Bit8u *g_ail_digi_driver_buf2;		// ds:0xbceb; seg002
#endif
extern Bit32s g_archive_file_length;	// ds:0xbce7; seg002
extern Bit32s g_archive_file_remaining;	// ds:0xbce3; seg002
extern Bit32s g_archive_file_offset;	// ds:0xbcfd; seg002
#if defined(__BORLANDC__)
extern void interrupt far (*g_mouse_handler_bak)(...);	// ds:0xbcbd;
#endif
extern signed char g_freeze_timers;	// ds:0xbcda; seg002, seg004
extern signed short g_map_townmark_state;	// ds:0xbcd8; seg002
extern signed short g_spinlock_flag;	// ds:0xbcd6; seg002
extern Bit8u  g_hero_splash_timer[7];	// ds:0xbccf; seg002
extern unsigned char *g_splash_le;	// ds:0xbccb; seg002, seg028
extern unsigned char *g_splash_ae;	// ds:0xbcc7; seg002, seg028
extern unsigned char g_gfx_wait_keypress[100];	// ds:0xbc63; seg002
extern unsigned char g_playmask_us;	// ds:0xbc62; seg002, seg029

extern signed char g_travel_event_active;		// ds:0xb132; seg002, seg109, seg117
extern signed short g_travel_event_tx2;			// ds:0xb133; seg109
extern Bit8u g_travel_event_axes[6];			// ds:0xb135; seg109, seg118
extern char g_empty_string10[1];			// ds:0xb13b; seg109
extern char g_empty_string11[1];			// ds:0xb13c; seg109
extern char g_empty_string12[1];			// ds:0xb13d; seg109
extern struct struct_chest g_tevent014_corpse;		// ds:0xb13e; seg110
extern struct struct_chest g_tevent064_chest_descr;	// ds:0xb154; seg111
extern Bit8u g_tevent067_chest[10];			// ds:0xb16a; seg112
extern unsigned char g_tevent135_climb_damage[5][2];	// ds:0xb174; seg116
extern unsigned char g_random_encounter_index[59];	// ds:0xb17e; seg117
extern const Bit8u g_random_encounter_descr[14][7];	// ds:0xb1b9; seg117
extern char g_str_rad1[4];			// ds:0xb21e; seg118
extern char g_str_rad2[4];			// ds:0xb222; seg118
extern char g_str_rad3[4];			// ds:0xb226; seg118
extern Bit8u g_color_white[3];			// ds:0xb22a; seg120
extern Bit8u g_color_black[3];			// ds:0xb22d; seg029, seg120
extern Bit8u g_palette_unknown2[8][3];		// ds:0xb230; seg120
extern Bit8u g_palette_unknown3[3][3];		// ds:0xb248; seg120
extern Bit8u g_palette_unknown4[32][3];		// ds:0xb251; seg120
extern Bit8u g_palette_general[32][3];		// ds:0xb2b1; seg120
extern char g_str_backslash_temp[6];		// ds:0xb311; seg120
extern char g_str_not_enough_mem[124];		// ds:0xb317; seg120
extern char g_str_drive_x[4];			// ds:0xb393; seg120
extern char g_str_temp_dir_fail[280];		// ds:0xb397; seg120
extern char g_all_files_wildcard2[4];		// ds:0xb4af; seg120
extern char g_all_chr_wildcard4[6];		// ds:0xb4b3; seg120
extern char g_all_files_wildcard3[4];		// ds:0xb4b9; seg120
extern char g_str_gen_exe[8];			// ds:0xb4bd; seg120
extern char g_str_gen_exe2[8];			// ds:0xb4c5; seg120
extern const char g_str_gen_b[2];		// ds:0xb4cd; seg120
extern const char g_str_gen_a[2];		// ds:0xb4cf; seg120
extern const char g_str_gen_n[2];		// ds:0xb4d1; seg120
extern const char g_str_gen_1[2];		// ds:0xb4d3; seg120
extern const char g_str_gen_generation[11];	// ds:0xb4d5; seg120

#if !defined(__BORLANDC__)
/* arrays for meaningful log messages */
const char* names_attrib[14];
const char* names_skill[52];
const char* names_spell[86];
const char* names_mspell[14];
#endif

#endif
