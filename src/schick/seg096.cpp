/*
 *	Rewrite of DSA1 v3.02_de functions of seg096 (text output)
 *	Functions rewritten: 23/23 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg096.cpp
 *
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#include "seg096.h"
#include "seg097.h"

enum{
	GRAMMAR_ARTICLE_ID_DER       = 0,
	GRAMMAR_ARTICLE_ID_DIE       = 1,
	GRAMMAR_ARTICLE_ID_DAS       = 2,
	GRAMMAR_ARTICLE_ID_DES       = 3,
	GRAMMAR_ARTICLE_ID_DEN       = 4,
	GRAMMAR_ARTICLE_ID_DEM       = 5,
	GRAMMAR_ARTICLE_ID_EIN       = 6,
	GRAMMAR_ARTICLE_ID_EINE      = 7,
	GRAMMAR_ARTICLE_ID_EINES     = 8,
	GRAMMAR_ARTICLE_ID_EINER     = 9,
	GRAMMAR_ARTICLE_ID_EINEN     = 10,
	GRAMMAR_ARTICLE_ID_EINE__DUP = 11, // the article "eine" is present twice
	GRAMMAR_ARTICLE_ID_EINEM     = 12,
	GRAMMAR_ARTICLE_ID_NONE      = 13,
	GRAMMAR_ARTICLE_ID_VON       = 14
};

static char g_grammar_article_der[4]       = "DER"; // ds:0xa8d4
static char g_grammar_article_die[4]       = "DIE"; // ds:0xa8d8
static char g_grammar_article_das[4]       = "DAS"; // ds:0xa8dc
static char g_grammar_article_des[4]       = "DES"; // ds:0xa8e0
static char g_grammar_article_den[4]       = "DEN"; // ds:0xa8e4
static char g_grammar_article_dem[4]       = "DEM"; // ds:0xa8e8
static char g_grammar_article_ein[4]       = "EIN"; // ds:0xa8ec
static char g_grammar_article_eine[5]      = "EINE"; // ds:0xa8f0
static char g_grammar_article_eines[6]     = "EINES"; // ds:0xa8f5
static char g_grammar_article_einer[6]     = "EINER"; // ds:0xa8fb
static char g_grammar_article_einen[6]     = "EINEN"; // ds:0xa901
static char g_grammar_article_eine__dup[5] = "EINE"; // ds:0xa907
static char g_grammar_article_einem[6]     = "EINEM"; // ds:0xa90c
static char g_grammar_article_none[1]      = ""; // ds:0xa912
static char g_grammar_article_von[4]       = "VON"; // ds:0xa913
static char* g_grammar_articles_index[15] = {
	g_grammar_article_der,
	g_grammar_article_die,
	g_grammar_article_das,
	g_grammar_article_des,
	g_grammar_article_den,
	g_grammar_article_dem,
	g_grammar_article_ein,
	g_grammar_article_eine,
	g_grammar_article_eines,
	g_grammar_article_einer,
	g_grammar_article_einen,
	g_grammar_article_eine__dup,
	g_grammar_article_einem,
	g_grammar_article_none,
	g_grammar_article_von
}; // ds:0xa917; uint8_t*

static signed int g_grammar_definite_article_table[24] = {
	GRAMMAR_ARTICLE_ID_DER, GRAMMAR_ARTICLE_ID_DIE, GRAMMAR_ARTICLE_ID_DAS, // singular, 1st case
	GRAMMAR_ARTICLE_ID_DES, GRAMMAR_ARTICLE_ID_DER, GRAMMAR_ARTICLE_ID_DES, // singular, 2nd case
	GRAMMAR_ARTICLE_ID_DEN, GRAMMAR_ARTICLE_ID_DIE, GRAMMAR_ARTICLE_ID_DAS, // singular, 4th case
	GRAMMAR_ARTICLE_ID_DEM, GRAMMAR_ARTICLE_ID_DER, GRAMMAR_ARTICLE_ID_DEM, // singular, 3rd case
	GRAMMAR_ARTICLE_ID_DIE, GRAMMAR_ARTICLE_ID_DIE, GRAMMAR_ARTICLE_ID_DIE, // plural, 1st case
	GRAMMAR_ARTICLE_ID_DER, GRAMMAR_ARTICLE_ID_DER, GRAMMAR_ARTICLE_ID_DER, // plural, 2st case
	GRAMMAR_ARTICLE_ID_DIE, GRAMMAR_ARTICLE_ID_DIE, GRAMMAR_ARTICLE_ID_DIE, // plural, 4st case
	GRAMMAR_ARTICLE_ID_DEN, GRAMMAR_ARTICLE_ID_DEN, GRAMMAR_ARTICLE_ID_DEN  // plural, 3st case
}; // ds:0xa953, [8],

static signed int g_grammar_indefinite_article_table[24] = {
	GRAMMAR_ARTICLE_ID_EIN,   GRAMMAR_ARTICLE_ID_EINE,      GRAMMAR_ARTICLE_ID_EIN,   // singular, 1st case
	GRAMMAR_ARTICLE_ID_EINES, GRAMMAR_ARTICLE_ID_EINER,     GRAMMAR_ARTICLE_ID_EINES, // singular, 2nd case
	GRAMMAR_ARTICLE_ID_EINEN, GRAMMAR_ARTICLE_ID_EINE__DUP, GRAMMAR_ARTICLE_ID_EIN,   // singular, 4th case
	GRAMMAR_ARTICLE_ID_EINEM, GRAMMAR_ARTICLE_ID_EINER,     GRAMMAR_ARTICLE_ID_EINEM, // singular, 3rd case
	GRAMMAR_ARTICLE_ID_NONE,  GRAMMAR_ARTICLE_ID_NONE,      GRAMMAR_ARTICLE_ID_NONE,  // plural, 1st case
	GRAMMAR_ARTICLE_ID_NONE,  GRAMMAR_ARTICLE_ID_NONE,      GRAMMAR_ARTICLE_ID_NONE,  // plural, 2st case
	GRAMMAR_ARTICLE_ID_NONE,  GRAMMAR_ARTICLE_ID_NONE,      GRAMMAR_ARTICLE_ID_NONE,  // plural, 4st case
	GRAMMAR_ARTICLE_ID_NONE,  GRAMMAR_ARTICLE_ID_NONE,      GRAMMAR_ARTICLE_ID_NONE   // plural, 3st case
}; // ds:0xa983, [8], by case, then by gender

static signed int g_grammar_omitted_article_table[24] = {
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // singular, 1st case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // singular, 2nd case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // singular, 4th case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // singular, 3rd case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // plural, 1st case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // plural, 2st case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, // plural, 4st case
	GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE, GRAMMAR_ARTICLE_ID_NONE  // plural, 3st case
}; // ds:0xa9b3, [8], by case, then by gender

extern char g_str_s_s[6];
extern char g_str_von_s_s[9];

static char *g_str_s_s_ptr = (char*)&g_str_s_s; // ds:0xa9e3, to STR_S_S; uint8_t*
static char *g_str_von_s_s_ptr = (char*)&g_str_von_s_s; // ds:0xa9e7, to STR_VON_S_S; uint8_t*

static signed int g_grammar_buf_index = 0; // ds:0xa9eb
// takes values 0, 1, 2, 3. Used to access g_grammar_bufs[1..4] (note the index shift!) as a ring buffer.

static struct int16_t_3 g_grammar_gender_bitmasks = {
	INFLECT_MASCULINE, // 0x1000
	INFLECT_FEMININE,  // 0x2000
	INFLECT_NEUTER     // 0x3000
}; // ds:0xa9ed

enum {
	GRAMMAR_PRONOUN_ID_ER    = 0,
	GRAMMAR_PRONOUN_ID_SIE   = 1,
	GRAMMAR_PRONOUN_ID_ES    = 2,
	GRAMMAR_PRONOUN_ID_SEIN  = 3,
	GRAMMAR_PRONOUN_ID_IHR   = 4,
	GRAMMAR_PRONOUN_ID_IHN   = 5,
	GRAMMAR_PRONOUN_ID_IHM   = 6,
	GRAMMAR_PRONOUN_ID_IHNEN = 7
};

static char g_grammar_pronouns_er[3]     = "ER"; // ds:0xa9f3
static char g_grammar_pronouns_sie[4]    = "SIE"; // ds:0xa9f6
static char g_grammar_pronouns_es[3]     = "ES"; // ds:0xa9fa
static char g_grammar_pronouns_sein[5]   = "SEIN"; // ds:0xa9fd
static char g_grammar_pronouns_ihr[4]    = "IHR"; // ds:0xaa02
static char g_grammar_pronouns_ihn[4]    = "IHN"; // ds:0xaa06
static char g_grammar_pronouns_ihm[4]    = "IHM"; // ds:0xaa0a
static char g_grammar_pronouns_ihnen[6]  = "IHNEN"; // ds:0xaa0e
static char *g_grammar_pronouns_index[7] = {
	g_grammar_pronouns_er,
	g_grammar_pronouns_sie,
	g_grammar_pronouns_es,
	g_grammar_pronouns_sein,
	g_grammar_pronouns_ihr,
	g_grammar_pronouns_ihn,
	g_grammar_pronouns_ihm
}; // ds:0xaa14; uint8_t*

static const signed char g_grammar_pronouns_table2[33] = {
	GRAMMAR_PRONOUN_ID_ER,    GRAMMAR_PRONOUN_ID_SIE,   GRAMMAR_PRONOUN_ID_ES,    // singular, 1st case
	GRAMMAR_PRONOUN_ID_SEIN,  GRAMMAR_PRONOUN_ID_IHR,   GRAMMAR_PRONOUN_ID_SEIN,  // singular, posessive (2nd case would be "seiner, ihrer, seiner")
	GRAMMAR_PRONOUN_ID_IHN,   GRAMMAR_PRONOUN_ID_SIE,   GRAMMAR_PRONOUN_ID_ES,    // singular, 4th case
	GRAMMAR_PRONOUN_ID_IHM,   GRAMMAR_PRONOUN_ID_IHR,   GRAMMAR_PRONOUN_ID_IHM,   // singular, 3rd case
	GRAMMAR_PRONOUN_ID_SIE,   GRAMMAR_PRONOUN_ID_SIE,   GRAMMAR_PRONOUN_ID_SIE,   // plural, 1st case
	GRAMMAR_PRONOUN_ID_IHR,   GRAMMAR_PRONOUN_ID_IHR,   GRAMMAR_PRONOUN_ID_IHR,   // plural, posessive (2nd case would be "ihrer, ihrer, ihrer")
	GRAMMAR_PRONOUN_ID_SIE,   GRAMMAR_PRONOUN_ID_SIE,   GRAMMAR_PRONOUN_ID_SIE,   // plural, 4th case
	GRAMMAR_PRONOUN_ID_IHNEN, GRAMMAR_PRONOUN_ID_IHNEN, GRAMMAR_PRONOUN_ID_IHNEN, // plural, 3rd case
	0x00, -128, -64, -32, -16, -8, -4, -2, -1
}; // ds:0xaa30

static const struct struct_char_width g_gui_char_width[75] = {
	{        ' ',  0, 6 },
	{        'A',  1, 6 },
	{        'B',  2, 6 },
	{        'C',  3, 6 },
	{        'D',  4, 6 },
	{        'E',  5, 6 },
	{        'F',  6, 6 },
	{        'G',  7, 6 },
	{        'H',  8, 6 },
	{        'I',  9, 4 },
	{        'J', 10, 6 },
	{        'K', 11, 6 },
	{        'L', 12, 5 },
	{        'M', 13, 6 },
	{        'N', 14, 6 },
	{        'O', 15, 6 },
	{        'P', 16, 6 },
	{        'Q', 17, 6 },
	{        'R', 18, 6 },
	{        'S', 19, 6 },
	{        'T', 20, 6 },
	{        'U', 21, 6 },
	{        'V', 22, 6 },
	{        'W', 23, 6 },
	{        'X', 24, 6 },
	{        'Y', 25, 6 },
	{        'Z', 26, 6 },
	{ (char)0x8e, 27, 6 },
	{ (char)0x99, 28, 6 },
	{ (char)0x9a, 29, 6 },
	{        ',', 30, 3 },
	{        '.', 31, 2 },
	{        '-', 32, 6 },
	{        '(', 33, 4 },
	{        ')', 34, 4 },
	{        '!', 35, 3 },
	{        '?', 36, 5 },
	{        '0', 37, 6 },
	{        '1', 38, 3 },
	{        '2', 39, 6 },
	{        '3', 40, 6 },
	{        '4', 41, 6 },
	{        '5', 42, 6 },
	{        '6', 43, 6 },
	{        '7', 44, 6 },
	{        '8', 45, 6 },
	{        '9', 46, 6 },
	{       '\'', 47, 3 },
	{        ':', 48, 3 }, /* NOT IN HEIGHT TABLE */
	{        '#', 49, 6 },
	{        '&', 50, 5 },
	{        '%', 51, 5 },
	{        '<', 53, 5 },
	{        '>', 52, 5 },
	{        '/', 54, 6 },
	{        '_', 55, 6 }, /* NOT IN HEIGHT TABLE */
	{ (char)0xc8, 56, 6 },
	{ (char)0xc9, 57, 6 },
	{ (char)0xca, 58, 6 },
	{ (char)0xcb, 59, 5 },
	{ (char)0xcc, 60, 5 },
	{ (char)0xcd, 61, 4 },
	{ (char)0xce, 62, 5 },
	{ (char)0xcf, 63, 5 },
	{ (char)0xd0, 64, 2 },
	{ (char)0xd1, 65, 6 },
	{ (char)0xd2, 66, 6 },
	{ (char)0xd3, 67, 5 },
	{ (char)0xd4, 68, 6 },
	{ (char)0xd5, 69, 5 },
	{ (char)0xd6, 70, 6 },
	{ (char)0xd7, 71, 5 },
	{ (char)0xd8, 72, 6 },
	{ (char)0xd9, 73, 5 },
	{        '+',  0, 6 } /* NOT IN HEIGHT TABLE */
}; // ds:0xaa51
char g_str_s_s[6] = "%s %s"; // ds:0xab32
char g_str_von_s_s[9] = "VON%s %s"; // ds:0xab38


