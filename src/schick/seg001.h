#if !defined(__BORLANDC__)
namespace M302de {
#endif

void CD_enable_repeat(void);
int CD_bioskey(const int);
void CD_audio_stop(void);
void CD_audio_pause(void);
void CD_audio_play(void);
void CD_set_track(const int);
void CD_check(void);
int CD_init(void);

#if !defined(__BORLANDC__)
}
#endif
