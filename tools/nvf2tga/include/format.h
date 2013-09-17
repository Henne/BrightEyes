/*
 * Header file for DSA/ROA file formats
 *
 * Authors:	Henne_NWH <henne@nachtwindheim.de>
 *		Hendrik <hermes9@web.de>
 * License:	GPLv3
 *
 */

/*
 * I/O-Funktionen für typische Datentypen
 */
static inline signed short get_sshort(const char* buf) {
    return (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline unsigned short get_ushort(const char* buf) {
    return (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline unsigned int get_uint(const char* buf) {
    return (buf[3]&0xff)<<24 | (buf[2]&0xff)<<16 | (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline int get_sint(const char* buf) {
    return (buf[3]&0xff)<<24 | (buf[2]&0xff)<<16 | (buf[1]&0xff)<<8 | (buf[0]&0xff);
}

static inline void set_sshort(char* buf, const signed short val) {
    buf[0] = (val >> 0) & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
}

static inline void set_ushort(char* buf, const unsigned short val) {
    buf[0] = (val >> 0) & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
}

static inline void set_uint(char* buf, const unsigned int val) {
    buf[0] = (val >>  0) & 0xFF;
    buf[1] = (val >>  8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;
}

static inline void set_sint(char* buf, const signed int val) {
    buf[0] = (val >>  0) & 0xFF;
    buf[1] = (val >>  8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;
}


/*
 * Header der NLT-Bildformate
 */
struct ace_header {
    char label[4];			/* "ACE\0"	*/
    short version;			/* 1		*/
    unsigned char sequences;	/* 0..250	*/
    unsigned char speed;		/* 0..99	*/
};

struct ass_header {
    short celwidth;			/* Größe der Cels	*/
    short celheight;
    unsigned char  amount;		/* Anzahl der Cels	*/
    unsigned char  playmode;	/* Abspielmodus		*/
};

struct seq_header {
    int offset;			/* Seek-Offset zur Sequenz	*/
    short label;			/* Kenn-Nummer der Sequenz	*/
    short celwidth;			/* Breite des Cels		*/
    short celheight;		/* Höhe der Cels		*/
    short hotspotx;			/* Koordianten des Hot Spots	*/
    short hotspoty;
    unsigned char amount;		/* Anzahl der Cels		*/
    unsigned char playmode;		/* Abspielmodus			*/
} __attribute__((__packed__));

struct cel_header {
    int size;			/* Größe der Cels		*/
    short xoffset;			/* Offset im Cel		*/
    short yoffset;
    short width;			/* Größe des Frames im Cel	*/
    short height;
    unsigned char compression;	/* Verwendeter Packer		*/
    unsigned char action;		/* Action Button der Cel	*/
} __attribute__((__packed__));

struct raw_header {
    char  label[26];                     /* Bildinformation (Copyright)  */
    short version;                       /* 0x1A00 (Zweck unbekannt)     */
    char  magic_nr[4];                   /* ID-String ROH (0x524F4800)   */
    short width;                         /* Bildbreite-1                 */
    short height;                        /* Bildhöhe-1                   */
    short palette_size;                  /* Anzahl der Paletteneinträge  */
} __attribute__((__packed__));

typedef struct struct_color {
    signed char r, g, b;
} Color;


/*
 * Strukturen für die interne Datenrepräsentation von Bildern und Frames
 */
typedef struct {
    unsigned short x0, y0;
    unsigned short width, height;
    unsigned short delay; // Delay in Millisekunden
    unsigned char* pixels;
    Color* localPalette;
    char* comment;
} AnimFrame;

typedef struct T_ImageSet {
    unsigned short globalWidth;
    unsigned short globalHeight;
    Color* globalPalette;
    unsigned short frameCount;
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
