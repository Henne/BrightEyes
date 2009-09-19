/*
 * Header file for DSA/ROA file formats
 *
 * Author: Nenne_NWH <henne@nachtwindheim.de>
 * Licanse: GPLv3
 *
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

struct cel_header {
	int size;			/* Größe der Cels		*/
	short xoffset;			/* Offset im Cel		*/
	short yoffset;
	short width;			/* Größe des Frames im Cel	*/
	short height;
	unsigned char compression;	/* Verwendeter Packer		*/
	unsigned char action;		/* Action Button der Cel	*/
} __attribute__((__packed__));
