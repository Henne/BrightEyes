/**
 * Project:	BrightEyes
 * Author:	HenneNWH
 * File:	genripper.c
 * Purpose:	rips hardcoded tables from GEN.EXE and saves them as c-files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct struct_exe_info {
	char version[6];		/* version string */
	unsigned long flen;		/* filelength */
	unsigned short datseg;		/* adress of the datasegment */
	unsigned short ds_len;		/* length of datseg */

	unsigned short o_spelltab;	/* offset of the spelltables */

	unsigned short o_housetab;	/* offset of the spelltables */

	unsigned short o_schooltab;	/* offset of the spelltables */

	unsigned short o_skills;	/* offset of the skills */

	unsigned short o_spells;	/* offset of the spells */
};

#define VERSIONS (5)

static const struct struct_exe_info *info = NULL;

static const struct struct_exe_info exe_info[VERSIONS] = {
	/* V1.01 */
	{
		.version = "V1.01",
		.flen = 57352,
		.datseg = 0xb3e,
		.ds_len = 11304,
		.o_spelltab = 0x92,
		.o_housetab = 0x245,
		.o_schooltab = 0x2e5,
		.o_skills = 0x371,
		.o_spells = 0x615,
	},
	/* V1.03 */
	{
		.version = "V1.03",
		.flen = 57384,
		.datseg = 0xb40,
		.ds_len = 11304,
		.o_spelltab = 0x92,
		.o_housetab = 0x245,
		.o_schooltab = 0x2e5,
		.o_skills = 0x371,
		.o_spells = 0x615,
	},
	/* V1.04 */
	{
		.version = "V1.04",
		.flen = 57854,
		.datseg = 0xb59,
		.ds_len = 11374,
		.o_spelltab = 0x94,
		.o_housetab = 0x247,
		.o_schooltab = 0x2e7,
		.o_skills = 0x373,
		.o_spells = 0x617,
	},
	/* V1.05 */
	{
		.version = "V1.05",
		.flen = 74112,
		.datseg = 0xcc7,
		.ds_len = 21776,
		.o_spelltab = 0x158,
		.o_housetab = 0x30b,
		.o_schooltab = 0x3ab,
		.o_skills = 0x437,
		.o_spells = 0x6db,
	},
	/* V3.00 */
	{
		.version = "V3.00",
		.flen = 61012,
		.datseg = 0xc0a,
		.ds_len = 11700,
		.o_spelltab = 0x94,
		.o_housetab = 0x247,
		.o_schooltab = 0x2e7,
		.o_skills = 0x373,
		.o_spells = 0x617,
	}
};

static long extracted = 0;

static const char usage_string[] = "\
genripper - rips hardcoded tables from GEN.EXE\n\n\
genripper <filename>\n";

static void usage()
{
	fprintf(stderr, "%s", usage_string);
}

static char *get_datseg(char *fname)
{
	FILE *fd;
	char *buf;
	long len;
	long cs_start;
	long datseg;
	long tmp;
	long i;

	fd = fopen(fname, "r");

	if (fd == NULL) {
		fprintf(stderr, "Error: %s cannot be opened\n", fname);
		return NULL;
	}

	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	/* check the length of the file */
	if ((len > 100000) || (len < 50000)) {

		fprintf(stderr,
			"Error: %s is not in the range to be a GEN.EXE\n",
			fname);

		fclose(fd);
		return NULL;
	}

	/* allocate memory to buffer GEN.EXE */
	buf = calloc(len, sizeof(char));
	if (buf == NULL) {
		fprintf(stderr, "Error: Out of memory\n");
		fclose(fd);
		return NULL;
	}

	/* read the complete EXE file */
	tmp = fread(buf, 1, len, fd);
	if (tmp != len) {
		fprintf(stderr, "Error: Failed to read from %s\n", fname);
		fclose(fd);
	}

	/* close GEN.EXE */
	fclose(fd);

	/* check for MZ signature */
	if (*(unsigned short *)buf != 0x5a4d) {
		fprintf(stderr, "Error: %s is not a valid EXE file\n", fname);
		free(buf);
		return NULL;
	}

	/* get the start from the codesegment from the exe header */
	cs_start = *(unsigned short*)(buf + 8) << 4;

	/* get the datasegment adress from the code */
	datseg = *(unsigned short*)(buf + cs_start + 1);

	for (i = 0; i < VERSIONS; i++) {

		if (exe_info[i].flen != len)
			continue;

		if (exe_info[i].datseg != datseg)
			continue;

		info = &exe_info[i];
	}

	/* check if info is set */
	if (info == NULL) {
		fprintf(stderr, "Error: %s is not a known GEN.EXE\n", fname);
		fprintf(stderr, "Contact a BrightEyes developer about that\n");
		free(buf);
		return NULL;
	}

	if (info->ds_len != len - (datseg << 4)) {
		fprintf(stderr, "Error: %s has wrong ds_length\n", fname);
		free(buf);
		return NULL;
	}

	/* copy the datseg to the begin of the buf */
	memcpy(buf, buf + cs_start + (datseg << 4), info->ds_len);

	/* shrink the allocated memory */
	buf = realloc(buf, info->ds_len);

	return buf;
}

void dump_bin(char *fname, char *ds) {
	FILE *fd;
	char name[25];

	strncpy(name, fname, 12);
	strncat(name, ".bin", 16);

	fd = fopen(name, "w+");
	if (fd == NULL) {
		fprintf(stderr, "Error: Cant write %s\n", name);
		return;
	}

	fwrite(ds, info->ds_len, 1, fd);
	fclose(fd);
}