static char g_gui_print_char;		// ds:0xe4d8
signed int g_dialogbox_indent_height;	// ds:0xe4d9
signed int g_dialogbox_indent_width;	// ds:0xe4db
unsigned char g_unkn_092[6];		// ds:0xe4dd
static char g_grammar_bufs[5][40];	// ds:0xe4e3


//000
/**
 * \brief   Forms a grammatical inflection of a noun, possibly including an article.
 *
 * \param   inflection_features
 *              bits 0 and 1: case
 *                  0: 1st case; 1: 2nd case; 2: 4th case; 3: 3rd case
 *              bit 2: number
 *                  0: singular; 1: plural
 *              bits 14 and 15: article
 *                  1: indefinite article (ein/eine/einer etc.); 2: omit article; 3: definite article (der/die/das etc.)
 *              bits 12 and 13 are internally added for gender.
 *                  1: masculine; 2: feminine; 3: neuter
 * \param   name_id	index of the noun to be inflected
 * \param   name_type	0: name_id refers to an item; 1: name_id refers to a monster.
 */
uint8_t* GUI_name_inflect_with_article(int16_t inflection_features, const signed int name_id, const signed int name_type)
{
	signed int *tmp_ptr; /* multi use: pointer to entries of g_items_noplural, g_grammar_definite_article_table, g_grammar_omitted_article_table, and g_grammar_indefinite_article_table */
	signed int use_von_form = 0;
	char *p_name;
	signed int tmp;
	struct int16_t_3 gender_bitmask = g_grammar_gender_bitmasks;

	if (name_type == INFLECT_NAME_TYPE_ITEM) {
		/* string_array_itemnames */
		p_name = g_itemsname[name_id];

		 /* set gender to bits 12 and 13 */
#ifndef M302de_ORIGINAL_BUGFIX
		// Usage of + is not clean, assumes that the bits got initialized with 0.
		// This is probably always the case, so more a glitch than a bug.
		inflection_features += gender_bitmask.a[g_item_name_genders[name_id]]; /* set gender to bits 12, 13 */
#else
		inflection_features |= gender_bitmask.a[g_item_name_genders[name_id]]; /* set gender to bits 12, 13 */
#endif

		tmp_ptr = &g_items_noplural[0];

		while (((tmp = *tmp_ptr++) != -1) && (tmp != name_id));

		if (tmp == name_id) {
			/* item name has no plural form */

#ifndef M302de_ORIGINAL_BUGFIX
			// the following is pretty wild...
			inflection_features += INFLECT_PLURAL; // Usage of "+" instead of "|" is not clean. This in effect toggles the plural bit.
			inflection_features &= ~INFLECT_DEFINITE_ARTICLE; // unset bit 15 (unset definite_article)
			inflection_features |= INFLECT_OMIT_ARTICLE; // set bit 14 (set omit_article)
			if (inflection_features & 1) // either 2nd or 3rd case. 3rd case does not make sense here.
				use_von_form = 1;
#else
			// Maybe this was the original intention
			inflection_features &= ~INFLECT_NUMBER_BITMASK; // unset plural bit

			if ((inflection_features & INFLECT_CASE_BITMASK) == INFLECT_2ND_CASE) {
				inflection_features &= ~INFLECT_DEFINITE_ARTICLE; // unset bit 15 (unset definite_article)
				inflection_features |= INFLECT_OMIT_ARTICLE; // set bit 14 (set omit_article)
				use_von_form = 1;
			}
#endif
		}
	} else {
		p_name = g_monnames_index[name_id];

		 /* set gender to bits 12 and 13 */
#ifndef M302de_ORIGINAL_BUGFIX
		// Usage of + is not clean, assumes that the bits got initialized with 0.
		// This is probably always the case, so more a glitch than a bug.
		inflection_features += gender_bitmask.a[g_monster_name_genders[name_id]];
#else
		inflection_features |= gender_bitmask.a[g_monster_name_genders[name_id]];
#endif
	}

	// point tmp_ptr to the entry in g_grammar_definite_article_table in masculine form, grammatical number and case described by bits 0..2
	tmp_ptr = (inflection_features & INFLECT_DEFINITE_ARTICLE) ?
		&g_grammar_definite_article_table[(inflection_features & 0xf) * 3] :
		((inflection_features & INFLECT_OMIT_ARTICLE) ?
		&g_grammar_omitted_article_table[0] : // note that g_grammar_omitted_article_table is constant
		&g_grammar_indefinite_article_table[(inflection_features & 0xf) * 3]); // otherwise: indefinite_article


	sprintf(g_grammar_bufs[g_grammar_buf_index + 1],
		(use_von_form == 0 ? g_str_s_s_ptr : g_str_von_s_s_ptr),
		g_grammar_articles_index[tmp_ptr[((inflection_features & INFLECT_GENDER_BITMASK) - 1) >> 12]], // tmp_ptr is refined by gender stored in bits 12 and 13 */
		GUI_name_inflect(inflection_features, p_name)
	);

	p_name = g_grammar_bufs[g_grammar_buf_index + 1];

	/* remove leading blank. applies to cases without article */
	if (*p_name == ' ') {
		do {
			tmp = *(++p_name);
			*(p_name - 1) = tmp;

		} while (tmp != 0);
	}

	tmp = g_grammar_buf_index;

	/* increase index of ring buffer */
	if (++g_grammar_buf_index == 4)
		g_grammar_buf_index = 0;

	return (uint8_t*)g_grammar_bufs[1 + tmp];
}

