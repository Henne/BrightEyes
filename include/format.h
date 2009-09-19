/*
 * Header file for DSA/ROA file formats
 *
 * Author: Nenne_NWH <henne@nachtwindheim.de>
 * Licanse: GPLv3
 *
 */

static inline unsigned short get_ushort(const char* buf) {
	        return (buf[1]<<8 | buf[0]);
}

static inline unsigned int get_uint(const char* buf) {
	        return (buf[3]<<24) | (buf[2]<<16) | (buf[1]<<8) | (buf[0]);
}


struct ace_header {
	char label[4];			/* "ACE\0"	*/
	int version;			/* 1		*/
	unsigned char sequences;	/* 0..250	*/
	unsigned char speed;		/* 0..99	*/
};
