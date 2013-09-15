/*
 * Header file for DSA/ROA data loaders
 *
 * Authors: Henne_NWH <henne@nachtwindheim.de>
 *          Hendrik <hermes9@web.de>
 * Licanse: GPLv3
 *
 */

typedef struct {
	unsigned short x0, y0;
	unsigned short width, height;
	unsigned short delay; // Delay in Millisekunden
	unsigned char* pixels;
	unsigned char *localPalette;
	char* comment;
} AnimFrame;

typedef struct T_ImageSet {
    unsigned short globalWidth;
    unsigned short globalHeight;
    unsigned char *globalPalette;
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
