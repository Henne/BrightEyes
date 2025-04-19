#include <stdio.h>
#include <stdlib.h>
#include <cstring>

static unsigned char readb(const char *p)
{
	return *(unsigned char*)p;
}

static unsigned short readw(const char *p)
{
	return (((unsigned char)p[0]) | ((unsigned char)p[1] << 8)); 
}

static unsigned long readd(const char *p)
{
	return ((readw((const char*)p)) | (readw((const char*)p + 2) << 16)); 
}

static int check_is_exe(const char *buffer, unsigned long len)
{
	if ((len > 0x1c) && (buffer[0] == 'M') && (buffer[1] == 'Z'))
		return 1;
	return 0;
}

static int compare_exe_stub(const char *orig, const char *rewrite)
{
	int diff = 0;
	for (int i = 0x02; i <= 0x1c; i+=2) {
		if (readw(orig) != readw(rewrite)) {
			diff++;
			fprintf(stdout, "@ EXE-Header:0x%04x O = 0x%04x R = 0x%04x\n", i, readw(orig), readw(rewrite)); 
		}
	}

	if (diff == 0) fprintf(stdout, "EXE-Headers: OK\n");

	return diff;
}

static int compare_exe_stub_reloc_items(const char *orig, const char *rewrite)
{
	int ITEMS[2] = {0, 0};
	int TABLE_START[2] = {0, 0};
	int diff = 0;

	ITEMS[0] = readw(orig + 0x06);
	ITEMS[1] = readw(rewrite + 0x06);
	const int items_max = ITEMS[0] >= ITEMS[1] ? ITEMS[0] : ITEMS[1]; 
	const int longer_table = ITEMS[0] >= ITEMS[1] ? 0 : 1;
	char *marker = (char*)calloc(items_max, sizeof(char));

	TABLE_START[0] = readw(orig + 0x18);
	TABLE_START[1] = readw(rewrite + 0x18);


	if (marker == NULL) {
		fprintf(stderr, "Failed to alloc %d bytes\n", items_max);
		return -1;
	}

	fprintf(stdout, "Reloc items: orig = %d, rewrite = %d\n", ITEMS[0], ITEMS[1]);

	if (ITEMS[0] == ITEMS[1]) {
		// both lengths are identical
		for (int i = 0x00; i < items_max; i++) {
			if (readd(orig + TABLE_START[0] + 4 * i) != readd(rewrite + TABLE_START[1] + 4 * i))
			{
				diff++;
				marker[i] = 1;
			}
		}

		// print only the different ones
		for (int i = 0x00; i < items_max; i++) {
			if (marker[i] == 1) {
				fprintf(stdout, "@ EXE-Reloc[0x%04x]@0x%04x O = 0x%08x R = 0x%08x\n",
					i,
					TABLE_START[0] + 4 * i,
				       	readd(orig + TABLE_START[0] + 4 * i),
				       	readd(rewrite + TABLE_START[1] + 4 * i));
			}
		}
	} else {
		// both lengths are different
		int I[2] = {0, 0};
#if 0
		fprintf(stdout, "Reloc Items orig Start\n");
		for (I[0] = 0; I[0] < ITEMS[0]; I[0]++) {
			fprintf(stdout, "0x%04x\n", readd(orig + TABLE_START[0] + 4 * I[0]));
		}
		fprintf(stdout, "Reloc Items orig End\n");

		fprintf(stdout, "Reloc Items rewrite Start\n");
		for (I[1] = 0; I[1] < ITEMS[1]; I[1]++) {
			fprintf(stdout, "0x%04x\n", readd(rewrite + TABLE_START[1] + 4 * I[1]));
		}
		fprintf(stdout, "Reloc Items rewrite End\n");

		while ((I[0] < ITEMS[0]) && (I[1] < ITEMS[1])) {
			I[0]++;
			I[1]++;
		}
#endif
	}


	if (diff == 0) fprintf(stdout, "EXE-Relocs: OK\n");

	free(marker);

	return diff;
}
static const char borsig[] = "Borland C++ -";

