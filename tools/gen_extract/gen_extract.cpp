/**
 *  Project:	BrightEyes
 *  Author:	Henne
 *  License:	GPLv3
 *  File:	gen_extract.cpp
 *  Purpose:	detect and extracts DSAGEN.DAT archives
 *
 *  Warning:    This program will only work on Little Endian machines.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <cstring>

static const int FLEN_DE_DISK = 634785;
static const int FLEN_DE_CD   = 663221;
static const int FLEN_EN_DISK = 671236;

static int ver_dsagen = -1;

static long readd(const unsigned char *p)
{
	return (p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
}

void extract_dsagen(const unsigned char *buffer, long flen)
{
	int exp_entries = -1;
	int data_start = -1;
	long sum = 0;
	
	// check if file content is valid
	if (flen == FLEN_DE_DISK) {
		exp_entries = 34;
		data_start = 0x2f0;
	} else if ((flen == FLEN_DE_CD) || (flen == FLEN_EN_DISK)) {
		exp_entries = 38;
		data_start = 0x330;
	} else {
		int offset;
		fprintf(stderr, "WARNING: file version unknown => autodetect\n");
		exp_entries = 0;
		data_start = readd(buffer + 0xc);
		if (data_start >= flen) {
			fprintf(stderr, "ERROR: file is too short => GIVE UP!\n");
			return;
		}
		while ((offset = readd(buffer + 16 * exp_entries + 0xc)) != 0) {
			if (offset >= flen) {
				fprintf(stderr, "ERROR: file containes malformed offsets => GIVE UP!\n");
				return;
			}
			exp_entries++;
		}
		fprintf(stdout, "Seems valid and contains %d entries, data starts at 0x%08d\n", exp_entries, data_start);
	}

	char path[20];

	if (ver_dsagen == 1) {
		strncpy(path, "DE_DISK", 10);
	} else if (ver_dsagen == 2) {
		strncpy(path, "DE_CD", 10);
	} else if (ver_dsagen == 3) {
		strncpy(path, "EN_DISK", 10);
	} else {
		strncpy(path, "UNKNOWN", 10);
	}

	mkdir(path, 0777);
	strcat(path, "/");

	// calculate the start of the first data entry
	sum = 16 * (readd(buffer + 0xc) / 16);

	for (int i = 0; i < exp_entries; i++) {
		FILE *fd;
		char fname[40];
		char fpath[80];
		long offset;
		long offset_next;
		long len;
		
		// preapare output filename in order of the archive file
		strncpy(fname, (char*)buffer + 16 * i, 12);
		fname[12] = 0;
		sprintf(fpath, "%s%02d_%s", path, i, fname);

		offset = readd(buffer + 16 * i + 0x0c);
		offset_next = readd(buffer + 16 * (i + 1) + 0x0c);
		if (offset_next == 0) offset_next = flen;
		// check for the last entry
		len = offset_next - offset;

		sum += len;
		fprintf(stderr, "dumping %s offset = 0x%06lx, length = %ld\n", fpath, offset, len);

		fd = fopen(fpath, "w");
		if (fd == NULL) {
			fprintf(stderr, "ERROR: failed to open %s => GIVE UP!\n", fpath);
			return;
		}
		fwrite(buffer + offset, 1, len, fd);
		fclose(fd);
	}

	if (sum != flen) {
		fprintf(stderr, "ERROR: flen = %ld, but %ld were only processed\n", flen, sum);
	} else {
		fprintf(stderr, "OK\n");
	}
}

int main(int argc, char **argv)
{
	FILE *fd = NULL;
	long unsigned flen = 0;
	unsigned char *buffer = NULL;
	
	if (argc <= 1) {
		fprintf(stderr, "Usage %s <DSAGEN.DAT>\n", argv[0]);
		exit(0);
	}
	
	fd = fopen(argv[1], "r");
	if (fd == NULL) {
		fprintf(stderr, "ERROR: failed to open %s\n", argv[1]);
		exit(-1);
	}
	
	fseek(fd, 0, SEEK_END);
	flen = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	fprintf(stderr, "Filelength: %ld\n", flen);

	if (flen == FLEN_DE_DISK) {
		fprintf(stdout, "Detected Version: DE_DISK\n");
		ver_dsagen = 1;
	} else if (flen == FLEN_DE_CD) {
		fprintf(stdout, "Detected Version: DE_CD\n");
		ver_dsagen = 2;
	} else if (flen == FLEN_EN_DISK) {
		fprintf(stdout, "Detected Version: EN_DISK\n");
		ver_dsagen = 3;
	} else {
		fprintf(stdout, "Detected Version: unknown (USE AT OWN RISK)\n");
		ver_dsagen = 0;
	}

	buffer = (unsigned char*)calloc(flen, 1);
	if (buffer == NULL) {
		fprintf(stderr, "ERROR: could not allocate %ld bytes\n", flen);
		fclose(fd);
		exit(-2);
	}
	
	if (flen != fread(buffer, 1, flen, fd)) {
		fprintf(stderr, "ERROR: could not read the whole file\n");
		fclose(fd);
		free(buffer);
		exit(-2);
	} else {
		fclose(fd);
	}
	
	extract_dsagen(buffer, flen);

	free(buffer);
	return 0;

}
