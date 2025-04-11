#if !defined(__BORLANDC__)

#include "g105de_seg006.h"

void AIL_startup()
{
}

void AIL_shutdown(char *signoff_msg)
{
}

signed short AIL_register_driver(void *driver_base_addr)
{
	return 0;
}

drvr_desc* AIL_describe_driver(signed short driver)
{
	return 0L;
}

unsigned short AIL_detect_device(signed short driver, unsigned short IO_addr, unsigned short IRQ, unsigned short DMA, unsigned short DRQ)
{
	return 0;
}

void AIL_init_driver(signed short driver, unsigned short IO_addr, unsigned short IRQ, unsigned short DMA, unsigned short DRQ)
{
}

unsigned short AIL_state_table_size(signed short driver)
{
	return 0;
}

signed short AIL_register_sequence(signed short driver, void *FORM_XMID, unsigned short sequence_num, void *state_table, void *controller_table)
{
	return 0;
}

void AIL_release_sequence_handle(signed short driver, signed short sequence)
{
}

unsigned short AIL_default_timbre_cache_size(signed short driver)
{
	return 0;
}

void AIL_define_timbre_cache(signed short driver, void *cache_addr, unsigned short cache_size)
{
}

unsigned short AIL_timbre_request(signed short driver, signed short sequence)
{
	return 0;
}

void AIL_install_timbre(signed short driver, signed short bank, signed short patch, void *src_addr)
{
}

void AIL_start_sequence(signed short driver, signed short sequence)
{
}

void AIL_stop_sequence(signed short driver, signed short sequence)
{
}

unsigned short AIL_sequence_status(signed short driver, signed short sequence)
{
	return 0;
}
#endif