# if 0
/* GEN V1.05de related */
static void scan_got_ch_bonus(const char *orig, const char *rewrite)
{
	/* Search in main-seg for BSS variable FLEN */
	const unsigned char sig[] = {0x55, 0x8b, 0xec, 0x56, 0x33, 0xc0, 0xa3};
	unsigned short ADDR[2] = {0, 0};
	unsigned short CODE[2] = {0, 0};

	for (int i = 0; i < 0xffff; i++) {
		if (!memcmp(orig + i, sig, sizeof(sig))) {
			CODE[0] = i;
			ADDR[0] = readw(orig + i + sizeof(sig));
			break;
		}
	}
	for (int i = 0; i < 0xffff; i++) {
		if (!memcmp(rewrite + i, sig, sizeof(sig))) {
			CODE[1] = i;
			ADDR[1] = readw(rewrite + i + sizeof(sig));
			break;
		}
	}
	if (ADDR[0] == 0 || ADDR[1] == 0) return;

	fprintf(stdout, "[GOT_CH_BONUS] : orig = 0x%04x@0x%04x rewrite = 0x%04x@0x%04x\n", ADDR[0], CODE[0], ADDR[1], CODE[1]);
}

static void scan_flen(const char *orig, const char *rewrite)
{
	/* Search in main-seg for BSS variable FLEN */
	const unsigned char sig[] = {0x26, 0x2b, 0x54, 0x0c, 0x26, 0x1b, 0x44, 0x0e, 0xa3};
	unsigned short CODE[2] = {0, 0};
	unsigned short ADDR[2] = {0, 0};

	for (int i = 0; i < 0xffff; i++) {
		if (!memcmp(orig + i, sig, sizeof(sig))) {
			CODE[0] = i;
			ADDR[0] = readw(orig + i + sizeof(sig));
			break;
		}
	}
	for (int i = 0; i < 0xffff; i++) {
		if (!memcmp(rewrite + i, sig, sizeof(sig))) {
			CODE[1] = i;
			ADDR[1] = readw(rewrite + i + sizeof(sig));
			break;
		}
	}
	if (ADDR[0] == 0 || ADDR[1] == 0) return;

	fprintf(stdout, "[FLEN] : orig = 0x%04x@0x%04x rewrite = 0x%04x@0x%04x\n", ADDR[0] - 2, CODE[0], ADDR[1] - 2, CODE[1]);
}
#endif

static void disjoin_file(const char *name, const char *buffer, unsigned long len, unsigned long off_cs, unsigned long off_ds)
{
	if ((buffer != NULL) && (off_cs < off_ds) && (off_ds < len)) {
		FILE *fd;
		char fname[50];

		// EXE Header
		sprintf(fname, "%s_EXE.bin", name);
		fd = fopen(fname, "w");
		if (fd != NULL) {
			fwrite(buffer, 1, off_cs, fd);
			fclose(fd);
		}

		// CODE
		sprintf(fname, "%s_CODE.bin", name);
		fd = fopen(fname, "w");
		if (fd != NULL) {
			fwrite(buffer + off_cs, 1, off_ds - off_cs, fd);
			fclose(fd);
		}

		// DATA
		sprintf(fname, "%s_DATA.bin", name);
		fd = fopen(fname, "w");
		if (fd != NULL) {
			fwrite(buffer + off_ds, 1, len - off_ds, fd);
			fclose(fd);
		}
	}
}

