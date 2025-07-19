#if !defined V302DE_DATSEG_H
#define V302DE_DATSEG_H


#if defined(__BORLANDC__)
#define DS_SIZE (0xf7af - 0x938 - 2)
extern char ds[1];
#endif

#if !defined(__BORLANDC__)
/* arrays for meaningful log messages */
const char* names_attrib[];
const char* names_skill[];
const char* names_spell[];
const char* names_mspell[];
#endif

#endif