//1a7
/**
 * \brief   Forms a grammatical inflection of a noun
 *
 * \param   inflection_features
 *              bits 0 and 1: case
 *                  0: 1st case; 1: 2nd case; 2: 4th case; 3: 3rd case
 *              bit 2: number
 *                  0: singular; 1: plural
 *              bits 12 and 13: gender
 *                  1: masculine; 2: feminine; 3: neuter
 *                  0: unspecified: this is often o.k.: The gender information is only used in 2nd or 3rd case singluar.
 * \param   noun_template
 * 		the noun to be inflected, given as a template "<root>.<singular_ending>.<plural_ending>".
 * 		example: "WURF.AXT.ÄXTE"
 */
char* GUI_name_inflect(const signed int inflection_features, char *noun_template)
{
	char *p = g_grammar_bufs[0];
	char tmp;

	/* copy first part */
	while ((tmp = *noun_template++) && (tmp != '.')) {
		*p++ = tmp;
	}

	if (inflection_features & INFLECT_PLURAL)
		/* skip second part */
		while ((tmp = *noun_template++) && (tmp != '.'));

	/* copy next part (second part for singular, third part for plural) */
	while ((tmp = *noun_template) && (tmp != '.'))
		*p++ = *noun_template++;

	if (
#ifndef M302de_ORIGINAL_BUGFIX
		(inflection_features & INFLECT_CASE_NUMBER_BITMASK) == 1
		// 2nd or 3rd case singular. 3rd case is not correct.
#else
		(inflection_features & INFLECT_CASE_NUMBER_BITMASK) == INFLECT_2ND_CASE
		// 2nd case singular
#endif
		&& (inflection_features & INFLECT_GENDER_BITMASK) != INFLECT_FEMININE // masculine or neuter
	) {
		// if word ends in B or D, append 'E'
		if (*(p-1) == 'B' || *(p-1) == 'D') {
			*p++ = 'E';
		}

		// then always append 'S'
		*p++ = 'S';

		// Example: DAS HEMD
		// second case: "angesichts DES HEMDES"
		// third case: "Ich vertraue DEM HEMD"
		// fourth case: "Ich sehe DAS HEMD"

	} else {
		if (((inflection_features & INFLECT_CASE_NUMBER_BITMASK) == (INFLECT_PLURAL | INFLECT_3RD_CASE))
			&& (*(p-1) != 'N') && (*(p-1) != 'S')) {
			// append 'N'
				*p++ = 'N';
		}

		// Example: DER HELM, plural DIE HELME
		// second case: "angesichts DER HELME" -> no N
		// third case: "Ich vertraue DEN HELMEN" -> append N
		// fourth case: "Ich sehe DIE HELME" -> no N
	}

	*p = 0; // append terminator symbol

	return g_grammar_bufs[0];
}