static void compare(const char *orig, unsigned long len_orig, const char* rewrite, unsigned long len_rewrite)
{
	unsigned long start_cs[2] = {0, 0};
	unsigned short DS[2] = {0, 0};
	unsigned short BSS[2] = {0, 0};
	unsigned short DS_LEN[2] = {0, 0};
	unsigned long start_ds[2] = {0, 0};
	unsigned short MAIN_SEG[2] = {0, 0};
	unsigned short MAIN_OFF[2] = {0, 0};

	unsigned long i;

	if (!check_is_exe(orig, len_orig)) {
		fprintf(stderr, "<Original> is no EXE-file\n");
		return;
	}
	if (!check_is_exe(rewrite, len_rewrite)) {
		fprintf(stderr, "<Rewrite> is no EXE-file\n");
		return;
	}

	/* compare EXE-headers */
	if (compare_exe_stub(orig, rewrite) != 0) {
		fprintf(stderr, "ERROR: EXE-Headers differ => give up\n");
		return;
	}
	/* compare relocation items */
	if (compare_exe_stub_reloc_items(orig, rewrite) != 0) {
		fprintf(stderr, "ERROR: EXE-Relocs differ => proceed\n");
	}

	/* From here assumptions from BCC 3.1 generated files with -mlarge are made. */

	start_cs[0] = 16 * readw(orig + 0x08);
       	start_cs[1] = 16 * readw(rewrite + 0x08);
	fprintf(stdout, "Start CS: orig = 0x%04x, rewrite = 0x%04x\n", start_cs[0], start_cs[1]);

	if (readb(orig + start_cs[0]) == 0xba) {
		DS[0] = readw(orig + start_cs[0] + 1);

		if (readb(orig + start_cs[0] + 0xb3) == 0xbf) {
			BSS[0] = readw(orig + start_cs[0] + 0xb3 + 1);
		} else {
			fprintf(stderr, "ERROR: <Original> doesn't have with 'mov di, BSS' => give up\n");
			return;
		}
		if (readb(orig + start_cs[0] + 0xb6) == 0xb9) {
			DS_LEN[0] = readw(orig + start_cs[0] + 0xb6 + 1);
		} else {
			fprintf(stderr, "ERROR: <Original> doesn't have with 'mov cx, DS_LEN' => give up\n");
			return;
		}
	} else {
		fprintf(stderr, "ERROR: <Original> doesn't start with 'mov dx, DSEG' => give up\n");
		return;
	}

	if (readb(rewrite + start_cs[1]) == 0xba) {
		DS[1] = readw(rewrite + start_cs[1] + 1);

		if (readb(rewrite + start_cs[1] + 0xb3) == 0xbf) {
			BSS[1] = readw(rewrite + start_cs[1] + 0xb3 + 1);
		} else {
			fprintf(stderr, "ERROR: <Rewrite> doesn't have with 'mov di, BSS' => give up\n");
			return;
		}
		if (readb(rewrite + start_cs[1] + 0xb6) == 0xb9) {
			DS_LEN[1] = readw(rewrite + start_cs[1] + 0xb6 + 1);
		} else {
			fprintf(stderr, "ERROR: <Original> doesn't have with 'mov cx, DS_LEN' => give up\n");
			return;
		}
	} else {
		fprintf(stderr, "ERROR: <Rewrite> doesn't start with 'mov dx, DSEG' => give up\n");
		return;
	}

	fprintf(stdout, "Add DS:   orig = 0x%04x, rewrite = 0x%04x\n", DS[0], DS[1]);
	fprintf(stdout, "DS:BSS:   orig = 0x%04x, rewrite = 0x%04x\n", BSS[0], BSS[1]);
	fprintf(stdout, "DS_LEN:   orig = 0x%04x, rewrite = 0x%04x\n", DS_LEN[0], DS_LEN[1]);

	start_ds[0] = (DS[0] << 4) + start_cs[0];
	start_ds[1] = (DS[1] << 4) + start_cs[1];

	if ((orig[start_ds[0] + 4] != 'B') || (orig[start_ds[0] + 5] != 'o')) {
		fprintf(stderr, "ERROR: <Original> does not contain Borland Signature\n");
		return;
	}

	if ((rewrite[start_ds[1] + 4] != 'B') || (rewrite[start_ds[1] + 5] != 'o')) {
		fprintf(stderr, "ERROR: <Rewrite> does not contain Borland Signature\n");
		return;
	}

	fprintf(stdout, "Start DS: orig = 0x%04x, rewrite = 0x%04x\n", start_ds[0], start_ds[1]);

	int ratio = 10000 * (DS_LEN[1]) / (DS_LEN[0]);
	fprintf(stdout, "Len   DS: orig = 0x%04x, rewrite = 0x%04x ratio = %02d.%02d\n",
			DS_LEN[0], DS_LEN[1], ratio / 100, ratio % 100);

	unsigned short max_len = (DS_LEN[0] < DS_LEN[1] ? DS_LEN[0] : DS_LEN[1]);

	fprintf(stdout, "max_len = %d/0x%04x bytes\n", max_len, max_len);

	/* get adress of main() functions */
	if (readb(orig + start_cs[0] + 0x14c) == 0x9a) {
		MAIN_OFF[0] = readw(orig + start_cs[0] + 0x14c + 1);
		MAIN_SEG[0] = readw(orig + start_cs[0] + 0x14c + 3);
	} else {
		fprintf(stderr, "ERROR: <Original> failed to detect main() => proceed\n");
	}
	if (readb(rewrite + start_cs[1] + 0x14c) == 0x9a) {
		MAIN_OFF[1] = readw(rewrite + start_cs[1] + 0x14c + 1);
		MAIN_SEG[1] = readw(rewrite + start_cs[1] + 0x14c + 3);
	} else {
		fprintf(stderr, "ERROR: <Rewrite> failed to detect main() => proceed\n");
	}
	fprintf(stdout, "MAIN() : orig = 0x%04x:0x%04x rewrite = 0x%04x:0x%04x\n",
			MAIN_SEG[0], MAIN_OFF[0], MAIN_SEG[1], MAIN_OFF[0]);


	/* compare initialized DATA only, BSS content is either 0 or not existent in the file */
	if (BSS[0] == BSS[1]) {
		/* Both areas have the same length, that's good */
		int diffs = 0;
		int diff_offset = -1;
		for (i = 0; (i < BSS[0]) && (diffs < 20); i++) {
			if (orig[start_ds[0] + i] != rewrite[start_ds[1] + i]) {
				fprintf(stdout, "@ DS:0x%04x O = 0x%02x R = 0x%02x\n", i,
				       	(unsigned char)orig[start_ds[0] + i],
					(unsigned char)rewrite[start_ds[1] + i]);
				diffs++;
				if (diff_offset == -1) diff_offset = i;
			}
		}
		if (diffs == 0) {
			fprintf(stdout, "DS:DATA: OK\n");
			/* check EOF or 0's in orig */
			/* check EOF or 0's in rewrite */
		} else {
		}
	} else {
		int diffs = 0;
		for (i = 0; (i < max_len) && (diffs < 20); i++) {
			if (orig[start_ds[0] + i] != rewrite[start_ds[1] + i]) {
				fprintf(stdout, "@ DS:0x%04x O = 0x%02x R = 0x%02x\n", i,
				       	(unsigned char)orig[start_ds[0] + i],
					(unsigned char)rewrite[start_ds[1] + i]);
				diffs++;
			}
		}
	}

	disjoin_file("ORIG", orig, len_orig, start_cs[0], start_ds[0]);
	disjoin_file("REWR", rewrite, len_rewrite, start_cs[1], start_ds[1]);

#if 0
	/* GEN V1.05de related */
	scan_got_ch_bonus(orig + start_cs[0] + 16 * MAIN_SEG[0] , rewrite + start_cs[1] + 16 * MAIN_SEG[1]);
	scan_flen(orig + start_cs[0] + 16 * MAIN_SEG[0] , rewrite + start_cs[1] + 16 * MAIN_SEG[1]);
#endif
}

