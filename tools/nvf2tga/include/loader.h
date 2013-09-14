/*
 * Header file for DSA/ROA data loaders
 *
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * Licanse: GPLv3
 *
 */

typedef struct {
	unsigned short x0, y0;
	unsigned short width, height;
	unsigned short delay;
	unsigned char* pixels;
	unsigned char *localPalette;
	char* comment;
} AnimFrame;

typedef struct T_ImageSet {
    unsigned short globalWidth;
    unsigned short globalHeight;
    const unsigned char *globalPalette;
    unsigned short frameCount;
    AnimFrame **frames;
} ImageSet;

ImageSet* process_nvf(const char *buf, size_t len);
ImageSet* process_ace(const char *buf, size_t len);
ImageSet* process_aif(const char *buf, size_t len);
ImageSet* process_raw(const char *buf, size_t len);
ImageSet* process_bob(const char *buf, size_t len);
ImageSet* process_gif(const char *buf, size_t len);
ImageSet* process_tga(const char *buf, size_t len);

int sanitycheck_nvf(const char *buf, size_t len);
int sanitycheck_ace(const char *buf, size_t len);
int sanitycheck_aif(const char *buf, size_t len);
int sanitycheck_raw(const char *buf, size_t len);
int sanitycheck_bob(const char *buf, size_t len);
int sanitycheck_gif(const char *buf, size_t len);
int sanitycheck_tga(const char *buf, size_t len);

int dump_nvf(ImageSet* img);
int dump_ace(ImageSet* img);
int dump_aif(ImageSet* img);
int dump_raw(ImageSet* img);
int dump_bob(ImageSet* img);
int dump_gif(ImageSet* img);
int dump_tga(ImageSet* img);
