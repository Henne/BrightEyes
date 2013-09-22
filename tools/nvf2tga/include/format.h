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
typedef struct struct_color {
    signed char r, g, b;
} Color;


/*
 * Strukturen für die interne Datenrepräsentation von Bildern und Frames
 */

// Ein einzelnes Bild
typedef struct {
    uint16_t  x0, y0;
    uint16_t  width, height;
    Color*    palette;
    uint8_t*  pixels;
} MyImage;

// Ein einzelner Schritt der Animation
typedef struct {
    uint16_t index; // Index auf das Bild
    uint16_t delay; // Wie lange soll es angezeigt werden
} Frame;

typedef struct {
    char*    name;
    uint16_t frameCount;   // Anzahl der Animationsschritte
    Frame*   frames;       // Sequenz, in der die Bilder angezeigt werden
    uint16_t defaultDelay; // Wenn frameCount==0, verwende dieses Delay.
    uint16_t imgCount;     // Anzahl der verwendeten Bilder
    MyImage* img;          // Bilddaten
} Sequence;

typedef struct T_ImageSet {
    uint16_t  width;       // Breite des Bildes
    uint16_t  height;      // Höhe des Bildes
    Color*    palette;     // Globale Palette
    uint8_t*  mainPixels;  // Pixel des Haupt-/Hintergrundbildes
    uint16_t  seqCount;    // Anzahl der Sequenzen
    Sequence* sequences;   // Sequenzen
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
