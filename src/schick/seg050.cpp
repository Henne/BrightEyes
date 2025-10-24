/*
 *	Rewrite of DSA1 v3.02_de functions of seg050 (level up)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg050.cpp
 */
#include <stdio.h>
#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg046.h"
#include "seg047.h"
#include "seg050.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

const signed short g_autoinc_spells_witch[3] = { SP_ODEM_ARCANUM, SP_BLITZ_DICH_FIND, -1 }; // ds:25924
const signed short g_autoinc_spells_druid[4] = { SP_ODEM_ARCANUM, SP_VERWANDLUNG_BEENDEN, SP_EXPOSAMI_CREATUR, -1 }; // ds:25930

static const signed short g_autoinc_spells_mage00[] = {
	SP_GEISTER_AUSTREIBEN, SP_BALSAM_SALABUNDE, SP_ARCANO_PSYCHOSTABILIS,
	SP_IGNIFAXIUS_FLAMMENSTRAHL, SP_KLARUM_PURUM_KRAEUTERSUD, SP_ANALUES_ARCANSTRUKTUR,
	SP_FLIM_FLAM_FUNKEL, SP_ARMATRUTZ, SP_HEPTAGON_UND_KROETENEI, SP_ABVENENUM_PEST_UND_GALLE, -1};
static const signed short g_autoinc_spells_mage01[] = {
	SP_ODEM_ARCANUM, SP_SENSIBAR_WAHR_UND_KLAR, SP_IGNIFAXIUS_FLAMMENSTRAHL,
	SP_BAND_UND_FESSEL, SP_DUPLICATUS_DOPPELPEIN, SP_GROSSE_VERWIRRUNG, SP_BALSAM_SALABUNDE,
	SP_HARMLOSE_GESTALT, SP_FLIM_FLAM_FUNKEL, SP_HERR_UEBER_DAS_TIERREICH, SP_EIGENSCHAFTEN_SEID_GELESEN, -1};
static const signed short g_autoinc_spells_mage02[] = {
	SP_NIHILATIO_GRAVITAS, SP_ODEM_ARCANUM, SP_SOLIDIRID_FARBENSPIEL,
	SP_FLIM_FLAM_FUNKEL, SP_BALSAM_SALABUNDE, SP_IGNIFAXIUS_FLAMMENSTRAHL, SP_DUNKELHEIT,
	SP_GARDIANUM_PARADEI, SP_ECLIPTIFACTUS, SP_GEISTER_BESCHWOEREN, SP_ANALUES_ARCANSTRUKTUR, -1};
static const signed short g_autoinc_spells_mage03[] = {
	SP_BALSAM_SALABUNDE, SP_IGNIFAXIUS_FLAMMENSTRAHL, SP_FLIM_FLAM_FUNKEL, SP_PENETRIZZEL_HOLZ_UND_STEIN,
	SP_CLAUDIBUS_CLAVISTIBOR, SP_SOLIDIRID_FARBENSPIEL, SP_ODEM_ARCANUM, SP_PLUMBUMBARUM_UND_NARRETEI,
	SP_IN_SEE_UND_FLUSS, SP_VISIBILI_VANITAR, SP_UEBER_EIS_UND_UEBER_SCHNEE, -1};
static const signed short g_autoinc_spells_mage04[] = {
	SP_ABVENENUM_PEST_UND_GALLE, SP_ODEM_ARCANUM, SP_EIGENSCHAFTEN_SEID_GELESEN, SP_ARMATRUTZ,
	SP_GARDIANUM_PARADEI, SP_BLITZ_DICH_FIND, SP_TIERE_BESPRECHEN, SP_KK_STEIGERN, SP_FLIM_FLAM_FUNKEL, SP_EISENROST_UND_GRUENER_SPAN, -1};
static const signed short g_autoinc_spells_mage05[] = {
	SP_BALSAM_SALABUNDE, SP_FLIM_FLAM_FUNKEL, SP_RESPONDAMI_VERITAR,
	SP_IGNIFAXIUS_FLAMMENSTRAHL, SP_BANNBALADIN, SP_ARMATRUTZ,
	SP_ARCANO_PSYCHOSTABILIS, SP_GARDIANUM_PARADEI, SP_HORRIPHOBUS, -1};