//290
char* GUI_name_base_form(char *s)
{
	char *p = g_grammar_bufs[0];
	char tmp;

	/* write first and second part of name template to g_grammar_bufs[0] */
	/* Example: WURF.AXT.ÄXTE -> WURFAXT */

	/* first part */
	while ((tmp = *s++) && (tmp != '.')) {
		*p++ = tmp;
	}

	/* second part */
	while ((tmp = *s) && (tmp != '.')) {
		*p++ = *s++;
	}

	*p = 0; // append terminator symbol

	return g_grammar_bufs[0];
}

//2f2
/**
 * \brief   return a pointer to a personal pronoun which can be used to replace a name
 *
 * \param   inflection_features
 *              bits 0 and 1: case
 *                  0: 1st case; 1: 2nd case; 2: 4th case; 3: 3rd case
 *              bit 2: number
 *                  0: singular; 1: plural
 * \param   name_id	index of the noun to be inflected
 * \param   name_type	0: name_id refers to an item; 1: name_id refers to a monster.
 */
uint8_t* GUI_grammar_name_to_personal_pronoun(const signed int inflection_features, const signed int name_id, const signed int name_type)
{
	signed int genus = (name_type == INFLECT_NAME_TYPE_ITEM ? g_item_name_genders[name_id] : g_monster_name_genders[name_id]);

	return (uint8_t*)g_grammar_pronouns_index[g_grammar_pronouns_table2[inflection_features * 3 + genus]];
}

