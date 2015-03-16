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
#include <format.h>


typedef struct T_Unknown_Head {
    // TODO: Ich habe keine Ahnung, was diese Daten eigentlich sollen.
    uint8_t unk1;
    uint8_t len;
    uint8_t unk2;
    uint32_t* data;
} Unknown_Head;

const char* block_read(Unknown_Head* block, const char* buf, size_t len, uint8_t block_length) {
    block->unk1 = buf[0];
    block->len  = buf[1];
    block->unk2 = buf[2];

    printf("reading block of len %x\n", block_length);
    if (block_length != block->len) {
	fprintf(stderr, "Error reading BOB: inconsistent block lengths %d and %d\n",
		block_length, block->len);
	exit(-1);
    }

    block->data = malloc(block->len * sizeof(uint32_t));
    memcpy(block->data, buf+3, block->len);
    buf += 3 + 4*block->len;
    return buf;
}
void block_free(Unknown_Head* block) {
    free(block->data);
}

struct AnimStepData {
    uint16_t img_index;
    uint16_t duration;
};

typedef struct T_Ani_Head {
    char      name[4];
    uint16_t  x0;
    uint8_t   y0;
    uint8_t   height;
    uint16_t  width;
    uint8_t   unk1;
    uint8_t   frameCount;
    uint32_t  *pixelPos; //[frameCount];
    uint16_t  animSteps;
    struct AnimStepData *animStepData; //[unkData2Count];
} Ani_Head;

const char* ani_read(Ani_Head* ani, const char* buf, size_t len) {
    int i;
    memcpy(ani->name, buf, 4);
    ani->x0         = get_uint16(buf+4);
    ani->y0         = buf[6];
    ani->height     = buf[7];
    ani->width      = get_uint16(buf+8);
    ani->unk1       = buf[10];
    ani->frameCount = buf[11];
    buf += 4+3+3+2;

    ani->pixelPos = (uint32_t*)malloc(ani->frameCount * sizeof(uint32_t));
    for (i=0;  i < ani->frameCount;  i++) {
	ani->pixelPos[i] = get_uint32(buf + 4*i);
    }
    buf += ani->frameCount * sizeof(uint32_t);
    
    ani->animSteps = get_uint16(buf);
    buf += 2;
    ani->animStepData = (struct AnimStepData*)malloc(ani->animSteps * sizeof(struct AnimStepData));
    for (i=0;  i < ani->animSteps;  i++) {
	ani->animStepData[i].img_index = get_uint16(buf);
	ani->animStepData[i].duration  = get_uint16(buf);
	buf += 4;
    }

    printf("Sequence %c%c%c%c has %d frames of %dx%d at %d,%d: %d steps\n",
	   ani->name[0], ani->name[1], ani->name[2], ani->name[3],
	   ani->frameCount, ani->width, ani->height, ani->x0, ani->y0, ani->animSteps);
	   
    return buf;
}
void ani_free(Ani_Head* ani) {
    free(ani->pixelPos);
    free(ani->animStepData);
}

typedef struct T_BOB_Header {
    char signature[3];           // Signatur = "BOB"
    uint8_t  version;            // Major/Minor Version
    uint8_t  unk1;

    uint8_t  block_count;        // Anzahl der Teilanimationen (ohne Hauptbild)
    uint8_t  unk2[11];           // Unklarer Teil, fast überall 0x0A010000...
    uint8_t  *block_lengths;
    Unknown_Head *block;
    
    char*    imgdata_ofs; // An dieser Position beginnen die Bilddaten
    char*    paldata_ofs; // An dieser Position beginnt die Palette
    uint16_t width;
    uint8_t  height;
    uint8_t  ani_count;
    uint32_t *ani_headpos;  // relative Positionen der Animations-Header

    Ani_Head *ani_head;
    // Die folgenden Informationen stehen nicht direkt im Header
    long sigheader_size; // Größe des Headers mit der "BOB"-Signatur
    uint8_t compression_mode; // Kompressionsmodus. Wo zur Hölle steht der?
} BOB_Header;

const char* bob_read(BOB_Header* bob, const char* buf, size_t len) {
    const char* buf0 = buf;
    int i,j;
    long tmp = 0;
    memcpy(bob->signature, buf, 3);
    if (strncmp(bob->signature, "BOB", 3) == 0) {
	// Signaturlose BOB-Files überspringen diesen Bereich.
	bob->version         = buf[3];
	bob->unk1            = buf[4];
	bob->block_count     = buf[5];
	memcpy(bob->unk2,      buf+6, 11);
	bob->block_lengths = malloc(bob->block_count);
	memcpy(bob->block_lengths, buf+6+11, bob->block_count);
	buf += 6 + 11 + bob->block_count;
	// Unknown Header
	bob->block = (Unknown_Head*)malloc(bob->block_count * sizeof(Unknown_Head));
	for (i=0;  i < bob->block_count;  i++) {
	    buf = block_read(&(bob->block[i]), buf, len, bob->block_lengths[i]);
	}
	printf("BOB file has signature, global header continues at 0x%x.\n", buf-buf0);
    } else {
	printf("BOB file has no signature.\n");
    }
    bob->sigheader_size = buf - buf0;
    
    // Rest des BOB-Headers
    bob->imgdata_ofs = (char*)buf0 + bob->sigheader_size+0 + get_uint32(buf+0);
    bob->paldata_ofs = (char*)buf0 + bob->sigheader_size+4 + get_uint32(buf+4);
    bob->width     = get_uint16(buf + 8);
    bob->height    = buf[10];
    bob->ani_count = buf[11];
    bob->ani_headpos = malloc(bob->ani_count * sizeof(uint32_t));
    memcpy(bob->ani_headpos, buf+8+2+1+1, bob->ani_count * sizeof(uint32_t));

    printf("BOB has %d animations, global size %dx%d.\n",
	   bob->ani_count, bob->width, bob->height);
    buf += 4+4+2+1+1 + 4*bob->ani_count;

    // Animations-Header
    bob->ani_head = (Ani_Head*)malloc(bob->ani_count * sizeof(Ani_Head));
    for (i=0;  i < bob->ani_count;  i++) {
	buf = ani_read(&(bob->ani_head[i]), buf, len);
    }
    return buf;
}


