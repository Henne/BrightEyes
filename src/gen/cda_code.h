#if defined(__BORLANDC__)
/* used by external modules */
signed short CD_bioskey(signed short);
void CD_audio_stop(void);
void CD_play_track(signed short);
signed short CD_audio_init(void);
#else
/* Dummy functions for modern OSes */
static inline signed short CD_bioskey(signed short cmd) { return 0; }
static inline void CD_audio_stop(void) { }
static inline void CD_play_track(signed short track) { }
static inline signed short CD_audio_init(void) { return 1; }
#endif