static const signed short g_autoinc_spells_mage06[] = {
	SP_GARDIANUM_PARADEI, SP_BALSAM_SALABUNDE, SP_PARALUE_PARALEIN,
	SP_FLIM_FLAM_FUNKEL, SP_ECLIPTIFACTUS, SP_HORRIPHOBUS, SP_DUNKELHEIT,
	SP_SAFT_KRAFT_MONSTERMACHT, SP_ODEM_ARCANUM, SP_FUROR_BLUT_UND_SULPHURDAMPF,
	SP_ARCANO_PSYCHOSTABILIS, SP_KK_STEIGERN, SP_GE_STEIGERN, SP_MU_STEIGERN, -1};
static const signed short g_autoinc_spells_mage07[] = {
	SP_BALSAM_SALABUNDE, SP_KLARUM_PURUM_KRAEUTERSUD, SP_IGNIFAXIUS_FLAMMENSTRAHL,
	SP_FLIM_FLAM_FUNKEL, SP_KK_STEIGERN, SP_ARCANO_PSYCHOSTABILIS, SP_GE_STEIGERN,
	SP_ECLIPTIFACTUS, SP_IN_SEE_UND_FLUSS, SP_ODEM_ARCANUM, SP_EIGENSCHAFTEN_SEID_GELESEN, -1};
static const signed short g_autoinc_spells_mage08[] = {
	SP_PARALUE_PARALEIN, SP_ARMATRUTZ, SP_GARDIANUM_PARADEI,
	SP_BALSAM_SALABUNDE, SP_IGNIFAXIUS_FLAMMENSTRAHL, SP_PENETRIZZEL_HOLZ_UND_STEIN,
	SP_FORAMEN_FORAMINOR, SP_WEICHES_ERSTARRE, SP_AEOLITUS_WINDGEBRAUS,
	SP_STURMGEBRUELL_BESAENFTGE_DICH, SP_PLUMBUMBARUM_UND_NARRETEI, SP_DESTRUCTIBO_ARCANITAS, -1};

const signed short* g_autoinc_spells_mage_index[9] = {
	g_autoinc_spells_mage00,
	g_autoinc_spells_mage01,
	g_autoinc_spells_mage02,
	g_autoinc_spells_mage03,
	g_autoinc_spells_mage04,
	g_autoinc_spells_mage05,
	g_autoinc_spells_mage06,
	g_autoinc_spells_mage07,
	g_autoinc_spells_mage08
}; // ds:0x662a, by magic school; Bit8u*
const signed short g_autoinc_spells_gelf[7] = { SP_FLIM_FLAM_FUNKEL, SP_BLITZ_DICH_FIND, SP_AXXELERATUS_BLITZGESCHWIND, SP_KLARUM_PURUM_KRAEUTERSUD, SP_TIERE_BESPRECHEN, SP_DUNKELHEIT, -1 }; // ds:26190
const signed short g_autoinc_spells_self[7] = { SP_SILENTIUM_SILENTILLE, SP_ARMATRUTZ, SP_FULMINICTUS_DONNERKEIL, SP_ADLERAUG_UND_LUCHSENOHR, SP_ODEM_ARCANUM, SP_FLIM_FLAM_FUNKEL, -1 }; // ds:26204
const signed short g_autoinc_spells_ielf[12] = { SP_BALSAM_SALABUNDE, SP_CHAMAELIONI_MIMIKRY, SP_BLITZ_DICH_FIND, SP_ADLERAUG_UND_LUCHSENOHR, SP_FULMINICTUS_DONNERKEIL, SP_ARMATRUTZ, SP_SILENTIUM_SILENTILLE, SP_ODEM_ARCANUM, SP_BAND_UND_FESSEL, SP_AXXELERATUS_BLITZGESCHWIND, SP_EXPOSAMI_CREATUR, -1 }; // ds:26218
const struct mssr g_magic_school_spellranges = { {
	{ SP_BEHERRSCHUNG_BRECHEN, SP_VERWANDLUNG_BEENDEN },			/* Antimagie */
	{ SP_BAND_UND_FESSEL, SP_ZWINGTANZ },					/* Beherrschung */
	{ SP_FUROR_BLUT_UND_SULPHURDAMPF, SP_SOLIDIRID_FARBENSPIEL },		/* Daemonologie + Elemente */
	{ SP_AXXELERATUS_BLITZGESCHWIND, SP_UEBER_EIS_UND_UEBER_SCHNEE },	/* Bewegung */
	{ SP_BALSAM_SALABUNDE, SP_TIERE_BESPRECHEN },				/* Heilung */
	{ SP_ADLERAUG_UND_LUCHSENOHR, SP_SENSIBAR_WAHR_UND_KLAR },		/* Wahrnehmung */
	{ SP_BLITZ_DICH_FIND, SP_SCHARFES_AUGE_UND_SICHRE_HAND },		/* Kampf */
	{ SP_ADLER_WOLF_UND_HAMMERHAI, SP_VISIBILI_VANITAR },			/* Verwandlung */
	{ SP_ABVENENUM_PEST_UND_GALLE, SP_STURMGEBRUELL_BESAENFTGE_DICH }	/* Veraenderung */
	/* Note: There are no schools for the spell groups "Visionen" and "Verstaendigung". */
} }; // ds:0x6682



