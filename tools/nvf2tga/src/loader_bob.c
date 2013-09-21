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

typedef struct T_Ani_Head {
    char      name[4];
    uint16_t  x0;
    uint8_t   y0;
    uint8_t   height;
    uint16_t  width;
    uint8_t   unk1;
    uint8_t   frameCount;
    uint32_t  *unkData1; //[frameCount];
    uint16_t  unkData2Count;
    uint32_t  *unkData2; //[unkData2Count];
} Ani_Head;

const char* ani_read(Ani_Head* ani, const char* buf, size_t len) {
    memcpy(ani->name, buf, 4);
    ani->x0         = get_uint16(buf+4);
    ani->y0         = buf[6];
    ani->height     = buf[7];
    ani->width      = get_uint16(buf+8);
    ani->unk1       = buf[10];
    ani->frameCount = buf[11];
    buf += 4+3+3+2;

    ani->unkData1 = (uint32_t*)malloc(ani->frameCount * sizeof(uint32_t));
    memcpy(ani->unkData1, buf, ani->frameCount);
    buf += ani->frameCount * sizeof(uint32_t);
    
    ani->unkData2Count = get_uint16(buf);
    ani->unkData2 = (uint32_t*)malloc(ani->unkData2Count * sizeof(uint32_t));
    memcpy(ani->unkData2, buf+2, ani->unkData2Count);
    buf += 2 + ani->unkData2Count * sizeof(uint32_t);

    printf("Sequence %c%c%c%c has %d frames of %dx%d at %d,%d: %d\n",
	   ani->name[0], ani->name[1], ani->name[2], ani->name[3],
	   ani->frameCount, ani->width, ani->height, ani->x0, ani->y0, ani->unkData2Count);
	   
    return buf;
}
void ani_free(Ani_Head* ani) {
    free(ani->unkData1);
    free(ani->unkData2);
}

typedef struct T_BOB_Header {
    char signature[3];           // Signatur = "BOB"
    uint8_t  version;            // Major/Minor Version
    uint8_t  unk1;

    uint8_t  block_count;     // Anzahl der Teilanimationen (ohne Hauptbild)
    uint8_t  unk2[11];           // Unklarer Teil, fast überall 0x0A010000...
    uint8_t  *block_lengths;  //[ani_count];
    Unknown_Head *block; //[ani_count];
    uint32_t unk3[2];

    uint16_t width;
    uint8_t  height;
    uint8_t  ani_count;
    uint32_t *unkData1; //[ani_count]

    Ani_Head *ani_head;//[ani_count];
    // Imagedata
    // Palette
} BOB_Header;

const char* bob_read(BOB_Header* bob, const char* buf, size_t len) {
    const char* buf0 = buf;
    int i,j;
    memcpy(bob->signature, buf, 3);
    bob->version         = buf[3];
    bob->unk1            = buf[4];
    bob->block_count     = buf[5];
    memcpy(bob->unk2,      buf+6, 11);
    bob->block_lengths = malloc(bob->block_count);
    memcpy(bob->block_lengths, buf+6+11, bob->block_count);
    printf("BOB file has %d sequences\n", bob->block_count);
    if (strncmp(bob->signature, "BOB", 3) != 0) {
	printf("File has no BOB signature, not supported yet.\n");
	exit(1);
    }
    buf += 6 + 11 + bob->block_count;
    // Unknown Header
    bob->block = (Unknown_Head*)malloc(bob->block_count * sizeof(Unknown_Head));
    for (i=0;  i < bob->block_count;  i++) {
	buf = block_read(&(bob->block[i]), buf, len, bob->block_lengths[i]);
    }
    
    // Rest des BOB-Headers
    printf("position is now 0x%02x\n", buf-buf0);
    memcpy(bob->unk3, buf, 8);
    bob->width     = get_uint16(buf + 8);
    bob->height    = buf[10];
    bob->ani_count = buf[11];
    bob->unkData1 = malloc(bob->ani_count * sizeof(uint32_t));
    memcpy(bob->unkData1, buf+8+2+1+1, bob->ani_count * sizeof(uint32_t));
    printf("BOB has %d animations, global size %dx%d.\n",
	   bob->ani_count, bob->width, bob->height);
    buf += 8+2+1+1 + 4*bob->ani_count;

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

ImageSet* process_bob(const char *buf, size_t len) {
    ImageSet* img;
    BOB_Header* bob;
    bob = (BOB_Header*)malloc(sizeof(BOB_Header));
    buf = bob_read(bob, buf, len);
    
    // Übertragen der Daten in die img-Struktur
    img = (ImageSet*)malloc(sizeof(ImageSet));
    img->globalWidth  = bob->width;
    img->globalHeight = bob->height;
    img->frameCount   = bob->ani_count;
    img->frames       = (AnimFrame**)malloc(img->frameCount * sizeof(AnimFrame*));
    for (int i = 0; i<img->frameCount; i++) {
	img->frames[i] = (AnimFrame*)malloc(sizeof(AnimFrame));
    }
    /*TODO
    for (i = 0; i < img->frameCount; i++) {
	AnimFrame* frame = img->frames[i];
	
    }
    */
    fprintf(stderr, "Error: only header reading is supported for BOB (yet).\n");
    
    return NULL;
}

int dump_bob(ImageSet* img, char* prefix) {
    // TODO
    return 1;
}
