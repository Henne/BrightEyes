#include "v302de.h"

void AIL_startup(void);

void AIL_shutdown(uint8_t* signoff_msg);

int16_t AIL_register_driver(uint8_t*);

uint8_t* AIL_describe_driver(uint16_t);

uint16_t AIL_detect_device(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

void AIL_init_driver(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

void AIL_play_VOC_file(uint16_t, uint8_t*, int16_t);

uint16_t AIL_format_VOC_file(uint16_t, uint8_t*, int16_t);

uint16_t AIL_VOC_playback_status(uint16_t);

void AIL_start_digital_playback(uint16_t);

void AIL_stop_digital_playback(uint16_t);

void AIL_set_digital_playback_volume(uint16_t, uint16_t);

uint16_t AIL_register_sequence(uint16_t driver, uint8_t* FORM_XMID, uint16_t sequence_num, uint8_t* state_table, uint8_t* controller_table);

uint16_t AIL_state_table_size(uint16_t);

void AIL_release_sequence_handle(int16_t driver, int16_t sequence);

uint16_t AIL_default_timbre_cache_size(uint16_t);

void AIL_define_timbre_cache(uint16_t, uint8_t*, uint16_t);

uint16_t AIL_timbre_request(uint16_t driver, uint16_t sequence);

void AIL_install_timbre(uint16_t driver, uint16_t bank, uint16_t patch, uint8_t* src_addr);

void AIL_start_sequence(int16_t driver, int16_t sequence);

void AIL_stop_sequence(int16_t driver, int16_t sequence);

uint16_t AIL_sequence_status(uint16_t driver, uint16_t sequence);

void AIL_set_relative_volume(uint16_t, uint16_t, uint16_t, uint16_t);