//330
/**
 * \brief   return a pointer to a personal pronoun.
 *          Only singular form, masculine or feminine.
 *
 * \param   gender	       the grammatical gender
 * \param   grammatical_case   the grammatical case
 */
char* GUI_get_personal_pronoun(const signed int gender, const signed int grammatical_case)
{
	if (gender == GRAMMAR_GENDER_MASCULINE) {
		return (grammatical_case == GRAMMAR_CASE_1ST) ? g_grammar_pronouns_er :
			((grammatical_case == GRAMMAR_CASE_2ND) ? g_grammar_pronouns_sein :
			((grammatical_case == GRAMMAR_CASE_3RD) ? g_grammar_pronouns_ihm : g_grammar_pronouns_ihn));
	} else {
		return (grammatical_case == GRAMMAR_CASE_1ST) ? g_grammar_pronouns_sie :
			((grammatical_case == GRAMMAR_CASE_2ND) ? g_grammar_pronouns_ihr :
			((grammatical_case == GRAMMAR_CASE_3RD) ? g_grammar_pronouns_ihr : g_grammar_pronouns_sie));
	}
}

//394
/**
 * \brief   return a pointer to a definite article.
 *          Only singular form, masculine or feminine.
 *
 * \param   gender	       the grammatical gender
 * \param   grammatical_case   the grammatical case
 */
