/*
 * ULI loader
 *
 * Loads/Dumps an ULI animation to/from an ImageSet structure.
 * ULI files are used in the Schicksalsklinge/Blade of destiny 1 intro.
 *
 * Authors: Henne_NWH <henne@nachtwindheim.de>
 *          Hendrik <hermes9@web.de>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <format.h>

/*
  Offenbar ein Format für Animationen/Bilder. Codiert mit einer Art von RLE, siehe unten.
  TODO: Animationsdaten und Bitbreite aus dem Header holen, Headerlänge bestimmen.
  |  Byte | Bedeutung                                                                  |
  |  0-25 | Signatur "(C) 1991 by Ulrich Walther"                                      |
  | 26-28 | Immer identische Bytefolge 0x1A0085                                        |
  | 29-30 | Breite des Bildes -1                                                       |
  | 31-32 | Höhe des Bildes -1                                                         |
  |   ??? | Unbekannte Menge an Animations- und Darstellungsoptionen.                  |
  |       | Das 1. Byte scheint mit der Anzahl/Länge dieser Optionen zusammenzuhängen. |
  Komprimierung ist eine Mischung aus RLE und Bitmap, wobei die Bitbreite für den Bitmap-Modus von Datei zu Datei unterschiedlich ist. Wo der Bitmodus und die Farbpalette gespeichert sind, ist noch unklar.
*/

int sanitycheck_uli(const char* buf, size_t len) {
	if (strncmp(buf, "(C) 1991 by Ulrich Walther\x1A\x00\x85", 0x1C)) return 1;
	else return 0;
}

ImageSet* process_uli(const char *buf, size_t len) {
	const char* buf_end = buf + len;
	const uint16_t bitbreite = 8;
	char* imgptr;
	ImageSet* img;
	
	
	img = (ImageSet*)malloc(sizeof(ImageSet));
	img->globalWidth   = get_uint16(buf + 0x1D)+1;
	img->globalHeight  = get_uint16(buf + 0x1F)+1;
	img->frameCount    = 1; // TODO???
	img->globalPalette = 0; // TODO???
	img->frames       = (AnimFrame**)malloc(img->frameCount * sizeof(AnimFrame*));
	for (int i = 0; i<img->frameCount; i++) img->frames[i] = (AnimFrame*)malloc(sizeof(AnimFrame));

	AnimFrame* frame = img->frames[0];
	frame->x0 = frame->y0 = 0;
	frame->width  = img->globalWidth;
	frame->height = img->globalHeight;
	frame->delay  = 0;
	frame->localPalette = 0;
	frame->pixels = malloc(frame->width * frame->height);
	
	imgptr = frame->pixels;
	buf += 0x20; // TODO: Wieviel genau weiß ich noch nicht!
	while  ( (buf < buf_end)) {
		uint8_t cur = *(buf++);
		if (cur < 0x80) {
			for (int i=0; i < cur*(8/bitbreite); i++) {
				*(imgptr++) = *buf;
			}
			buf++;
		} else {
			int runlength = (cur - 0x80);
			for (int i=0; i<runlength; i++) {
				printf("%02x", *buf);
				for (int bit=8-bitbreite;   bit >= 0;   bit-=bitbreite) {
					*(imgptr++) = (2^bitbreite-1) & (*buf >> bit);
				}
			}
		}
	}
	return img;
}
int dump_uli(ImageSet* img, char* prefix) {
	// TODO
	return 1;
}