int main(int argc, char **argv)
{
	FILE *fd;
	char *orig;
	char *rewrite;
	unsigned long len_orig = 0;
	unsigned long len_rewrite = 0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <Original> <Rewrite>\n", argv[0]);
		return 0;
	}

	fd = fopen(argv[1], "r");
	if (fd == NULL) {
		fprintf(stderr, "ERROR: failed to open %s\n", argv[1]);
		return -1;
	} else {
		fseek(fd, 0, SEEK_END);
		len_orig = ftell(fd);
		fseek(fd, 0, SEEK_SET);

		orig = (char*)calloc(len_orig, 1);
		if (orig == NULL) {
			fprintf(stderr, "ERROR: failed to allocate %ld bytes\n");
			fclose(fd);
			return -1;
		}
	}
	fread(orig, 1, len_orig, fd);
	fclose(fd);

	fd = fopen(argv[2], "r");
	if (fd == NULL) {
		fprintf(stderr, "ERROR: failed to open %s\n", argv[2]);
		return -1;
	} else {
		fseek(fd, 0, SEEK_END);
		len_rewrite = ftell(fd);
		fseek(fd, 0, SEEK_SET);

		rewrite = (char*)calloc(len_rewrite, 1);
		if (rewrite == NULL) {
			fprintf(stderr, "ERROR: failed to allocate %ld bytes\n");
			fclose(fd);
			return -1;
		}
	}
	fread(rewrite, 1, len_rewrite, fd);
	fclose(fd);

	compare(orig, len_orig, rewrite, len_rewrite);

	free(orig);
	free(rewrite);

	return 0;
}