char* GUI_get_definite_article(const signed int gender, const signed int grammatical_case)
{
	if (gender == GRAMMAR_GENDER_MASCULINE) {
		return (grammatical_case == GRAMMAR_CASE_1ST) ? g_grammar_article_der :
			((grammatical_case == GRAMMAR_CASE_2ND) ? g_grammar_article_des :
			((grammatical_case == GRAMMAR_CASE_3RD) ? g_grammar_article_dem : g_grammar_article_den));
	} else {
		return (grammatical_case == GRAMMAR_CASE_1ST) ? g_grammar_article_die :
			((grammatical_case == GRAMMAR_CASE_2ND) ? g_grammar_article_der :
			((grammatical_case == GRAMMAR_CASE_3RD) ? g_grammar_article_der : g_grammar_article_die));
	}
}

//3f8
void GUI_write_char_to_screen(uint8_t *dst, const signed int char_width, const signed int char_height)
{
	uint8_t *ptr;
	signed int y;
	signed int x;

	ptr = &g_gui_text_buffer[0];

	for (y = 0; y < char_width; ptr += 8 - char_height, dst += 320, y++) {
		for (x = 0; x < char_height; x++)
			*(dst + x) = *ptr++;
	}
}

/**
 * \brief   return the number of lines the input string needs on a screen
 *
 * \param   str		the input string
 * \return		the number of lines on the screen
 *
 * \warning The input string str may be modified if it does not
 *          fit on the screen. So EVERY string in this sourcecode,
 *	    which should be printed on screen must not be constant.
 *	    Otherwise this application may crash.
 */
signed int GUI_count_lines(char *str)
{
	signed int current_pos;
	signed int last_ws;
	char *str_loc;
	signed int always_zero;
	signed int lines = 0;
	signed int max_line_width;
	signed int width_char;
	signed int width_line;

	if (!str) return 0;

	/* replace all CR and LF in the input string with whitespaces */
	for (str_loc = str; *str_loc; str_loc++)
		if (*str_loc == 0x0d || *str_loc == 0x0a)
			*str_loc = 0x20;

	str_loc = str;
	current_pos = last_ws = always_zero = 0;
	max_line_width = g_textline_maxlen;

	if (g_dialogbox_indent_width)
		g_textline_maxlen -= g_dialogbox_indent_width;

	width_line = 0;

	for (; str_loc[current_pos]; current_pos++) {

		GUI_lookup_char_width(str_loc[current_pos], &width_char);

		width_line += width_char;

		/* check if the input string str is to long for the line */
		if (width_line >=  g_textline_maxlen) {
			if (last_ws != always_zero) {
				/* 	if a whitespace existed on the current line,
					do a linebreak there */
				str_loc[last_ws] = 0x0d;
				str_loc = str_loc + last_ws;
			} else {
				/* 	if no whitespace existed on the current line,
					break at current pos and overwrite str_loc[current_pos] with CR */
				str_loc[current_pos] = 0x0d;
				str_loc = &str_loc[current_pos + 1];
			}

			if (++lines == g_dialogbox_indent_height)
				g_textline_maxlen += g_dialogbox_indent_width;

			/* reset variables */
			always_zero = current_pos = last_ws = width_line = 0;
		}

		/* remember the position of the last whitespace for linebreaks */
		if (str_loc[current_pos] == 0x20)
			last_ws = current_pos;

		/* a forced break in the text with '@' */
		if (str_loc[current_pos] == 0x40) {
			str_loc = &str_loc[current_pos + 1];
			current_pos = -1;
			always_zero = last_ws = width_line = 0;
			if (++lines == g_dialogbox_indent_height)
				g_textline_maxlen += g_dialogbox_indent_width;
		}
	}

	if (width_line >= g_textline_maxlen) {

		if (always_zero == last_ws)
			str_loc[current_pos - 1] = 0;
		else {
			str_loc[last_ws] = 0x0d;
			if (++lines == g_dialogbox_indent_height)
				g_textline_maxlen += g_dialogbox_indent_width;
		}
	}

	g_textline_maxlen = max_line_width;
	return ++lines;
}

