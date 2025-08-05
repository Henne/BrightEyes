#if !defined V302DE_DATSEG_H
#define V302DE_DATSEG_H

#include <time.h>

#if defined(__BORLANDC__)
extern char ds[1];
#endif

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

struct struct_point {
	int x;
	int y;
};

struct struct_rect {
	int y1;
	int x1;
	int y2;
	int x2;
};

struct struct_smith_repairitems {
	Bit16s item_id;
	Bit32s pickup_time;
};

struct struct_informer_tab {
	Bit16s name_id;
	Bit8s town;
	Bit8s unkn;
};

/* seg001 */
extern unsigned short g_cd_init_successful;
extern Bit32u g_cd_skipmagic;

/* seg096 */
extern signed short g_items_noplural[23];
/* seg106 + seg107 */
extern signed short g_items_pluralwords[7];
/* seg096 */
extern signed char g_items_genders[254];
/* seg048, seg056, seg105 */
extern signed short* g_wearable_items_index[12];
/* seg041 */
struct ranged_weapons {
	signed char damage_modifier[7];
	signed char base_handicap;
};
/* TODO: use it properly in seg041 */
extern struct ranged_weapons g_ranged_weapons_table[9];

extern signed short g_poison_potions[10];
extern signed short g_herbs_toxic[5];
extern signed short g_herbs_uneatable[7];
extern signed short g_elixir_potions[8];
extern signed short g_bad_elixirs[8];
extern signed short g_attack_items[3];
extern signed char g_monname_genders[78];

extern const signed short* g_magic_schools_index[9];
extern void (*g_spell_handlers[86])(void);		//ds:0x0dbb; seg098
extern void (*g_mon_spellhandlers[15])(void);		//ds:0x0fc2; seg102

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

extern Bit8u gs_algrid_treborns_hint;		//ds:0x3467; seg072

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

extern Bit8s  gs_dng07_poison_flag;		//ds:0x3cb5; seg082
extern Bit32s gs_dng07_poison_timer;		//ds:0x3cb6; seg002, seg082

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

extern Bit32s gs_dng12_watertrap_timer;		//ds:0x3fa2; seg002, seg090

extern Bit8s  gs_unconscious_message[7];	//ds:0x4212; seg002, seg049
extern Bit8s  gs_food_message[7];		//ds:0x4212; seg002, seg049, seg059
extern Bit16s gs_current_locdata;		//ds:0x4222; seg025-seg109
extern Bit16s gs_current_typeindex;		//ds:0x4224; seg025-seg120
extern Bit8u  gs_dng03_highpriest_killed;	//ds:0x4226; seg079
extern Bit8s  gs_dng03_chest12_loads;		//ds:0x4227; seg079
extern Bit16s gs_trv_i;				//ds:0x4228; seg063, seg094
extern Bit16s gs_route_stepcount;		//ds:0x422a; seg063, seg094
extern Bit16s gs_forcedmarch_le_cost;		//ds:0x422c; seg094

extern Bit16s gs_arsenal_money;			//ds:0x43a3; seg068
/* GameState written directly into SaveGame file ends at 0x4474 */

extern unsigned char g_fading_state;		//ds:0x4475; seg002-seg095
extern signed char g_check_party;		//ds:0x4495; seg002, seg032, seg063, seg113
extern signed char g_food_mod;			//ds:0x4496; seg002, seg051, seg052, seg054
extern signed char g_travel_herokeeping;	//ds:0x4497; seg002, seg063

extern signed char g_special_screen;		//ds:0x45b8; seg002-seg109

extern char g_dsa_version_string[12];		//ds:0x46e0; seg026
extern char g_game_version[6];			//ds:0x46ec; seg002
extern char g_version_token1;			//ds:0x46f8; seg026
extern char g_version_token2;			//ds:0x46f9; seg026
extern char g_version_token3;			//ds:0x46fa; seg026
extern char g_version_token4;			//ds:0x46fb; seg026

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

