/**
 *	Rewrite of DSA1 v3.02_de functions of seg011 (AIL, miles sound system)
 *	Functions rewritten: 0/0
 */
#if !defined(__BORLANDC__)

#define AIL_SEGMENT (0x1042)

namespace M302de {

void AIL_startup(void)
{
}

void AIL_shutdown(RealPt signoff_msg)
{
}

Bit16s AIL_register_driver(RealPt driver)
{
	return 0;
}

RealPt AIL_describe_driver(Bit16u driver)
{
	return NULL;
}

Bit16u AIL_detect_device(Bit16u driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ)
{
	return 0;
}

void AIL_init_driver(Bit16u driver, Bit16u IO_addr, Bit16u IRQ, Bit16u DMA, Bit16u DRQ)
{
}

void AIL_play_VOC_file(Bit16u driver, RealPt VOC_file, Bit16s block_marker)
{
}

Bit16u AIL_format_VOC_file(Bit16u driver, RealPt VOC_file, Bit16s block_marker)
{
	return 0;
}

Bit16u AIL_VOC_playback_status(Bit16u driver)
{
	return 0;
}

void AIL_start_digital_playback(Bit16u driver)
{
}

void AIL_stop_digital_playback(Bit16u driver)
{
}

void AIL_set_digital_playback_volume(Bit16u driver, Bit16u percent)
{
}

Bit16u AIL_state_table_size(Bit16u driver)
{

Bit16u AIL_register_sequence(Bit16u driver, RealPt FORM_XMID, Bit16u sequence_num, RealPt state_table, RealPt controller_table)
{
	return 0;
}

void AIL_release_sequence_handle(Bit16s driver, Bit16s sequence)
{
}

Bit16u AIL_default_timbre_cache_size(Bit16u driver)
{
	return 0;
}

void AIL_define_timbre_cache(Bit16u driver, RealPt cache_addr, Bit16u cache_size)
{
}

Bit16u AIL_timbre_request(Bit16u driver, Bit16u sequence)
{
	return 0;
}

void AIL_install_timbre(Bit16u driver, Bit16u bank, Bit16u patch, RealPt src_addr)
{
}

void AIL_start_sequence(Bit16s driver, Bit16s sequence)
{
}

void AIL_stop_sequence(Bit16s driver, Bit16s sequence)
{
}

Bit16u AIL_sequence_status(Bit16u driver, Bit16u sequence)
{
	return 0;
}

void AIL_set_relative_volume(Bit16u driver, Bit16u sequence, Bit16u percent, Bit16u ms)
{
}

}
#endif