void bob_free(BOB_Header* bob) {
    int i;
    free(bob->block_lengths);
    for (i=0;  i < bob->block_count;  i++) block_free(&(bob->block[i]));
    free(bob->block);
    for (i=0;  i < bob->ani_count;  i++) ani_free(&(bob->ani_head[i]));
    free(bob->ani_head);
}

int sanitycheck_bob(const char* buf, size_t len) {
    // TODO
    return 1;
}
/* TODO
 * Da ich nicht weiß, wo der Kompressionsmodus gespeichert wird,
 * errate ich den hier anhand der Bilddimensionen.
 * Das ist natürlich total ungenau und Scheiße.
 * Aber WO ZUR HÖLLE SOLL DER KOMPRESSIONSMODUS STECKEN?
 * force steht entweder auf -1 (raten), 0 (unkomprimiert) oder 0x50 (PP).
 */
uint8_t* guess_compression(BOB_Header* bob, uint16_t width, uint8_t height, uint8_t* ptr, int force) {
    uint32_t plen = get_uint32(ptr);
    if (force == 0x50 ||
	force ==   -1 && (unsigned long)plen < (unsigned long)(width*height)) {
	// wahrscheinlich PowerPacker
	printf("compression: pp, denn %lu < %lu\n", plen, width*height, force);
	uint8_t* ret = (uint8_t*)malloc(width*height);
	ppdepack(ptr, ret, plen, width * height);
	return ret;
    } else {
	printf("compression: raw, denn %lu >= %lu\n", plen, width*height);
	// sonst ist es wohl unkomprimiert.
	return ptr;
    }
}

ImageSet* process_bob(const char *buf, size_t len) {
    ImageSet* img;
    BOB_Header* bob;
    int i,j;
    uint8_t* pixelptr;
    const char* buf0 = buf;
    bob = (BOB_Header*)malloc(sizeof(BOB_Header));
    buf = bob_read(bob, buf, len);
    pixelptr = (char*)bob->imgdata_ofs;
    if (buf != (const char*)pixelptr) {
	printf("Warning: Header end (%x) and pixelptr (%x) don't match.",
	       buf-buf0, (const char*)pixelptr-buf0);
    }
    
    // Übertragen der Daten in die img-Struktur
    img = (ImageSet*)malloc(sizeof(ImageSet));
    img->width      = bob->width;
    img->height     = bob->height;
    img->mainPixels = guess_compression(bob, img->width, img->height, pixelptr, -1);
    int guessed_compression = -1;
    if (img->mainPixels == pixelptr) guessed_compression = 0x00;
    else                             guessed_compression = 0x50;

    img->seqCount   = bob->ani_count;
    img->sequences  = (Sequence*)malloc(img->seqCount * sizeof(Sequence));
    
    printf("pixelptr = %p\n", (const char*)pixelptr-buf0);
    for (i=0;  i < img->seqCount;  i++) {
	Sequence* seq = &img->sequences[i];
	Ani_Head* ani = &bob->ani_head[i];
	seq->name        = ani->name;
	seq->frameCount  = ani->animSteps;
	seq->frames      = (Frame*)ani->animStepData;
	seq->defaultDelay= 60;
	seq->imgCount    = ani->frameCount;
	seq->img         = malloc(seq->imgCount * sizeof(MyImage));
	for (j=0;  j < seq->imgCount;  j++) {
	    seq->img[j].width  = ani->width;
	    seq->img[j].height = ani->height;
	    seq->img[j].x0     = ani->x0;
	    seq->img[j].y0     = ani->y0;
	    seq->img[j].palette= NULL;
	    // TODO: auf pixelPos ist nicht unbedingt Verlass.
	    // Insbesondere in DSA3 sind die Frames einfach hintereinandergeschrieben,
	    // und pixelPos zeigt immer auf das gleiche Frame. Seufz.
	    pixelptr = (char*)buf0 + bob->sigheader_size + ani->pixelPos[j];
	    seq->img[j].pixels = guess_compression(bob, ani->width, ani->height,
						   pixelptr, guessed_compression);
	}
    }
    // Farbpalette
    img->palette = (Color*)malloc(256 * sizeof(Color));
    for (i=0;  i < 0x100;  i++) {
	img->palette[i].r = bob->paldata_ofs[i*3 + 2];
	img->palette[i].g = bob->paldata_ofs[i*3 + 3];
	img->palette[i].b = bob->paldata_ofs[i*3 + 4];
    }
    
    return img;
}

int dump_bob(ImageSet* img, char* prefix) {
    // TODO
    return 1;
}