extern struct mouse_action g_action_table_options[10];	//ds:0x4bae; seg025

extern void (*g_location_handlers[19])(void);	//ds:0x4c3b; seg025

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

extern signed short g_fightobj_count;		//ds:0x605e; seg040
extern const signed short g_gfxtab_obj_offset_x[63];	//ds:0x6060; seg032, seg040
extern const signed short g_gfxtab_obj_offset_y[63];	//ds:0x6060; seg032, seg040
extern signed short g_force_weapons[9];		//ds:0x615c; seg041
extern signed short g_knive_weapons[5];		//ds:0x616e; seg041

extern signed short g_anitab_spell_ox[2];	//ds:0x633e; seg045
extern signed short g_anitab_spell_oy[2];	//ds:0x6342; seg045
extern signed char  g_anitab_spell_nvfno[2];	//ds:0x634c; seg045
extern signed char* g_anitab_spell_index[2];	//ds:0x6366; seg045

extern const struct struct_point g_invslot_iconxy_table[23];	//ds:0x63d2; seg046

extern signed char g_hero_sel_exclude;		//ds:0x64a2; seg047, seg049, seg103, seg120

extern const struct struct_point g_visual_field_offsets_std[29];	//ds:0x733a; seg066
extern const struct struct_point g_visual_field_offsets_sign[29];	//ds:0x73ae; seg066
extern const struct struct_point g_visual_field_offsets_inn[29];	//ds:0x7422; seg066
extern const struct struct_point g_visual_field_offsets_grass[29];	//ds:0x7496; seg066
extern unsigned char g_tex_descr_table[101][18];	//ds:0x750a; seg066
extern const signed char g_visual_field_draw_order[29];//ds:0x7c24; seg066
extern signed char g_can_merge_group;		//ds:0x7c41; seg066, seg076
extern signed char g_c_event_active;		//ds:0x7c42; seg002, seg067
extern const char g_str_bank_deposit_to_big[57];//ds:0x7c44; seg068
extern const char g_str_obviously_closed[28];	//ds:0x7c7d; seg068
extern signed char g_tmap_double1;		//ds:0x7c9a; seg025, seg072
extern signed char g_tmap_double2;		//ds:0x7c9b; seg025, seg072, seg082
extern signed char g_swafnild_traveled;		//ds:0x7c9c; seg063, seg072
extern unsigned char g_towns_gossip_max[52];	//ds:0x7c9d; seg073
extern unsigned char g_towns_gossip_off[52];	//ds:0x7cd1; seg073
extern const char g_gossip_str_triple_wildcard[8];	//ds:0x7d06;

extern const unsigned char g_automap_bitmask[8];	//ds:0x7d4a; seg002, seg074
extern const unsigned char g_automap_tile_arrowup[49];	//ds:0x7d52; seg074
extern const unsigned char g_automap_tile_arrowright[49];	//ds:0x7d83; seg074
extern const unsigned char g_automap_tile_cross[49];	//ds:0x7d4b; seg074
extern signed short g_automap_selx;		//ds:0x7de5; seg074, seg099
extern signed short g_automap_sely;		//ds:0x7de7; seg074, seg099

extern signed short g_dng_level_changed;	//ds:0x9312; seg075, seg076

extern const char g_dng01_str_marbo[6];		//ds:0x93d7; seg077

extern struct mouse_action g_action_table_travelmap[35];	//ds:0xa50f; seg093

extern char g_grammar_article_der[4];		//ds:0xa8d4; seg096
extern char g_grammar_article_die[4];		//ds:0xa8d8; seg096
extern char g_grammar_article_des[4];		//ds:0xa8e0; seg096
extern char g_grammar_article_den[4];		//ds:0xa8e4; seg096
extern char g_grammar_article_dem[4];		//ds:0xa8e8; seg096

extern char *g_grammar_articles_index[15];	//ds:0xa917; seg096