//5d7
signed int GUI_print_header(char *str)
{
	signed int retval = 1;

	call_mouse_bg();
	retval = GUI_count_lines(str);
	GUI_print_string(str, g_textline_posx, g_textline_posy);
	call_mouse();

	return retval;
}

//614
void GUI_print_loc_line(char *str)
{
	signed int fg_bak;
	signed int bg_bak;
	signed int tlx_bak;
	signed int tly_bak;
	signed int ml_bak;

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	tlx_bak = g_textline_posx;
	tly_bak = g_textline_posy;
	ml_bak = g_textline_maxlen;

	g_textline_posx = 6;
	g_textline_posy = 143;
	g_textline_maxlen = 307;

	clear_loc_line();
	GUI_print_header(str);

	g_textline_posx = tlx_bak;
	g_textline_posy = tly_bak;
	g_textline_maxlen = ml_bak;

	set_textcolor(fg_bak, bg_bak);
}

//691
void GUI_print_string(char *str, signed int x, signed int y)
{
	signed int l1 = 0;
	signed int i = 0;
	signed int x_bak;
	unsigned char c;

	call_mouse_bg();

	if (g_gui_text_centered == 1) {
		x = GUI_get_first_pos_centered(str, x, g_textline_maxlen, 0);
	} else
		if (g_dialogbox_indent_width)
			x += g_dialogbox_indent_width;
	x_bak = x;

	while ((c = str[i++])) {

		/* handle line breaks */
		if ((c == 0x0d) || (c == '@')) {

			if (++l1 == g_dialogbox_indent_height) {
				g_textline_maxlen += g_dialogbox_indent_width;
				x_bak -= g_dialogbox_indent_width;
			}

			y += 7;
			x = (g_gui_text_centered == 1) ?
				GUI_get_first_pos_centered(str + i, g_textline_posx, g_textline_maxlen, 0) : x_bak;

		} else	if (c == '~') {

			if (x < g_txt_tabpos[0])
				x = g_txt_tabpos[0];
			else if (x < g_txt_tabpos[1])
				x = g_txt_tabpos[1];
			else if (x < g_txt_tabpos[2])
				x = g_txt_tabpos[2];
			else if (x < g_txt_tabpos[3])
				x = g_txt_tabpos[3];
			else if (x < g_txt_tabpos[4])
				x = g_txt_tabpos[4];
			else if (x < g_txt_tabpos[5])
				x = g_txt_tabpos[5];
			else if (x < g_txt_tabpos[6])
				x = g_txt_tabpos[6];

		} else if (c == (unsigned char)0xf0 ||	c == (unsigned char)0xf1 || c == (unsigned char)0xf2 ||	c == (unsigned char)0xf3)
		{
			/* changes of the text color are only control bytes */
			g_textcolor_index = c - 0xf0;
		} else	{

			if (c == '<') {
				c = '>';
			} else { }

			x += GUI_print_char(c, x, y);
		}
	}

	call_mouse();

#if !defined(__BORLANDC__)
	sdl_forced_update();
#endif
}

//7f0
signed int GUI_print_char(const char c, const signed int x, const signed int y)
{
	signed int char_width;
	signed int font_index;

	g_gui_print_char = c;

	font_index = GUI_lookup_char_width(c, &char_width);

	GUI_write_fonti_to_screen(font_index, char_width, x, y);

	return char_width;
}

//82b
signed int GUI_lookup_char_width(const signed char c, signed int *p)
{
	signed int i;

	/* REMARK: This array of struct should be seen as signed char[75*3] */
	for (i = 0; i != 75 * 3; i += 3) {

		if (c == ((signed char*)g_gui_char_width)[i]) {

			*p = ((signed char*)g_gui_char_width)[i + 2] & 0xff;

			return ((signed char*)g_gui_char_width)[i + 1] & 0xff;
		}
	}

	if ((c == '~') || (c == (signed char)0xf0) || (c == (signed char)0xf1) || (c == (signed char)0xf2) || (c == (signed char)0xf3))
	{
		return *p = 0;
	}

	*p = 5;
	return 0;
}

