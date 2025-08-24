#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void AIL_startup(void);

void AIL_shutdown(Bit8u* signoff_msg);

Bit16s AIL_register_driver(Bit8u*);

Bit8u* AIL_describe_driver(Bit16u);

Bit16u AIL_detect_device(Bit16u, Bit16u, Bit16u, Bit16u, Bit16u);

void AIL_init_driver(Bit16u, Bit16u, Bit16u, Bit16u, Bit16u);

void AIL_play_VOC_file(Bit16u, Bit8u*, Bit16s);

Bit16u AIL_format_VOC_file(Bit16u, Bit8u*, Bit16s);

Bit16u AIL_VOC_playback_status(Bit16u);

void AIL_start_digital_playback(Bit16u);

void AIL_stop_digital_playback(Bit16u);

void AIL_set_digital_playback_volume(Bit16u, Bit16u);

Bit16u AIL_register_sequence(Bit16u driver, Bit8u* FORM_XMID, Bit16u sequence_num, Bit8u* state_table, Bit8u* controller_table);

Bit16u AIL_state_table_size(Bit16u);

void AIL_release_sequence_handle(Bit16s driver, Bit16s sequence);

Bit16u AIL_default_timbre_cache_size(Bit16u);

void AIL_define_timbre_cache(Bit16u, Bit8u*, Bit16u);

Bit16u AIL_timbre_request(Bit16u driver, Bit16u sequence);

void AIL_install_timbre(Bit16u driver, Bit16u bank, Bit16u patch, Bit8u* src_addr);

void AIL_start_sequence(Bit16s driver, Bit16s sequence);

void AIL_stop_sequence(Bit16s driver, Bit16s sequence);

Bit16u AIL_sequence_status(Bit16u driver, Bit16u sequence);

void AIL_set_relative_volume(Bit16u, Bit16u, Bit16u, Bit16u);

#if !defined(__BORLANDC__)
}
#endif
