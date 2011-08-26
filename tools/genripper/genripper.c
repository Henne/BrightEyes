/**
 * Project:	BrightEyes
 * Author:	HenneNWH
 * License:	GPLv3
 * File:	genripper.c
 * Purpose:	rips hardcoded tables from GEN.EXE and saves them as c-files
 *
 * Warning:	This program will only work correct on Little Endian machines.
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

	unsigned short o_reqs;		/* offset of required attributes */

	unsigned short o_skills;	/* offset of the skills */

	unsigned short o_spells;	/* offset of the spells */

	unsigned short o_le;		/* offset of initial LE values */

	unsigned short o_ae;		/* offset of initial AE values */

	unsigned short o_height_range;	/* offset of height_range */

	unsigned short o_weight;	/* offset of weight modificators */

	unsigned short o_mrmod;		/* offset of MR modificators */

	unsigned short o_skill_incs;	/* offset of skill incs */

	unsigned short o_spell_incs;	/* offset of spell incs */

	unsigned short o_conv_incs;	/* offset of convertable incs */

	unsigned short o_school_mod;	/* offset of school modifications */

	unsigned short o_autoskills;	/* offset of autoskills */

	unsigned short o_autospells;	/* offset of autospells */

	unsigned short o_chr_lookup;	/* offset of character lookup tables */
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
		.o_reqs = 0x309,
		.o_skills = 0x371,
		.o_spells = 0x615,
		.o_le = 0x819,
		.o_ae = 0x833,
		.o_height_range = 0x84d,
		.o_weight = 0x867,
		.o_mrmod = 0x874,
		.o_skill_incs = 0x9bf,
		.o_spell_incs = 0x9cb,
		.o_conv_incs = 0x9d1,
		.o_school_mod = 0x9d7,
		.o_autoskills = 0xadc,
		.o_autospells = 0xd66,
		.o_chr_lookup = 0x1a99,
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
		.o_reqs = 0x309,
		.o_skills = 0x371,
		.o_spells = 0x615,
		.o_le = 0x819,
		.o_ae = 0x833,
		.o_height_range = 0x84d,
		.o_weight = 0x867,
		.o_mrmod = 0x874,
		.o_skill_incs = 0x9bf,
		.o_spell_incs = 0x9cb,
		.o_conv_incs = 0x9d1,
		.o_school_mod = 0x9d7,
		.o_autoskills = 0xadc,
		.o_autospells = 0xd66,
		.o_chr_lookup = 0x1a99,
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
		.o_reqs = 0x30b,
		.o_skills = 0x373,
		.o_spells = 0x617,
		.o_le = 0x81b,
		.o_ae = 0x835,
		.o_height_range = 0x84f,
		.o_weight = 0x869,
		.o_mrmod = 0x876,
		.o_skill_incs = 0x9c1,
		.o_spell_incs = 0x9cd,
		.o_conv_incs = 0x9d3,
		.o_school_mod = 0x9d9,
		.o_autoskills = 0xade,
		.o_autospells = 0xd68,
		.o_chr_lookup = 0x1a9b,
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
		.o_reqs = 0x3cf,
		.o_skills = 0x437,
		.o_spells = 0x6db,
		.o_le = 0x8df,
		.o_ae = 0x8f9,
		.o_height_range = 0x913,
		.o_weight = 0x92d,
		.o_mrmod = 0x93a,
		.o_skill_incs = 0xa85,
		.o_spell_incs = 0xa91,
		.o_conv_incs = 0xa97,
		.o_school_mod = 0xa9d,
		.o_autoskills = 0xba2,
		.o_autospells = 0xe2c,
		.o_chr_lookup = 0x1b85,
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
		.o_reqs = 0x30b,
		.o_skills = 0x373,
		.o_spells = 0x617,
		.o_le = 0x81b,
		.o_ae = 0x835,
		.o_height_range = 0x84f,
		.o_weight = 0x869,
		.o_mrmod = 0x876,
		.o_skill_incs = 0x9c1,
		.o_spell_incs = 0x9cd,
		.o_conv_incs = 0x9d3,
		.o_school_mod = 0x9d9,
		.o_autoskills = 0xade,
		.o_autospells = 0xd68,
		.o_chr_lookup = 0x1ac3,
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

	fprintf(fd, "struct struct_reqs {\n");
	fprintf(fd, "\tunsigned char attrib, requirement;\n};\n");

	fprintf(fd, "static const struct_reqs reqs[13][4] = {\n");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "\t{");
		for (j = 0; j < 4; j++) {
			fprintf(fd, " {%u, 0x%x},",
					ds[info->o_reqs + i * 8 + j * 2],
					(unsigned char)ds[info->o_reqs + i * 8 + j * 2 + 1]);
		}
		fprintf(fd, " },\n");
	}
	fprintf(fd, "};\n");
	extracted += 13 * 4 * 2;


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


	fprintf(fd, "static const unsigned short init_le[13] = {\n\t");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "%d, ", (unsigned short)ds[info->o_le + i * 2]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 13 * 2;

	fprintf(fd, "static const unsigned short init_ae[13] = {\n\t");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "%d, ", (unsigned short)ds[info->o_ae + i * 2]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 13 * 2;

	fprintf(fd, "struct minmax {\n\tunsigned char min, max;\n};\n");
	fprintf(fd, "static const struct minmax height_range[13] = {\n");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "\t{%d, %d},\n",
			(unsigned char)ds[info->o_height_range + i * 2],
			(unsigned char)ds[info->o_height_range + i * 2 + 1]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 13 * 2;

	fprintf(fd, "static const unsigned short weight_mod[13] = {\n\t");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "%d, ", (unsigned char)ds[info->o_weight + i]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 13;

	fprintf(fd, "static const signed char mr_mod[13] = {\n\t");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "%d, ", (signed char)ds[info->o_mrmod + i]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 13;

	fprintf(fd, "static const signed char initial_skill_incs[12] = {\n\t");
	for (i = 0; i < 12; i++) {
		fprintf(fd, "%d, ", (unsigned char)ds[info->o_skill_incs + i]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 12;

	fprintf(fd, "static const signed char initial_spell_incs[6] = {\n\t");
	for (i = 0; i < 6; i++) {
		fprintf(fd, "%d, ", (unsigned char)ds[info->o_spell_incs + i]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 6;

	fprintf(fd, "static const signed char initial_conv_incs[6] = {\n\t");
	for (i = 0; i < 6; i++) {
		fprintf(fd, "%d, ", (unsigned char)ds[info->o_conv_incs + i]);
	}
	fprintf(fd, "\n};\n\n");
	extracted += 6;


	fprintf(fd, "struct struct_house_mod {\n");
	fprintf(fd, "\tsigned char nr;\n");
	fprintf(fd, "\tsigned short spells[7], mod[7];\n};\n\n");

	fprintf(fd, "static const struct struct_house_mod house_mod[9] = {\n");
	for (i = 0; i < 9; i++) {
		fprintf(fd, "\t{%d, {",
			(signed char)ds[info->o_school_mod + i * 29]);

		for (j = 0; j < 7; j++)
			fprintf(fd, "0x%x, ",
			(signed short)ds[info->o_school_mod + i * 29 + 1 + j * 2]);
		fprintf(fd, "}, {");

		for (j = 0; j < 7; j++)
			fprintf(fd, "%d, ",
			(signed short)ds[info->o_school_mod + i * 29 + 15 + j * 2]);

		fprintf(fd, "} },\n");
	}
	fprintf(fd, "\n};\n\n");
	extracted += 9 * 29;

	fprintf(fd, "static const unsigned short autoskills[13][25] = {\n");
	for (i = 0; i < 13; i++) {
		fprintf(fd, "\t{");
		for (j = 0; j < 25; j++)
			fprintf(fd, "%d, ",
				(unsigned short)ds[info->o_autoskills + (i * 25 + j) * 2]);
		fprintf(fd, "},\n");
	}
	fprintf(fd, "\n};\n");
	extracted += 13 * 25 * 2;

	fprintf(fd, "static unsigned short autospells[6][45] = {\n");
	for (i = 0; i < 6; i++) {
		fprintf(fd, "\t{");
		for (j = 0; j < 45; j++) {
			if (j > 0 && j % 8 == 0)
				fprintf(fd, "\n\t");
			fprintf(fd, "%d, ",
				(unsigned short)ds[info->o_autospells + (i * 45 + j) * 2]);
		}
		fprintf(fd, "},\n");
	}
	fprintf(fd, "\n};\n");
	extracted += 6 * 45 * 2;

	fprintf(fd, "struct struct_chr_lookup {\n");
	fprintf(fd, "\tsigned char chr, idx, width;\n};\n\n");

	fprintf(fd, "static const struct struct_chr_lookup chr_lookup[74] = {\n");
	for (i = 0; i < 74; i++) {
		fprintf(fd, "\t{0x%x, %d, %d},\n",
			(unsigned char)ds[info->o_chr_lookup + i * 3],
			(signed char)ds[info->o_chr_lookup + i * 3 + 1],
			(signed char)ds[info->o_chr_lookup + i * 3 + 2]);
	}
	fprintf(fd, "};\n\n");
	extracted += 73 * 3;

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
