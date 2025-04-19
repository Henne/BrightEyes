#if defined(__BORLANDC__)
/* used by external modules */
signed short CD_bioskey(signed short);
void seg001_033b(void);
void CD_play_track(signed short);
signed short seg001_0600(void);
#else
/* Dummy functions for modern OSes */
static inline signed short CD_bioskey(signed short cmd) { return 0; }
static inline void seg001_033b(void) { }
static inline void CD_play_track(signed short track) { }
static inline signed short seg001_0600(void) { return 1; }
#endif
