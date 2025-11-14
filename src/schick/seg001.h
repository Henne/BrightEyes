#if !defined(__BORLANDC__)
namespace M302de {
#endif

void CD_enable_repeat(void);
signed int CD_bioskey(const signed int);
void CD_audio_stop(void);
void CD_audio_pause(void);
void CD_audio_play(void);
void CD_set_track(const signed int);
void CD_check(void);
signed int CD_init(void);

#if !defined(__BORLANDC__)
}
#endif
