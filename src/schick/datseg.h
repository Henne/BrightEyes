#if !defined V302DE_DATSEG_H
#define V302DE_DATSEG_H


#if defined(__BORLANDC__)
extern char ds[1];
#endif

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

extern unsigned char g_always_zero4;			//ds:0x2cce; seg032

extern signed char g_levelup_ta_rise[12];		//ds:0x2d0f; seg050
extern signed char g_levelup_sp_rise[6];		//ds:0x2d1b; seg050
extern signed char g_levelup_spta_conv[6];		//ds:0x2d21; seg050
extern signed char g_mr_modificators[13];		//ds:0x2d27; seg050
extern void (*g_location_handlers[19])(void);
extern void (*g_use_special_item_handlers[14])(void);
extern void (*g_travel_event_handlers[146])(void);

extern Bit32s gs_day_timer;			//ds:0x2dbb; seg002-seg120
extern unsigned char g_always_one2;			//ds:0x35f3; seg078

extern Bit8s  gs_dng07_poison_flag;		//ds:0x3cb5; seg082
extern Bit32s gs_dng07_poison_timer;		//ds:0x3cb6; seg002, seg082

extern unsigned char *g_hexenknoten_gfx_buf;	// ds:0xacc8; seg100
extern signed short g_monster_spell_ae_cost;	// ds:0xaccc; seg102
extern char g_select_skill_lvlup[6]; 		// ds:0xacce; seg103
extern signed char g_select_skill_defaults[6];	// ds:0xacd4; seg103

/* BSS */

extern signed char g_large_buf;		// ds:0xe5e4; seg120, seg028
extern HugePt g_global_buffer_ptr;	// ds:0xe5e0; seg120
extern Bit32u g_buffersize;		// ds:0xe5dc; seg120
extern signed short g_basepos_x_bak;	// ds:0xe5d9; seg117
extern signed short g_basepos_y_bak;	// ds:0xe5d7; seg117
extern signed short g_textbox_width_bak; // ds:0xe5d5; seg117
extern signed short g_wallclock_update_bak; // ds:0xe5d3; seg117
extern unsigned char g_event_ani_busy;	// ds:0xe5d2; seg002, seg109-seg118
extern unsigned char *g_itemuser;	// ds:0xe5ce; seg107

extern unsigned char *g_dungeon_doors_buf; 	// ds:0xe49c; seg028,seg076,seg098,seg120
extern unsigned char *g_dungeon_stairs_buf; 	// ds:0xe498; seg028,seg076,seg098,seg120
extern unsigned char *g_dungeon_fights_buf; 	// ds:0xe494; seg028,seg076,seg098,seg120

extern HugePt g_buffer9_ptr;		// ds:0xc3db; seg004-seg120
extern HugePt g_buffer9_ptr4;		// ds:0xe3fc; seg028
extern Bit32s g_fightobj_buf_freespace;	// ds:0xe370; seg032-seg100
extern char **g_itemsname;		// ds:0xe22f; seg026, seg120
extern unsigned char *g_itemsdat;	// ds:0xe22b; seg002, seg027, seg105, seg107, seg120
extern signed char g_market_itemsaldo_table[254]; // ds:0xe12d; seg056, seg057, seg120
extern char **g_monnames_index;		// ds:0xe129; seg026, seg120
extern unsigned char *g_mem_slots_anis;	// ds:0xe121; seg027, seg120
extern unsigned char *g_mem_slots_mfig;	// ds:0xe11d; seg027, seg120
extern unsigned char *g_mem_slots_wfig;	// ds:0xe119; seg027, seg120
extern unsigned char *g_mem_slots_mon;	// ds:0xe115; seg027, seg120
extern signed short g_wallclock_update;	// ds:0xe113; seg004-seg117, seg120
extern unsigned char *g_saved_files_buf;// ds:0xe2d2; seg026;
extern unsigned char *g_spelltarget;	// ds:0xe5b8; seg098-seg102
extern unsigned char *g_spelluser;	// ds:0xecbc; seg068, seg098, seg102, seg107
extern signed char *g_chessboard;	// ds:0xd852;
extern unsigned char *g_townpal_buf;	// ds:0xd312; seg002, seg027, seg120
extern unsigned char *g_renderbuf_ptr;	// ds:0xd303; seg002-seg120
extern unsigned char *g_icon;		// ds:0xd2f7; seg002-seg058
extern signed char *g_dtp2;		// ds:0xd2f3: seg002-seg121
extern signed char *g_text_input_buf;	// ds:0xd2ef: seg002-seg120
extern signed char *g_text_output_buf;	// ds:0xd2eb: seg002-seg120
extern unsigned char *g_buf_icon;	// ds:0xd2e7; seg029, seg120
extern unsigned char *g_objects_nvf_buf;// ds:0xd2e3; seg004, seg032, seg040, seg120
extern unsigned char *g_buf_font6;	// ds:0xd2c1; seg026, seg096, seg120
extern char *g_buffer7_ptr;		// ds:0xd2b5; seg026, seg028, seg066, seg120
extern unsigned char *g_buffer6_ptr;	// ds:0xd2b1; seg002, seg027, seg063, seg093, seg094, seg120
extern char *g_buffer5_ptr;		// ds:0xd2a5; seg026, seg120
extern unsigned char *g_splash_buffer;	// ds:0xbcc7; seg002, seg028, seg097, seg120
extern char *g_text_ltx_buffer;		// ds:0xd2b9; seg026, seg120
extern unsigned char *g_popup;		// ds:0xd2ad; seg027, seg096, seg120
extern char *g_monnames_buffer;		// ds:0xd2a1; seg026, seg120
extern unsigned char *g_trv_track_pixel_bak;	// ds:0xd299; seg063, seg094, seg120
extern HugePt g_buffer9_ptr3;		// ds:0xd019; seg003-seg120
extern unsigned char *g_buffer9_ptr2;	// ds:0xd015; seg120
extern Bit32s g_ani_unknown4;		// ds:0xce43; seg027, seg120
extern char **g_text_ltx_index;		// ds:0xc3b5;
extern char **g_tx_index;		// ds:0xc3b1; seg026, seg028, seg031, seg064, seg120
extern char **g_tx2_index;		// ds:0xc3ad; seg
extern unsigned char *g_buffer8_ptr;	// ds:0xc3a9; seg005, seg026, seg028, seg032, seg042, seg050, seg061, seg098, seg120
extern unsigned char *g_splash_le;	// ds:0xbccb; seg002, seg028
extern unsigned char *g_splash_ae;	// ds:0xbcc7; seg002, seg028
extern unsigned char g_gfx_wait_keypress[100];	// ds:0xbc63; seg002
extern unsigned char g_playmask_us;	// ds:0xbc62; seg002, seg029
extern unsigned char *g_heroes;		// ds:0xbd34

#if !defined(__BORLANDC__)
/* arrays for meaningful log messages */
const char* names_attrib[14];
const char* names_skill[52];
const char* names_spell[86];
const char* names_mspell[14];
#endif

#endif