extern unsigned char *g_hexenknoten_gfx_buf;	// ds:0xacc8; seg100
extern signed short g_monster_spell_ae_cost;	// ds:0xaccc; seg102
extern char g_select_skill_lvlup[6]; 		// ds:0xacce; seg103
extern signed char g_select_skill_defaults[6];	// ds:0xacd4; seg103

extern void (*g_use_special_item_handlers[14])(void);	//ds:0xaeb0; seg107
extern int g_light_type;			// ds:0xaee8; seg101, seg107
extern void (*g_travel_event_handlers[146])(void);	//ds:0xaeea; seg109

/* BSS starts at 0xbc38, variables in reverse/decreasing order */

extern signed char g_large_buf;		// ds:0xe5e4; seg120, seg028
extern HugePt g_global_buffer_ptr;	// ds:0xe5e0; seg120
extern Bit32u g_buffersize;		// ds:0xe5dc; seg120
extern signed short g_basepos_x_bak;	// ds:0xe5d9; seg117
extern signed short g_basepos_y_bak;	// ds:0xe5d7; seg117
extern signed short g_textbox_width_bak; // ds:0xe5d5; seg117
extern signed short g_wallclock_update_bak; // ds:0xe5d3; seg117
extern unsigned char g_event_ani_busy;	// ds:0xe5d2; seg002, seg109-seg118
extern unsigned char *g_itemuser;	// ds:0xe5ce; seg107
extern unsigned char *g_spelluser;	// ds:0xe5bc; seg068, seg098, seg102, seg107
extern unsigned char *g_spelltarget;	// ds:0xe5b8; seg098-seg102

extern char g_str_beorn_hjall[20];	// ds:0xe42e; seg072

extern signed int g_fig_dropped_weapons[30];	// ds:0xe31a; seg032, seg041
extern signed int g_autofight;		// ds:0xe318; seg004-seg105
extern signed int g_current_fight_no;	// ds:0xe316; seg002-seg042
extern signed int g_tlk_id;		// ds:0xe314; seg030, seg031
extern signed int g_dialog_state;	// ds:0xe312; seg030, seg031, seg060
extern signed int g_dialog_done;	// ds:0xe310; seg030, seg031, seg060
extern signed int g_dialog_next_state;	// ds:0xe30e; seg030-seg117
extern signed int g_dialog_informer;	// ds:0xe30c; seg030, seg031
extern char *g_dialog_title;		// ds:0xe308; seg030, seg072

extern unsigned char g_good_camp_place;		// ds:0xe4c8; seg051, seg094, seg109
extern unsigned char *g_dungeon_doors_buf; 	// ds:0xe49c; seg028,seg076,seg098,seg120
extern unsigned char *g_dungeon_stairs_buf; 	// ds:0xe498; seg028,seg076,seg098,seg120
extern unsigned char *g_dungeon_fights_buf; 	// ds:0xe494; seg028,seg076,seg098,seg120
extern unsigned char *g_buffer11_ptr;		// ds:0xe404; seg028, seg075

extern HugePt g_buffer9_ptr4;		// ds:0xe3fc; seg028
extern Bit32s g_fightobj_buf_freespace;	// ds:0xe370; seg032-seg100
extern char g_savegame_names[5][9];	// ds:0xe2da; seg026, seg027
extern time_t g_last_save_time;		// ds:0xe2d6; seg026, seg059
extern unsigned char *g_saved_files_buf;// ds:0xe2d2; seg026;
extern signed short g_delay_timer;	// ds:0xe2d0; seg004, seg005

