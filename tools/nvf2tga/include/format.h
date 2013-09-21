/*
 * Header file for DSA/ROA file formats
 *
 * Authors:	Henne_NWH <henne@nachtwindheim.de>
 *		Hendrik <hermes9@web.de>
 * License:	GPLv3
 *
 */

#include <stdint.h> // Saubere int-Typen (int32_t u.s.w.)

/*
 * I/O-Funktionen für typische Datentypen
 */
static inline int16_t get_sint16(const char* buf) {
    return (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline uint16_t get_uint16(const char* buf) {
    return (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline uint32_t get_uint32(const char* buf) {
    return (buf[3]&0xff)<<24 | (buf[2]&0xff)<<16 | (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline int32_t get_sint32(const char* buf) {
    return (buf[3]&0xff)<<24 | (buf[2]&0xff)<<16 | (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline void set_uint16(char* buf, const uint16_t val) {
    buf[0] = (val >> 0) & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
}

static inline void set_sint16(char* buf, const int16_t val) {
    buf[0] = (val >> 0) & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
}

static inline void set_uint32(char* buf, const uint32_t val) {
    buf[0] = (val >>  0) & 0xFF;
    buf[1] = (val >>  8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;
}

static inline void set_sint32(char* buf, const int32_t val) {
    buf[0] = (val >>  0) & 0xFF;
    buf[1] = (val >>  8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;
}


/*
 * Header der NLT-Bildformate
 */
struct ace_header {
    char     label[4];			/* "ACE\0" */
    uint16_t version;			/* 1	   */
    uint8_t  sequences;	                /* 0..250  */
    uint8_t  speed;	                /* 0..99   */
};

struct ass_header {
    uint16_t celwidth;		 /* Größe der Cels  */
    uint16_t celheight;
    uint8_t  amount;		 /* Anzahl der Cels */
    uint8_t  playmode;	         /* Abspielmodus    */
};

struct seq_header {
    int      offset;		/* Seek-Offset zur Sequenz	*/
    uint16_t label;		/* Kenn-Nummer der Sequenz	*/
    uint16_t celwidth;		/* Breite des Cels		*/
    uint16_t celheight;		/* Höhe der Cels		*/
    int16_t  hotspotx;		/* Koordianten des Hot Spots	*/
    int16_t  hotspoty;
    uint8_t  amount;		/* Anzahl der Cels		*/
    uint8_t  playmode;		/* Abspielmodus			*/
} __attribute__((__packed__));

struct cel_header {
    int      size;		/* Größe der Cels		*/
    int16_t  xoffset;		/* Offset im Cel		*/
    int16_t  yoffset;
    uint16_t width;		/* Größe des Frames im Cel	*/
    uint16_t height;
    uint8_t  compression;	/* Verwendeter Packer		*/
    uint8_t  action;		/* Action Button der Cel	*/
} __attribute__((__packed__));

struct raw_header {
    char     label[26];                    /* Bildinformation (Copyright)  */
    uint16_t version;                      /* 0x1A00 (Zweck unbekannt)     */
    char     magic_nr[4];                  /* ID-String ROH (0x524F4800)   */
    uint16_t width;                        /* Bildbreite-1                 */
    uint16_t height;                       /* Bildhöhe-1                   */
    uint16_t palette_size;                 /* Anzahl der Paletteneinträge  */
} __attribute__((__packed__));

typedef struct struct_color {
    signed char r, g, b;
} Color;


/*
 * Strukturen für die interne Datenrepräsentation von Bildern und Frames
 */
typedef struct {
    uint16_t x0, y0;
    uint16_t width, height;
    uint16_t delay; // Delay in Millisekunden
    uint8_t* pixels;
    Color*   localPalette;
    char*    comment;
} AnimFrame;

typedef struct T_ImageSet {
    uint16_t  globalWidth;
    uint16_t  globalHeight;
    Color*    globalPalette;
    uint16_t  frameCount;
    AnimFrame **frames;
} ImageSet;

ImageSet* process_ace(const char *buf, size_t len);
ImageSet* process_aif(const char *buf, size_t len);
ImageSet* process_bob(const char *buf, size_t len);
ImageSet* process_gif(const char *buf, size_t len);
ImageSet* process_nvf(const char *buf, size_t len);
ImageSet* process_raw(const char *buf, size_t len);
ImageSet* process_tga(const char *buf, size_t len);
ImageSet* process_uli(const char *buf, size_t len);

int sanitycheck_ace(const char *buf, size_t len);
int sanitycheck_aif(const char *buf, size_t len);
int sanitycheck_bob(const char *buf, size_t len);
int sanitycheck_gif(const char *buf, size_t len);
int sanitycheck_nvf(const char *buf, size_t len);
int sanitycheck_raw(const char *buf, size_t len);
int sanitycheck_tga(const char *buf, size_t len);
int sanitycheck_uli(const char *buf, size_t len);

int dump_ace(ImageSet* img, char* prefix);
int dump_aif(ImageSet* img, char* prefix);
int dump_bob(ImageSet* img, char* prefix);
int dump_gif(ImageSet* img, char* prefix);
int dump_nvf(ImageSet* img, char* prefix);
int dump_raw(ImageSet* img, char* prefix);
int dump_tga(ImageSet* img, char* prefix);
int dump_uli(ImageSet* img, char* prefix);
