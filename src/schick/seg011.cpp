/**
 *	Rewrite of DSA1 v3.02_de functions of seg011 (AIL, miles sound system)
 *	Functions rewritten: 0/0
 */
#include "v302de.h"

#define AIL_SEGMENT (0x1042)

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void AIL_startup(void)
{
}

void AIL_shutdown(uint8_t* signoff_msg)
{
}

int16_t AIL_register_driver(uint8_t* driver)
{
	return 0;
}

uint8_t* AIL_describe_driver(uint16_t driver)
{
	return NULL;
}

uint16_t AIL_detect_device(uint16_t driver, uint16_t IO_addr, uint16_t IRQ, uint16_t DMA, uint16_t DRQ)
{
	return 0;
}

void AIL_init_driver(uint16_t driver, uint16_t IO_addr, uint16_t IRQ, uint16_t DMA, uint16_t DRQ)
{
}

void AIL_play_VOC_file(uint16_t driver, uint8_t* VOC_file, int16_t block_marker)
{
}

uint16_t AIL_format_VOC_file(uint16_t driver, uint8_t* VOC_file, int16_t block_marker)
{
	return 0;
}

uint16_t AIL_VOC_playback_status(uint16_t driver)
{
	return 0;
}

void AIL_start_digital_playback(uint16_t driver)
{
}

void AIL_stop_digital_playback(uint16_t driver)
{
}

void AIL_set_digital_playback_volume(uint16_t driver, uint16_t percent)
{
}

uint16_t AIL_state_table_size(uint16_t driver)
{
	return 0;
}

uint16_t AIL_register_sequence(uint16_t driver, uint8_t* FORM_XMID, uint16_t sequence_num, uint8_t* state_table, uint8_t* controller_table)
{
	return 0;
}

void AIL_release_sequence_handle(int16_t driver, int16_t sequence)
{
}

uint16_t AIL_default_timbre_cache_size(uint16_t driver)
{
	return 0;
}

void AIL_define_timbre_cache(uint16_t driver, uint8_t* cache_addr, uint16_t cache_size)
{
}

uint16_t AIL_timbre_request(uint16_t driver, uint16_t sequence)
{
	return 0;
}

void AIL_install_timbre(uint16_t driver, uint16_t bank, uint16_t patch, uint8_t* src_addr)
{
}

void AIL_start_sequence(int16_t driver, int16_t sequence)
{
}

void AIL_stop_sequence(int16_t driver, int16_t sequence)
{
}

uint16_t AIL_sequence_status(uint16_t driver, uint16_t sequence)
{
	return 0;
}

void AIL_set_relative_volume(uint16_t driver, uint16_t sequence, uint16_t percent, uint16_t ms)
{
}

#if !defined(__BORLANDC__)
}
#endif
