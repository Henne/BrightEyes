#if !defined(__BORLANDC__)

typedef struct
{
	unsigned short min_API_version;
	unsigned short drvr_type;
	char data_suffix[4];
	void *dev_name_table;
	signed short default_IO;
	signed short default_IRQ;
	signed short default_DMA;
	signed short default_DRQ;
	signed short service_rate;
	unsigned short display_size;
}
drvr_desc;

void AIL_startup();

void AIL_shutdown(char *signoff_msg);

signed short AIL_register_driver(void *driver_base_addr);

drvr_desc* AIL_describe_driver(signed short driver);

unsigned short AIL_detect_device(signed short driver, unsigned short IO_addr, unsigned short IRQ, unsigned short DMA, unsigned short DRQ);

void AIL_init_driver(signed short driver, unsigned short IO_addr, unsigned short IRQ, unsigned short DMA, unsigned short DRQ);

unsigned short AIL_state_table_size(signed short driver);

signed short AIL_register_sequence(signed short driver, void *FORM_XMID, unsigned short sequence_num, void *state_table, void *controller_table);

void AIL_release_sequence_handle(signed short driver, signed short sequence);

unsigned short AIL_default_timbre_cache_size(signed short driver);

void AIL_define_timbre_cache(signed short driver, void *cache_addr, unsigned short cache_size);

unsigned short AIL_timbre_request(signed short driver, signed short sequence);

void AIL_install_timbre(signed short driver, signed short bank, signed short patch, void *src_addr);

void AIL_start_sequence(signed short driver, signed short sequence);

void AIL_stop_sequence(signed short driver, signed short sequence);

unsigned short AIL_sequence_status(signed short driver, signed short sequence);

#endif