//88f
void GUI_write_fonti_to_screen(const signed int font_index, const signed int char_width, const signed int x, const signed int y)
{
	GUI_blank_char();
	GUI_font_to_buf(g_buf_font6 + font_index * 8);
	GUI_write_char_to_screen_xy(x, y, 7, char_width);
}
//8c5
/**
 * \brief   sets the area of a char to a color
 */
void GUI_blank_char(void)
{
	uint8_t *ptr = &g_gui_text_buffer[0];
	signed int i;
	signed int j;

	for (i = 0; i < 8; ptr += 8, i++) {
		for (j = 0; j < 8; j++)
			*(ptr + j) = g_textcolor_bg;
	}
}

//8f8
void GUI_font_to_buf(uint8_t *fc)
{
	uint8_t *p;
	char c;
	signed int i;
	signed int j;

	/* current text position */
	p = &g_gui_text_buffer[0];

	if (g_gui_print_char == 0x3a)
		fc++;

	for (i = 0; i < 8; p += 8, i++) {

		c = *fc++;

		for (j = 0; j < 8; j++) {
			if ((0x80 >> j) & c)
				 p[j] = g_textcolor_fg[g_textcolor_index];
		}
	}
}

//956
void GUI_write_char_to_screen_xy(const signed int x, const signed int y, const signed int char_height, const signed int char_width)
{
	/* screen_start */
	uint8_t* dst = g_vga_backbuffer + y * 320 + x;

	GUI_write_char_to_screen(dst, char_height, char_width);
}

/**
 * \brief   sets the textcolor
 *
 * \param   fg          foreground color index
 * \param   bg          background color index
 */
void set_textcolor(const signed int fg, const signed int bg)
{
	g_textcolor_fg[0] = fg;
	g_textcolor_bg = bg;
}

/**
 * \brief   gets the textcolor
 *
 * \param   fg          foreground color index
 * \param   bg          background color index
 */
void get_textcolor(signed int *fg, signed int *bg)
{
	*fg = g_textcolor_fg[0];
	*bg = g_textcolor_bg;
}

signed int GUI_unused(char *str)
{
	signed int lines = 0;

	while (*str) {

		if (*str++ == 0x0d) {
			lines++;
		}
	}

	return lines;
}

//9D6
signed int GUI_get_space_for_string(char *p, const signed int dir)
{
	signed int sum;
	signed int tmp;

	for (sum = 0; *p; sum += tmp) {
		if (dir) {
			GUI_lookup_char_height(*p++, &tmp);
		} else {
			GUI_lookup_char_width(*p++, &tmp);
		}
	}

	return sum;
}

//A26
signed int GUI_get_first_pos_centered(char *p, const signed int x, signed int v2, const signed int dir)
{
	register signed int i;
	register signed int c;
	signed int tmp;

	for (i = 0; (c = *p) && (c != '@') && (c != 0x0d); i += tmp) {
		if (dir)
			GUI_lookup_char_height(*p++, &tmp);
		else
			GUI_lookup_char_width(*p++, &tmp);

	}

	v2 -= i;
	v2 >>= 1;
	v2 += x;

	return v2;
}

//A93
/**
 * \brief   draws a line of a popup window
 *
 * \param   line        number of the current line
 * \param   type        type of line 0 = top / 1 = middle normal / 2 = middle radio / 3 =bottom
 */
void GUI_draw_popup_line(const signed int line, const signed int type)
{
	signed int popup_left;
	signed int i;
	signed int popup_middle;
	signed int popup_right;
	signed int y;
	signed int x;

	/* set the offsets in the popup.dat buffer */
	switch (type) {
		case 0:
			popup_left = 0;
			popup_middle = 0x380;
			popup_right = 0x80;
			break;
		case 1:
			popup_left = 0x100;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		case 2:
			popup_left = 0x200;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		case 3:
			popup_left = 0x280;
			popup_middle = 0x580;
			popup_right = 0x300;
			break;
	}

	x = g_textbox_pos_x;
	y = g_textbox_pos_y + (line * 8);
	g_pic_copy.x1 = x;
	g_pic_copy.y1 = y;
	g_pic_copy.x2 = x + 15;
	g_pic_copy.y2 = y + 7;
	g_pic_copy.src = g_popup + popup_left;

	do_pic_copy(0);

	g_pic_copy.src = g_popup + popup_middle;

	x += 16;

	for (i = 0; i < g_textbox_width; i++) {
		g_pic_copy.x1 = x;
		g_pic_copy.x2 = x + 31;
		do_pic_copy(0);
		x += 32;
	}

	g_pic_copy.src = g_popup + popup_right;
	g_pic_copy.x1 = x;
	g_pic_copy.x2 = x + 15;

	do_pic_copy(0);
}
