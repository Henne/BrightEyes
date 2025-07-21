#if !defined V302DE_DATSEG_H
#define V302DE_DATSEG_H


#if defined(__BORLANDC__)
extern char ds[1];
#endif

/* seg001 */
extern unsigned short g_cd_init_successful;
extern Bit32u g_cd_skipmagic;

#if !defined(__BORLANDC__)
/* arrays for meaningful log messages */
const char* names_attrib[14];
const char* names_skill[52];
const char* names_spell[86];
const char* names_mspell[14];
#endif

#endif
