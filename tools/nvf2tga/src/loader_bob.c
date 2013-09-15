/*
 * BOB loader
 *
 * Loads/Dumps an BOB animation to/from an ImageSet structure.
 * BOB files are used by DSA/ROA 2+3.
 *
 * Authors: Henne_NWH <henne@nachtwindheim.de>
 *          Hendrik <hermes9@web.de>
 * License: GPLv3
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <loader.h>
#include <format.h>


struct Dir_Minihead{}; // TODO
struct Ani_Minihead{}; // TODO
struct Mainimg_Head{}; // TODO
struct Ani_Head{};     // TODO

struct BOB_Header {
    char signature[3];              // Signatur = "BOB"
    unsigned char version_maj, version_min;    // Major/Minor Version
    unsigned char ani_count;                   // Anzahl der Teilanimationen (ohne Hauptbild)
    char reserved1[11];             // Unklarer Teil, fast überall 0x0A010000...
    struct Dir_Minihead dir_minihead;
    struct Ani_Minihead ani_minihead;
    struct Mainimg_Head mainimg_head;
    struct Ani_Head* ani_head;
    // Imagedata
    // Palette??
};

int sanitycheck_bob(const char* buf, size_t len) {
	// TODO
	return 1;
}

ImageSet* process_bob(const char *buf, size_t len) {
	// TODO
	return NULL;
}

int dump_bob(ImageSet* img, char* prefix) {
	// TODO
	return 1;
}
