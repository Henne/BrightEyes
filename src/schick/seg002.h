#if !defined(__BORLANDC__)
namespace M302de {
#endif

#include "datseg.h"

void play_music_file(signed short);
void set_audio_track(uint16_t);
void sound_menu(void);
/* static */
void read_sound_cfg(void);
void init_AIL(uint32_t);
void exit_AIL(void);
/* static */
uint8_t* read_music_driver(uint8_t*);
/* static */
signed short prepare_midi_playback(signed short);
/* static */
signed short start_midi_playback(signed short);
/* static */
uint8_t* prepare_timbre(signed short, signed short);
/* static */
signed short load_midi_file(signed short);
/* static */
signed short do_load_midi_file(signed short);
/* static */
signed short load_music_driver(uint8_t*, signed short, signed short);
/* static */
void do_play_music_file(signed short);
/* static */
void stop_midi_playback(void);
void start_midi_playback_IRQ(void);
signed short have_mem_for_sound(void);
void play_voc(signed short);
void play_voc_delay(signed short);
void alloc_voc_buffer(uint32_t);
/* static */
void free_voc_buffer(void);
/* static */
signed short read_new_voc_file(signed short);
/* static */
signed short read_voc_file(signed short);

/* static */
void SND_play_voc(signed short);
/* static */
void SND_stop_digi(void);
/* static */
void SND_set_volume(unsigned short);
/* static */
signed short load_digi_driver(uint8_t*, signed short, signed short, signed short);
/* static */
unsigned char* read_digi_driver(char*);
//static
signed short open_and_seek_dat(unsigned short);
uint32_t get_readlength2(signed short);
unsigned short read_archive_file(uint16_t, uint8_t *, uint16_t);
void seek_archive_file(uint16_t, int32_t, ...);
signed short load_regular_file(uint16_t);
signed short load_archive_file(uint16_t);
signed short open_temp_file(unsigned short);
void copy_from_archive_to_temp(unsigned short, char*);
void copy_file_to_temp(char*, char*);
int32_t process_nvf_extraction(struct nvf_extract_desc *);
#if defined(__BORLANDC__)
void interrupt mouse_isr(void);
#endif
//static
void mouse_action(unsigned short*, unsigned short*, unsigned short*, unsigned short*, unsigned short*);
signed int is_mouse_in_rect(const signed int, const signed int, const signed int, const signed int);
void mouse_init(void);
void disable_mouse(void);
#if defined(__BORLANDC__)
void mouse_get_button_press_info(unsigned short*, unsigned short*, unsigned short*, unsigned short*);
void call_mouse_isr(void);
void mouse_irq_init(signed short, void interrupt *);
#endif
void mouse_reset_ehandler(void);
void mouse_move_cursor(const signed int, const signed int);
#if defined(__BORLANDC__)
void mouse_setGMask(const unsigned int, const unsigned int, signed short, signed short);
void mouse_setCRTpage(const unsigned int);
#endif
void make_ggst_cursor(uint8_t *icon);
void call_mouse_bg(void);
void call_mouse(void);
void handle_gui_input(void);
signed int get_mouse_action(const signed int, const signed int, const struct mouse_action*);
void handle_input(void);
void flush_keyboard_queue_alt(void);
void pal_fade(int8_t*, int8_t*);
void pal_fade_in(int8_t*, int8_t*, const signed int, const signed int);

signed int get_current_season(void);
void do_timers(void);
void sub_ingame_timers(const int32_t);
void sub_mod_timers(const int32_t);
signed int get_free_mod_slot(void);
void set_mod_slot(const signed int, const int32_t, const uint8_t*, const signed char, const signed char);
void sub_heal_staffspell_timers(const int32_t);
void sub_light_timers(const int32_t);
void herokeeping(void);

void update_travelmap(void);
void set_and_spin_lock(void);
void timewarp(const int32_t);
void timewarp_until_time_of_day(const int32_t);
void dec_splash(void);
void timewarp_until_midnight(void);
void flush_keyboard_queue(void);
void wait_for_keypress(void);
void vsync_or_key(const signed int);

int32_t swap_u32(uint32_t);

#if defined(__BORLANDC__)
signed int alloc_EMS(const int32_t);
void from_EMS(const uint8_t*, const signed int, int32_t);
void to_EMS(const signed int, const uint8_t*, int32_t);
#endif

void clear_menu_icons(void);
void draw_loc_icons(const signed int, ...);

signed int mod_day_timer(const signed int);
void draw_compass(void);
signed int can_merge_group(void);
unsigned short div16(unsigned char);

void select_with_mouse(signed int*, const struct shop_item*);
void select_with_keyboard(signed int*, const struct shop_item*);

void set_automap_tile(const signed int, const signed int);
void set_automap_tiles(const signed int, const signed int);

signed int check_hero(const struct struct_hero*);
signed int is_hero_available_in_group(const struct struct_hero*);
void sub_ae_splash(struct struct_hero*, signed int);
void add_hero_ae(struct struct_hero*, const signed int);
void sub_hero_le(struct struct_hero*, const signed int);
void add_hero_le(struct struct_hero*, const signed int);
void add_group_le(const signed int);
signed int test_attrib(const struct struct_hero*, const signed int, const signed int);
signed int test_attrib3(const struct struct_hero*, const signed int, const signed int, const signed int, signed char);
signed int get_random_hero(void);
int32_t get_party_money(void);
void set_party_money(int32_t);
void add_party_money(const int32_t);
void add_hero_ap(struct struct_hero*, const int32_t);
void add_group_ap(int32_t);
void add_hero_ap_all(const signed int);
void sub_hero_ap_all(const signed int);
signed int get_hero_index(const struct struct_hero*);
signed int inv_slot_of_item(const struct struct_hero*, const signed int);

signed int get_first_hero_with_item(const signed int);
signed int get_first_hero_with_item_in_group(const signed int, const signed int);

void sub_group_le(const signed int);

struct struct_hero* get_first_hero_available_in_group(void);
struct struct_hero* get_second_hero_available_in_group(void);

signed int count_heroes_available(void);
#ifdef M302de_ORIGINAL_BUGFIX
/* this function allows cleaner fixes for Original-Bug 15 */
signed int count_heroes_available_ignore_npc(void);
#endif
signed int count_heroes_available_in_group(void);
#ifdef M302de_ORIGINAL_BUGFIX
/* this function allows cleaner fixes for Original-Bug 12, 13, 14 and 15 */
signed int count_heroes_available_in_group_ignore_npc(void);
#endif

void check_group(void);
uint8_t* schick_alloc(uint32_t);

#if !defined(__BORLANDC__)
}
#endif