struct inc_states {
	signed char tries;
	signed char incs;
};

static struct inc_states *g_inc_spells_counter;	// ds:0xe3b2
static struct inc_states *g_inc_skills_counter;	// ds:0xe3b6
static unsigned char *g_skills_buffer;		// ds:0xe3ba


/**
 * \brief   tries to increase a spell in advanced mode
 *
 * \param   hero        pointer to the hero
 * \param   spell       number of the spell
 */
void inc_spell_advanced(struct struct_hero *hero, const signed int spell_id)
{
	signed short max_incs = 1;
	signed short randval;
	struct mssr a = g_magic_school_spellranges;

	if ((hero->typus == HERO_TYPE_WITCH) && (g_spell_descriptions[spell_id].herotype == SPELL_DESC_HEROTYPE_WITCH))
	{
		/* witch spell */
		max_incs = 2;
	}

	if ((hero->typus >= HERO_TYPE_GREEN_ELF) && /* hero is one of the three elven types */
		((g_spell_descriptions[spell_id].herotype == SPELL_DESC_HEROTYPE_GELF) ||
			(g_spell_descriptions[spell_id].herotype == SPELL_DESC_HEROTYPE_SELF) ||
			(g_spell_descriptions[spell_id].herotype == SPELL_DESC_HEROTYPE_IELF)))
	{
		/* elven spell */
		max_incs = 2;
	}

	if ((hero->typus == HERO_TYPE_DRUID) &&
		(g_spell_descriptions[spell_id].herotype == SPELL_DESC_HEROTYPE_DRUID))
	{
		/* druid spell */
		max_incs = 2;
	}

	if (hero->typus == HERO_TYPE_MAGE) {

		/* mages */
		if (g_spell_descriptions[spell_id].herotype == SPELL_DESC_HEROTYPE_MAGE) {
			/* spell is a mage spell */
			max_incs = 2;
		}

		/* check if spell is of the specialized group of the mage (according to his HERO_MAGIC_SCHOOL) */
		if ((a.a[hero->spell_school].first <= spell_id) &&
			(a.a[hero->spell_school].last >= spell_id)) {
			max_incs = 2;
		}

		/* depending on the HERO_MAGIC_SCHOOL, the mage will get 3 possible increases on certain spells */
		if (is_in_word_array(spell_id, (signed short*)g_magic_schools_index[hero->spell_school]))
		{
			max_incs = 3;
		}
	}

	if (g_inc_spells_counter[spell_id].incs >= max_incs) {

		/* no increase is possible */

		GUI_output(get_tx2(43));

	} else if (g_inc_spells_counter[spell_id].tries == 3) {

		/* 3 increase failures -> no further increase try allowed */

		GUI_output(get_tx2(44));
	} else {
		/* try to increase */

		hero->spell_incs--;

#ifndef M302de_FEATURE_MOD
		if (hero->spells[spell_id] >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}
#else
		/* Feature mod 2:
		 * use the exact skill/spell increase mechanism as in DSA 2/3 */
		if (hero->spells[spell_id] >= 10) {
			randval = dice_roll(3,6,0);
		} else {
			randval = dice_roll(2,6,0);
		}
#endif

		if (hero->spells[spell_id] < randval) {

			/* success */
			GUI_output(get_tx2(37));

			/* increment spell value */
			hero->spells[spell_id]++;

			/* set the try counter to 0 */
			g_inc_spells_counter[spell_id].tries = 0;
			/* increment the increase counter */
			g_inc_spells_counter[spell_id].incs++;

		} else {
			/* fail */
			GUI_output(get_ttx(338));

			/* increment the try counter */
			g_inc_spells_counter[spell_id].tries++;
		}
	}
}