static const char decl_spells[] =
"struct struct_spelltab {\n\
\tsigned char origin;\n\
\tsigned char att1;\n\
\tsigned char att2;\n\
\tsigned char att3;\n\
\tsigned char cost;\n\
};\n\n\
\
struct struct_schooltab {\n\
\tunsigned short first_spell;\n\
\tunsigned short spells;\n\
};\n\n\
\
";



void dump_spelltab(char *fname, char *ds) {
	FILE *fd;
	char name[25];
	int i;
	short *p;

	strcpy(name, "spelltab_");
	strncat(name, info->version, 6);
	strncat(name, ".h", 3);

	fd = fopen(name, "w+");
	if (fd == NULL) {
		fprintf(stderr, "Error: Cant write %s\n", name);
		return;
	}

	/* print spelltable */
	fprintf(fd, decl_spells);
	fprintf(fd, "static const struct struct_spelltab spelltab[87] = {\n");
	for (i = 0; i < 87; i++) {
		fprintf(fd, "\t{ %d, %d, %d, %d, %d},\n",
			(char)ds[info->o_spelltab + i * 5 + 0],
			(char)ds[info->o_spelltab + i * 5 + 1],
			(char)ds[info->o_spelltab + i * 5 + 2],
			(char)ds[info->o_spelltab + i * 5 + 3],
			(char)ds[info->o_spelltab + i * 5 + 4]);
	}
	fprintf(fd, "};\n\n");
	extracted += 87 * 5;

	/* print house spell arrays */
	p = (signed short*)(ds + info->o_housetab);
	for (i = 0; i < 9; i++) {
		fprintf(fd, "static const short housesp_%d[] = {\t", i);
		do {
			fprintf(fd, " %d,", *p);
		} while (*p++ != -1);

		fprintf(fd, " };\n");
	}
	fprintf(fd, "\n");
	extracted += 86 * 2;

	/* print house spell array */
	fprintf(fd, "static const short *housetab[] = {\n");
	for (i = 0; i < 9; i++)
		fprintf(fd, "\t&housesp_%d,\n", i);
	fprintf(fd, "};\n\n");


	/* print school spell struct */
	p = (signed short*)(ds + info->o_schooltab);
	fprintf(fd, "static const struct_schooltab schooltab = {\n");
	for (i = 0; i < 9; i++) {
		fprintf(fd, "\t{%d, %d},\n", p[i * 2], p[i * 2 + 1]);
	}
	fprintf(fd, "};\n\n");
	extracted += 9 * 4;

	fclose(fd);
}

static void dump_inittab(char *fname, char *ds) {
	FILE *fd;
	char name[25];
	int i, j;

	strcpy(name, "inittab_");
	strncat(name, info->version, 6);
	strncat(name, ".h", 3);

	fd = fopen(name, "w+");
	if (fd == NULL) {
		fprintf(stderr, "Error: Cant open for write %s\n", name);
		return;
	}

	fprintf(fd, "static const signed char skills[13][52] = {\n");
	fprintf(fd, "\t{},\n");
	for (i = 1; i < 13; i++) {
		fprintf(fd, "\t{");
		for (j = 0; j < 52; j++) {
			if (j % 12 == 11)
				fprintf(fd, "%d,\n\t",
					(signed char)ds[info->o_skills + i * 52 + j]);
			else
				fprintf(fd, "%d, ",
					(signed char)ds[info->o_skills + i * 52 + j]);
		}
		fprintf(fd, "},\n");
	}
	fprintf(fd, "};\n\n");
	extracted += 52 * 13;

	fprintf(fd, "static const signed char spells[6][86] = {\n");
	for (i = 0; i < 6; i++) {
		fprintf(fd, "\t{");

		fprintf(fd, "\n\t\t/* DUMMY */\n\t\t");
		fprintf(fd, "%d, ",
			(signed char)ds[info->o_spells + i * 86]);

		fprintf(fd, "\n\t\t/* Antimagie / Dispel */\n\t\t");
		for (j = 1; j < 6; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Beherrschung / Domination */\n\t\t");
		for (j = 6; j < 18; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Dämonologie / Demonology */\n\t\t");
		for (j = 18; j < 24; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Elemente / Elements */\n\t\t");
		for (j = 24; j < 27; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Bewegung /Movement */\n\t\t");
		for (j = 27; j < 33; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Heilung / Healing */\n\t\t");
		for (j = 33; j < 38; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Hellsicht / Clairvoyance */\n\t\t");
		for (j = 38; j < 45; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Illusion */\n\t\t");
		for (j = 45; j < 49; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Kampf / Fight */\n\t\t");
		for (j = 49; j < 58; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Verständigung / Communication */\n\t\t");
		for (j = 58; j < 60; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Verwandlung / Transformation */\n\t\t");
		for (j = 60; j < 76; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "\n\t\t/* Veränderung / Transmutation */\n\t\t");
		for (j = 76; j < 86; j++)
			fprintf(fd, "%d, ",
				(signed char)ds[info->o_spells + i * 86 + j]);

		fprintf(fd, "},\n");
	}
	fprintf(fd, "};\n\n");
	extracted += 86 * 6;


	fclose(fd);
}

int main(int argc, char **argv)
{
	char *ds;
	long i;

	if (argc != 2) {
		usage();
		return 1;
	}

	ds = get_datseg(argv[1]);

	if (ds == NULL) {
		fprintf(stderr, "Nothing to dump here\n");
		return 1;
	}

	/* dump_bin(argv[1], ds); */
	dump_spelltab(argv[1], ds);
	dump_inittab(argv[1], ds);

	printf("Extracted %ld from %d\n", extracted, info->ds_len);
	free(ds);
	return 0;
}