extern unsigned char* g_fig_gfxbuffers[8];	// ds:0xe278; seg005
extern Bit32s g_gfx_spinlock;		// ds:0xe234; seg004
extern char **g_itemsname;		// ds:0xe22f; seg026, seg120
extern unsigned char *g_itemsdat;	// ds:0xe22b; seg002, seg027, seg105, seg107, seg120
extern signed char g_market_itemsaldo_table[254]; // ds:0xe12d; seg056, seg057, seg120
extern char **g_monnames_index;		// ds:0xe129; seg026, seg120
extern unsigned char *g_monster_dat_buf;// ds:0xe125; seg032, seg039
extern unsigned char *g_mem_slots_anis;	// ds:0xe121; seg027, seg120
extern unsigned char *g_mem_slots_mfig;	// ds:0xe11d; seg027, seg120
extern unsigned char *g_mem_slots_wfig;	// ds:0xe119; seg027, seg120
extern unsigned char *g_mem_slots_mon;	// ds:0xe115; seg027, seg120
extern signed short g_wallclock_update;	// ds:0xe113; seg004-seg117, seg120
extern signed short g_wallclock_x;	// ds:0xe111; seg004, seg029, seg063, seg094
extern signed short g_wallclock_y;	// ds:0xe10f; seg004, seg029, seg063, seg094
extern signed short g_wallclock_redraw;	// ds:0xe10e; seg004, seg029

extern unsigned char *g_fightobj_buf_seek_ptr; // ds:0xd86e; seg005-seg100
extern signed char *g_chessboard;	// ds:0xd852;
extern signed short g_gather_herbs_mod;	// ds:0xd32f; seg051, seg109
extern signed short g_wildcamp_sleep_quality;	// ds:0xd32f; seg051, seg094, seg109, seg114, seg118
extern unsigned char *g_townpal_buf;	// ds:0xd312; seg002, seg027, seg120
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
extern HugePt g_buffer9_ptr3;		// ds:0xd019; seg003-seg120
extern unsigned char *g_buffer9_ptr2;	// ds:0xd015; seg120

extern Bit32s g_ani_unknown4;		// ds:0xce43; seg027, seg120
extern unsigned char *g_ani_main_ptr;	// ds:0xce36; seg004, seg027
extern HugePt g_buffer9_ptr;		// ds:0xc3db; seg004-seg120
extern char **g_text_ltx_index;		// ds:0xc3b5;
extern char **g_tx_index;		// ds:0xc3b1; seg026, seg028, seg031, seg064, seg120
extern char **g_tx2_index;		// ds:0xc3ad; seg
extern unsigned char *g_buffer8_ptr;	// ds:0xc3a9; seg005, seg026, seg028, seg032, seg042, seg050, seg061, seg098, seg120

extern struct_pic_copy g_pic_copy;	// ds:0xc00d; seg002-seg120

extern signed short g_textbox_width;	// ds:0xbffd; seg002-seg120
extern unsigned char *g_gui_buffer_unkn;// ds:0xbff9; seg025, seg032, seg097, seg120
extern unsigned char *g_heroes;		// ds:0xbd34
extern unsigned char *g_fightobj_buf;	// ds:0xbd30; seg032, seg040
extern signed char *g_scenario_buf;	// ds:0xbd2c; seg027, seg032, seg034, seg038, seg040
extern unsigned char *g_current_fight;	// ds:0xbd28; seg027, seg032, seg043, seg035, seg039, seg040, seg100
extern signed char g_citycamp_city;	// ds:0xbd27; seg052, seg066, seg076
extern Bit32s g_archive_file_remaining;	// ds:0xbce3; seg002
extern signed short g_map_townmark_state;	// ds:0xbcd8; seg002
extern unsigned char *g_splash_le;	// ds:0xbccb; seg002, seg028
extern unsigned char *g_splash_ae;	// ds:0xbcc7; seg002, seg028
extern unsigned char g_gfx_wait_keypress[100];	// ds:0xbc63; seg002
extern unsigned char g_playmask_us;	// ds:0xbc62; seg002, seg029

#if !defined(__BORLANDC__)
/* arrays for meaningful log messages */
const char* names_attrib[14];
const char* names_skill[52];
const char* names_spell[86];
const char* names_mspell[14];
#endif

#endif