/**
 * \brief   tries to increase a skill in advanced mode
 *
 * \param   hero        pointer to the hero
 * \param   skill       number of the skill
 */
void inc_skill_advanced(struct struct_hero *hero, const signed int skill_id)
{
	signed short randval;
	signed short max_incs;

	max_incs = g_skill_descriptions[skill_id].max_incs;

	if (g_inc_skills_counter[skill_id].incs >= max_incs) {

		/* no increase is possible */
		GUI_output(get_tx2(43));

	} else if (g_inc_skills_counter[skill_id].tries == 3) {

		/* used up legal increase */
		GUI_output(get_tx2(44));

	} else {

		/* try to increase */
		hero->skill_incs--;

#ifndef M302de_FEATURE_MOD
		if (hero->skills[skill_id] >= 11) {
			randval = random_interval(3, 18);
		} else {
			randval = random_interval(2, 12);
		}
#else
		/* Feature mod 2:
		 * use the exact spell/skill increase mechanism as in DSA 2/3 */
		if (hero->skills[skill_id] >= 10) {
			randval = dice_roll(3,6,0);
		} else {
			randval = dice_roll(2,6,0);
		}
#endif

		if (hero->skills[skill_id] < randval) {

			/* success */
			GUI_output(get_tx2(37));

			/* increment skill value */
			hero->skills[skill_id]++;

			/* set the try counter to 0 */
			g_inc_skills_counter[skill_id].tries = 0;
			/* increment the increase counter */
			g_inc_skills_counter[skill_id].incs++;

			if (skill_id <= TA_ZWEIHAENDER) {
				/* increment a melee weapon skill */

				sprintf(g_dtp2,	get_ttx(426), get_ttx(48 + skill_id));

				randval = -1;

				/* AT - value */
				sprintf(g_text_output_buf, get_ttx(427), hero->at_weapon[skill_id]);

				/* PA - value */
				sprintf(g_text_output_buf + 50,	get_ttx(428), hero->pa_weapon[skill_id]);

				do {
					randval = GUI_radio(g_dtp2, 2, g_text_output_buf, g_text_output_buf + 50);

				} while (randval == -1);

				if (randval == 1) {
					hero->at_weapon[skill_id]++;
				} else {
					hero->pa_weapon[skill_id]++;
				}
			}

		} else {
			/* fail */
			GUI_output(get_ttx(338));

			/* increment the try counter */
			g_inc_skills_counter[skill_id].tries++;
		}
	}
}

/**
 * \brief   tries to increase a skill in novice mode
 *
 *          This function is quiet and does not check how many times
 *          a skill can be increased. So the correct rules come
 *          from the array which contain the skills.
 *
 * \param   hero        pointer to the hero
 * \param   skill       number of the skill
 */
void inc_skill_novice(struct struct_hero *hero, const signed int skill_id)
{
	signed short done;
	signed short randval;

	done = 0;

	while (!done) {

		/* leave the loop if 3 incs failes or the skill value is 18 */
		if ((g_inc_skills_counter[skill_id].tries == 3) ||
			(hero->skills[skill_id] == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s kann Talent nicht weiter steigern (3 Fehlversuche oder Talentwert 18)\n", hero->alias);
#endif
		} else {

			/* dec available skill incs */
			hero->skill_incs--;

			/* check if available skill incs are 0 */
			if (!hero->skill_incs) {
				done = 1;
			}

#ifndef M302de_FEATURE_MOD
			/* on a  skill value < 11 use 2W6 else 3W6 */
			if (hero->skills[skill_id] >= 11) {
				randval = random_interval(3, 18);
			} else {
				randval = random_interval(2, 12);
			}
#else
		/* Feature mod 2:
		 * use the exact skill/spell increase mechanism as in DSA 2/3 */
			if (hero->skills[skill_id] >= 10) {
				randval = dice_roll(3,6,0);
			} else {
				randval = dice_roll(2,6,0);
			}
#endif

			/* check if increase success */
			if (hero->skills[skill_id] < randval) {

				/* inc skill value */
				hero->skills[skill_id]++;

				/* reset failed counter */
				g_inc_skills_counter[skill_id].tries = 0;

				done = 1;

				/* adjust AT PA values */
				if (skill_id <= TA_ZWEIHAENDER) {
					if (hero->at_weapon[skill_id] > hero->pa_weapon[skill_id]) {
						hero->pa_weapon[skill_id]++;
					} else {
						hero->at_weapon[skill_id]++;
					}
				}

			} else {
				/* inc failed counter */
				g_inc_skills_counter[skill_id].tries++;
			}
		}
	}
}

/**
 * \brief   tries to increase a spell in novice mode
 *
 *          This function is quiet and does not check how many times
 *          a spell can be increased. So the correct rules come
 *          from the array which contain the spells.
 *
 * \param   hero        pointer to the hero
 * \param   spell       number of the spell
 */
void inc_spell_novice(struct struct_hero *hero, const signed int spell_id)
{
	signed short done;
	signed short randval;

	done = 0;

	while (!done) {
		/* leave the loop if 3 tries to increase have failed or if the spell is already at the max value 18 */
		if ((g_inc_spells_counter[spell_id].tries == 3) ||
			(hero->spells[spell_id] == 18)) {
			done = 1;
#if !defined(__BORLANDC__)
			D1_INFO("%s kann Zauber nicht weiter steigern (3 Fehlversuche oder Talentwert 18)\n", hero->alias);
#endif
		} else {

			/* dec available spell incs */
			hero->spell_incs--;

			/* check if available spell incs are 0 */
			if (!hero->spell_incs) {
				done = 1;
			}

#ifndef M302de_FEATURE_MOD
			/* on a  spell value < 11 use 2W6 else 3W6 */
			if (hero->spells[spell_id] >= 11) {
				randval = random_interval(3, 18);
			} else {
				randval = random_interval(2, 12);
			}
#else
		/* Feature mod 2:
		 * use the exact skill/spell increase mechanism as in DSA 2/3 */
			if (hero->spells[spell_id] >= 10) {
				randval = dice_roll(3,6,0);
			} else {
				randval = dice_roll(2,6,0);
			}
#endif

			/* check if increase success */
			if (hero->spells[spell_id] < randval) {

				/* inc spell value */
				hero->spells[spell_id]++;

				/* reset failed counter */
				g_inc_spells_counter[spell_id].tries = 0;

				done = 1;

			} else {
				/* inc failed counter */
				g_inc_spells_counter[spell_id].tries++;
			}
		}
	}
}

/**
 * \brief   brings your hero to the next level
 *
 * \param   hero_pos    the position of the hero
 */
void level_up(signed short hero_pos)
{
	signed short l_si;
	signed short l_di;
	signed char mr;
	signed char v2;
	signed char v3;
	signed short i;
	struct struct_hero *hero;
	signed short city_bak;

	hero = get_hero(hero_pos);

	if (g_fading_state) {
		refresh_colors();
	}

	g_timers_disabled = 1;

	city_bak = g_text_file_index;

	load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);

	g_inc_skills_counter = (struct inc_states*)(g_buffer8_ptr + 4500);
	g_inc_spells_counter = (struct inc_states*)((Bit8u*)g_inc_skills_counter + 208);
	g_skills_buffer = (unsigned char*)g_buffer9_ptr;

	l_si = load_archive_file(ARCHIVE_FILE_BSKILLS_DAT);

	read_archive_file(l_si, g_skills_buffer, 1300);

	close(l_si);


	sprintf(g_dtp2, get_ttx(411), hero->alias);
	GUI_output(g_dtp2);

	g_action = 0;
	g_status_page_mode = 1;

	for (i = 0; i < 86; i++) {
		g_inc_spells_counter[i].tries = g_inc_spells_counter[i].incs = 0;
	}

	for (i = 0; i < 52; i++) {
		g_inc_skills_counter[i].tries = g_inc_skills_counter[i].incs = 0;
	}

	load_ggsts_nvf();

	/* increment level */
	hero->level++;

	g_status_page_hero = hero_pos;

	status_show(hero_pos);

	g_status_page_hunger = g_status_page_thirst = -1;

	update_status_bars();

	l_si = -1;

	for (i = 0; i <= 6; i++) {
		if (hero->attrib[i].normal < 20) {
			l_si = 0;
		}
	}

	if (l_si != -1) {

		l_si = -2;


		do {

			g_basepos_x = -30;

			l_si = GUI_radio(get_tx2(35), 7,
						get_ttx(412),
						get_ttx(413),
						get_ttx(414),
						get_ttx(415),
						get_ttx(416),
						get_ttx(417),
						get_ttx(418)) - 1;

			if (hero->attrib[l_si].normal >= 20) {

				l_si = -2;
				GUI_output(get_tx2(43));
			}

		} while (l_si == -2);

		hero->attrib[l_si].normal++;
		hero->attrib[l_si].current++;

		status_show(hero_pos);

		g_status_page_hunger = g_status_page_thirst = -1;

		update_status_bars();
	}

	l_si = -1;

	for (i = 7; i <= 13; i++) {

		if (hero->attrib[i].normal > 2) {
			l_si = 0;
		}
	}

	if (l_si != -1) {

		l_si = -2;

		do {

			l_si = GUI_radio(get_tx2(36), 7,
						get_ttx(419),
						get_ttx(420),
						get_ttx(421),
						get_ttx(422),
						get_ttx(423),
						get_ttx(424),
						get_ttx(425)) - 1;

			if (hero->attrib[l_si + 7].normal <= 2) {

				l_si = -2;
				GUI_output(get_tx2(48));
			}

		} while (l_si == -2);

		v3 = 0;

		for (i = 0; i < 3; i++) {

			if (random_schick(20) <= hero->attrib[l_si + 7].normal) {

				v3 = 1;
			}
		}

		if (v3 != 0) {

			hero->attrib[l_si + 7].normal--;
			hero->attrib[l_si + 7].current--;

			GUI_output(get_tx2(37));

			status_show(hero_pos);

			g_status_page_hunger = g_status_page_thirst = -1;

			update_status_bars();
		} else {

			GUI_output(get_ttx(338));
		}
	}

	/* check changes in MR */

	mr = (hero->attrib[ATTRIB_KL].normal + hero->attrib[ATTRIB_MU].normal + hero->level) / 3 - 2 * hero->attrib[ATTRIB_AG].normal;
	mr += g_mr_modificators[hero->typus];

	if (hero->mr != mr) {

		/* set new basic MR */
		hero->mr = mr;

		/* check for STIRNREIF [blau] / CORONET [blue] equipped => MR + 2 */
		if (hero->inventory[HERO_INVENTORY_SLOT_HEAD].item_id == ITEM_CORONET_BLUE) {
			hero->mr += 2;
		}
		/* check for RING / RING equipped => MR + 2 */
		if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_RING_RED) {
			hero->mr += 2;
		}
		/* check for AMULETT / in inventory => MR + 5 */
		if (get_item_pos(hero, ITEM_AMULET_BLUE) != -1) {
			hero->mr += 5;
		}

		/* show the user the new MR value */
		sprintf(g_dtp2, get_tx2(41), hero->mr);
		GUI_output(g_dtp2);

		/* update status background */
		status_show(hero_pos);

		/* update_the bars of the hero */
		g_status_page_hunger = g_status_page_thirst = -1;

		update_status_bars();
	}


	/* add skill increasements */
	hero->skill_incs += g_levelup_ta_rise[hero->typus - 1];

	/* roll how many LE points the hero may get */
	i = random_schick(6);

	if (hero->typus >= HERO_TYPE_WITCH) {
		/* a magic user */

		/* add spell increasements */
		hero->spell_incs += g_levelup_sp_rise[hero->typus - 7];

		i += 2;

		/* show how many LE the hero may get */
		sprintf(g_dtp2, get_tx2(39), i);

		do {
			l_si = GUI_input(g_dtp2, 1);

		} while (l_si < 0);

		if (l_si > i) {
			l_si = i;
		}


		/* add LE and fill them up */
		hero->le_max += l_si;
		add_hero_le(hero, l_si);
		/* add AE and fill them up */
		hero->ae_max += i - l_si;
		add_hero_ae(hero, i - l_si);

		/* change skill increasements into AE */
		if ((hero->typus == HERO_TYPE_MAGE) && (g_game_mode == GAME_MODE_ADVANCED)) {

			if (GUI_bool(get_tx2(40))) {
				/* trade 10 skill increasements into 1W6+2 AE */
				hero->spell_incs += -10;
				i = random_interval(3, 8);
				hero->ae_max += i;
				hero->ae += i;
			}
		}

	} else {
		/* not a magic user */

		/* show how many LE the hero gets */
		sprintf(g_dtp2, get_tx2(38), i);
		GUI_output(g_dtp2);

		/* add LE and fill them up */
		hero->le_max += i;
		add_hero_le(hero, i);
	}

	status_show(hero_pos);

	/* update_the bars of the hero */
	g_status_page_hunger = g_status_page_thirst = -1;

	update_status_bars();

	update_atpa(hero);

	if (g_game_mode == GAME_MODE_BEGINNER) {

		i = v2 = 0;

		while (hero->skill_incs > 0) {

			l_si = *(Bit16s*)(g_skills_buffer + 100 * hero->typus + 4 * i);

			if (hero->skills[l_si] < *(Bit16s*)(g_skills_buffer + 100 * hero->typus + 4 * i + 2))
			{
				inc_skill_novice(hero, l_si);
			}

			i++;

			if (i > 24) {

				i = 0;

				v2++;

				if (v2 > 5) {
					hero->skill_incs = 0;
				}

			}
		}

		if (hero->typus >= HERO_TYPE_WITCH) {
			/* hero has magic type */

			i = 1;
			v2 = 0;

			while (hero->spell_incs != 0 && v2 < 3) {

				v2++;

				switch (hero->typus) {

					case HERO_TYPE_WITCH: {

						/* first try to increase all which-specific spells by 1,
						 * up to skill value at most 11. */
						while (hero->spell_incs != 0 && i < 86) {

							if (g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_WITCH && hero->spells[i] < 11) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						/* if there are attempts left, try to increase spells from AUTOINC_SPELLS_WITCH list by 1,
						 * up to max value 18 */
						i = 0;

						while (hero->spell_incs != 0 && (g_autoinc_spells_witch[i] != -1)) {
							inc_spell_novice(hero, g_autoinc_spells_witch[i]);
							i++;
						}

						break;
					}
					case HERO_TYPE_DRUID: {

						while (hero->spell_incs != 0 && i < 86) {

							if (g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_DRUID && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (hero->spell_incs != 0 && (g_autoinc_spells_druid[i] != -1)) {
							inc_spell_novice(hero, g_autoinc_spells_druid[i]);
							i++;
						}

						break;
					}
					case HERO_TYPE_MAGE: {

						i = 0;

						while (hero->spell_incs != 0 && (g_magic_schools_index[hero->spell_school][i] != -1)) {

							if (hero->spells[g_magic_schools_index[hero->spell_school][i]] < 11)
							{
								inc_spell_novice(hero, g_magic_schools_index[hero->spell_school][i]);
							}
							i++;
						}

						i = 0;
						while (hero->spell_incs != 0 && (g_autoinc_spells_mage_index[hero->spell_school][i] != -1)) {

							if (hero->spells[g_autoinc_spells_mage_index[hero->spell_school][i]] < 11)
							{
								inc_spell_novice(hero, g_autoinc_spells_mage_index[hero->spell_school][i]);
							}
							i++;
						}

						i = 0;
						while ((hero->spell_incs) != 0 && (g_magic_schools_index[hero->spell_school][i] != -1)) {

							if (hero->spells[g_magic_schools_index[hero->spell_school][i]] < 11)
							{
								inc_spell_novice(hero, g_magic_schools_index[hero->spell_school][i]);
							}
							i++;
						}

						break;
					}
					case HERO_TYPE_GREEN_ELF: {

						while (hero->spell_incs != 0 && i < 86) {

							if ((g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_GELF) && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (hero->spell_incs != 0 && (g_autoinc_spells_gelf[i] != -1)) {
							inc_spell_novice(hero, g_autoinc_spells_gelf[i]);
							i++;
						}

						i = 1;
						while (hero->spell_incs != 0 && i < 86) {

							if (g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_GELF && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						break;
					}
					case HERO_TYPE_ICE_ELF: {

						while (hero->spell_incs != 0 && i < 86) {

							if (g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_IELF && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while (hero->spell_incs != 0 && (g_autoinc_spells_ielf[i] != -1)) {
							inc_spell_novice(hero, g_autoinc_spells_ielf[i]);
							i++;
						}

						i = 1;
						while (hero->spell_incs != 0 && i < 86) {

							if (g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_IELF && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
					case HERO_TYPE_SYLVAN_ELF: {

						while (hero->spell_incs != 0 && i < 86) {

							if ((g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_SELF) && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}

						i = 0;

						while ((hero->spell_incs != 0) && (g_autoinc_spells_self[i] != -1)) {
							inc_spell_novice(hero, g_autoinc_spells_self[i]);
							i++;
						}

						i = 1;
						while ((hero->spell_incs != 0) && (i < 86)) {

							if ((g_spell_descriptions[i].herotype == SPELL_DESC_HEROTYPE_SELF) && (hero->spells[i] < 11)) {
								inc_spell_novice(hero, i);
							}
							i++;
						}
						break;
					}
				}
			}

			while (hero->spell_incs != 0) {
				inc_spell_novice(hero, random_schick(86));
			}
		}

	} else {

		if ((hero->typus >= HERO_TYPE_WITCH) && (l_di = g_levelup_spta_conv[hero->typus - 7]) && GUI_bool(get_tx2(45)))
		{

			sprintf(g_dtp2, get_tx2(46), l_di);

			i = GUI_input(g_dtp2, 1);

			if (i > 0) {

				if (i > l_di) {
					i = l_di;
				}
				l_di -= i;

				hero->spell_incs -= i;
				hero->skill_incs += i;

			} else {

				sprintf(g_dtp2, get_tx2(47), l_di);

				i = GUI_input(g_dtp2, 1);

				if (i > 0) {

					if (i > l_di) {
						i = l_di;
					}

					hero->spell_incs += i;
					hero->skill_incs -= i;
				}
			}
		}

		while (hero->skill_incs != 0) {

			l_si = LVL_select_skill(hero, 1);

			if (l_si >= 0) {

				inc_skill_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_tx2(42))) {
					break;
				}
			}
		}

		while (hero->spell_incs != 0) {

			l_si = select_spell(hero, 1);

			if (l_si > 0) {

				inc_spell_advanced(hero, l_si);

			} else if (l_si == -2) {

				if (GUI_bool(get_tx2(42))) {
					break;
				}
			}
		}
	}

	g_basepos_x = 0;
	g_request_refresh = 1;
	g_timers_disabled = 0;

	g_current_ani = -1;

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (city_bak != -1 && city_bak != ARCHIVE_FILE_CHARTEXT_LTX
	    && city_bak != ARCHIVE_FILE_TAVERN_TLK
	    && (city_bak < 156 || city_bak > 176)) {
		load_tx2(city_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
