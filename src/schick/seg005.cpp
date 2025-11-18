/*
 *	Rewrite of DSA1 v3.02_de functions of seg005 (fight)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg005.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#if defined(__BORLANDC__)
#include <DOS.H>
#include <IO.H>
#else
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
#endif

#include "v302de.h"
#include "common.h"

#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg005.h"
#include "seg006.h"
#include "seg027.h"
#include "seg032.h"
#include "seg096.h"


#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed int g_delay_factor = 4; // ds:0x4b66
extern char g_str_temp_xx[8];
static char *g_str_temp_xx_ptr = (char*)&g_str_temp_xx[0]; // ds:0x4b68, to STR_TEMP_XX; uint8_t*
static const signed char g_fig_star_colors[12] = { 0x03, 0x03, 0x0c, 0x0c, 0x04, 0x0b, 0x0d, 0x01, 0x07, 0x0e, 0x02, 0x07 }; // ds:0x4b6c
static signed char g_fig_star_counter = 0; // ds:0x4b78
signed int g_fig_star_timer = 0; // ds:0x4b79
static signed char g_fig_star_last_count = -1; // ds:0x4b7b
static const signed int g_fig_msg_dtps[12] = { 0x36, 0x37, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x00, 0x00, 0x3b, 0x00 }; // ds:0x4b7c
signed char g_fig_star_printed = 0; // ds:0x4b94
char g_str_temp_xx[8] = "TEMP\\XX"; // ds:0x4b95

static unsigned char* g_fig_gfxbuffers[8];	// ds:0xe278, 0x508 byte segments in FIGHTOBJ_BUF
static signed int g_fig_figlist_readd[8];	// ds:0xe298
static signed int g_fig_ani_state[8];		// ds:0xe2a8
struct_msg g_fig_actor_grammar;			// ds:0xe2ba
struct_msg g_fig_target_grammar;		// ds:0xe2be
static signed int g_figobj_unkn_y1;		// ds:0xe2c0
static signed int g_figobj_unkn_y1_bak;		// ds:0xe2c2
static signed int g_figobj_unkn_x1;		// ds:0xe2c4
static signed int g_figobj_unkn_x1_bak;		// ds:0xe2c6
static signed int g_figobj_unkn_y2;		// ds:0xe2c8
static signed int g_figobj_unkn_y2_bak;		// ds:0xe2ca
static signed int g_figobj_unkn_x2;		// ds:0xe2cc
static signed int g_figobj_unkn_x2_bak;		// ds:0xe2ce
signed int g_delay_timer;			// ds:0xe2d0

/* static prototypes */
static void set_delay_timer(void);
static void fight_delay(void);


/**
 * \param     fighter pointer to a fighter object
 * \param[in] x       x coordinate on the screen
 * \param[in] y       y coordinate on the screen
 */
static signed int FIG_obj_needs_refresh(struct struct_fighter *fighter, const signed int x, const signed int y)
{
	if (fighter->visible) {

		struct struct_fighter *list_i;
		signed int ox;
		signed int oy;

		/* animated objects always need a refresh */
		if ((fighter->sheet != -1) || (fighter->visible == 3))
			goto damn_label;

		/* i = i->next; */
		/* check if given object overlaps with any of the objects behind it */
		for (list_i = g_fig_list_head; list_i != fighter; list_i = list_i->next)
		{
			/* Ignore invisible objects or objects, that are not refreshed */
			if (list_i->visible >= 2) {

				ox = 10 - list_i->width / 2 + (list_i->cbx + list_i->cby) * 10;

				oy = 118 - list_i->height + (list_i->cbx - list_i->cby) * 5;

				ox += list_i->offsetx;
				oy += list_i->offsety;

				if ((x + fighter->width >= ox) && (x - list_i->width <= ox) && (y + fighter->height >= oy) && (y - list_i->height <= oy))
				{
damn_label:
					if (fighter->visible == 1)
						fighter->visible = 2;

					return 1;
				}
			}
		}
	}

	return 0;
}

/**
 * \brief   set the color of the star in fights
 *
 *          Sets the color of the star which shows fight activities,
 *          like damage, in fights.
 *
 * \param   ptr         pointer to the star template
 * \param   count       number of bytes the star has
 * \param   color       1=red/2=green/3=blue/4=yellow/11=darkbrown/12=lightbrown/13=pink
 *
 */
static void FIG_set_star_color(uint8_t *ptr, signed int count, signed char color)
{
	uint8_t *p;

	color += 0x80;

	/* REMARK: memset(ptr, color + 0x80, count); */
	for (p = ptr; count--; p++) {
		if (*p) *p = color;
	}
}

/**
 * \brief   returns name in the 3rd german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "einem Magier" if the enemy is a "Magier".
 */
static char* FIG_name_3rd_case(const signed int type, volatile const signed int pos)
{
	if (type == 2) {
		return get_hero(pos)->alias;
	} else {
		return (char*)GUI_names_grammar(3, pos, 1);
	}
}

/**
 * \brief   returns name in the 4th german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "einen Magier" if the enemy is a "Magier".
 */
static char* FIG_name_4th_case(const signed int type, volatile const signed int pos)
{
	if (type == 2) {
		return get_hero(pos)->alias;
	} else {
		return (char*)GUI_names_grammar(2, pos, 1);
	}
}

/**
 * \brief   returns name in the 1st german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "ein Magier" if the enemy is a "Magier".
 */
static char *FIG_name_1st_case(const signed int type, volatile const signed int pos)
{
	if (type == 2) {
		return get_hero(pos)->alias;
	} else {
		return (char*)GUI_names_grammar(0, pos, 1);
	}
}

#define idx (g_fig_msg_dtps[g_fig_msg_data[g_fig_star_counter].f_action - 1])

static signed int fight_printer(void)
{
	if (!g_fig_msg_data[0].f_action) {
		g_fig_continue_print = 0;
	}

	if (!g_fig_star_timer && g_fig_star_printed) {

		g_fig_star_counter++;

		g_fig_star_printed = 0;

		g_fig_star_timer = g_autofight ? 10 : g_delay_factor * 6;

		if (!g_fig_msg_data[g_fig_star_counter].f_action) {
			g_fig_continue_print = 0;
		}
	}

	if (g_fig_continue_print) {

		if (g_fig_star_counter != g_fig_star_last_count) {

			signed int fg_bak;
			signed int bg_bak;
			uint8_t* gfx_pos_bak;
			uint16_t x;
			char str[6];
			uint8_t* gfx_dst_bak;
			int16_t f_action;

			g_fig_star_printed = 1;

			f_action = g_fig_msg_data[g_fig_star_counter].f_action;

			if (f_action) {

				gfx_pos_bak = g_vga_backbuffer;

				g_vga_backbuffer = g_renderbuf_ptr;

				get_textcolor(&fg_bak, &bg_bak);

				FIG_set_star_color(g_fig_star_gfx, 3724, g_fig_star_colors[f_action - 1]);

				g_pic_copy.x1 = 0;
				g_pic_copy.y1 = 150;
				g_pic_copy.x2 = 75;
				g_pic_copy.y2 = 198;
				g_pic_copy.src = g_fig_star_gfx;
				g_pic_copy.dst = g_renderbuf_ptr;
				gfx_dst_bak = g_pic_copy.dst;
				do_pic_copy(2);

				g_pic_copy.dst = gfx_dst_bak;

				/* print number into the star */
				if (g_fig_msg_data[g_fig_star_counter].damage) {

					set_textcolor(0xff, g_fig_star_colors[f_action - 1] + 0x80);

					my_itoa(g_fig_msg_data[g_fig_star_counter].damage, str, 10);

					x = GUI_get_first_pos_centered(str, 30, 20, 0);
					GUI_print_string(str, x, 170);
				}

				/* Generate textmessage */
				if (g_fig_msg_dtps[f_action - 1]) {

					g_pic_copy.x1 = g_pic_copy.v1 = 0;
					g_pic_copy.y1 = g_pic_copy.v2 = 194;
					g_pic_copy.x2 = 318;
					g_pic_copy.y2 = 199;
					g_pic_copy.src = g_buffer8_ptr;
					do_pic_copy(3);

					set_textcolor(0xff, 0);

					if (f_action == 1 || f_action == 3) {
//						case 1:	/* heroes attack fails */
//						case 3: /* enemy attack fails */

						sprintf(g_text_output_buf, get_tx(idx),
							FIG_name_3rd_case(g_fig_actor_grammar.type, g_fig_actor_grammar.id));

					} else if (f_action == 2 || f_action == 4 || f_action == 7) {
//						case 2: /* hero parade fails */
//						case 4: /* enemy parade fails */
//						case 7:	/* hero get unconscious */

						sprintf(g_text_output_buf, get_tx(idx),
							FIG_name_3rd_case(g_fig_target_grammar.type, g_fig_target_grammar.id));



					} else if (f_action == 8 || f_action == 11) {
//						case 8:		/* enemy hits hero */
//						case 11:	/* hero hits enemy */

						sprintf(g_text_output_buf, get_tx(idx),
							FIG_name_1st_case(g_fig_actor_grammar.type, g_fig_actor_grammar.id),
							FIG_name_4th_case(g_fig_target_grammar.type, g_fig_target_grammar.id));
					} else {
						/* case 5: hero successful parade */
						/* case 6: weapon broke */
						strcpy(g_text_output_buf, get_tx(idx));
					}

					GUI_print_string(g_text_output_buf, 1, 194);
				}

				g_vga_backbuffer = gfx_pos_bak;
				set_textcolor(fg_bak, bg_bak);
			}

			g_fig_star_last_count = g_fig_star_counter;

			return 1;
		} else {
			return 0;
		}
	} else {
		g_fig_star_counter = 0;

		g_fig_star_timer = g_autofight ? 10 : g_delay_factor * 6;

		g_fig_star_last_count = -1;

		return 0;
	}

}
#undef idx

void draw_fight_screen(const signed int val)
{
	signed int i;
	signed int object_id_bak;
	signed int width;
	signed int flag;
	signed int current_x1;
	signed int current_y1;
	signed int obj_x;
	signed int obj_y;

	uint8_t* p_figure_gfx;
	uint8_t* p_weapon_gfx;
	struct struct_fighter *p_fighter;

	struct struct_rect rect_bak;
	struct struct_hero *hero;
	struct enemy_sheet *p_enemy_sheet;

	signed int viewdir_before;
	signed int viewdir_after;
	signed int object_id;
	signed char double_size_move_tail_first;
	struct struct_fighter *p_fighter_tmp;
	signed int viewdir_unconsc;
	int8_t *sheet;
	int8_t *p_weapon_anisheet;
	signed int handle;
	struct nvf_extract_desc nvf;
	signed int figlist_remove[8];

	call_mouse_bg();

	p_fighter = (struct struct_fighter*)g_fig_list_head;

	do {
		/* Check for each list entry if a sprite is needed */

		if (p_fighter->reload == -1) {

			nvf.src = (uint8_t*)load_fight_figs(p_fighter->figure);
			nvf.dst = p_fighter->gfxbuf;
			nvf.image_num = p_fighter->nvf_no;
			nvf.compression_type = 0;
			nvf.width = &width;
			nvf.height = &object_id_bak;

			process_nvf_extraction(&nvf);

			p_fighter->reload = 0;
		}

	} while ((p_fighter = p_fighter->next));

	/* set elements array[0] of array[9] */
	g_fig_gfxbuffers[0] = g_fightobj_buf_seek_ptr;
	g_fig_ani_state[0] = -1;

	for (i = 1; i < 8; i++) {
		/* copy a pointer to the next position */
		g_fig_gfxbuffers[i] = (HugePt)g_fig_gfxbuffers[i - 1] + 0x508L;
		g_fig_ani_state[i] = -1;
	}

	p_fighter = (struct struct_fighter*)g_fig_list_head;
	flag = 0;

	do {

#if !defined(__BORLANDC__)
		D1_LOG(" loop Figure = %3d Sheet_ID : %d 0xf : %d 0x12: %d object: %d\n",
				p_fighter->figure, p_fighter->sheet, p_fighter->wsheet,
				p_fighter->visible, p_fighter->object_id);
#endif

		if (p_fighter->sheet != -1) {
			/* Has a sheet id */

			if (p_fighter->visible) {
				p_fighter->visible = 3;
			}

			flag = 1;

			g_fig_ani_state[p_fighter->sheet] = 0;

			memcpy((uint8_t*)g_fig_gfxbuffers[p_fighter->sheet],
				p_fighter->gfxbuf,
				p_fighter->width * p_fighter->height);
		}

		if (p_fighter->wsheet != -1) {
			memset((uint8_t*)g_fig_gfxbuffers[p_fighter->wsheet], 0, 0x508);
		}


	} while ((p_fighter = p_fighter->next));

	/* write TEMP/XX */
	/* TODO: should be O_BINARY | O_WRONLY */
	handle = _creat(g_str_temp_xx_ptr, 0);
	write(handle, g_buffer8_ptr, 64000);
	close(handle);

	if (flag != 0) {

		g_figobj_unkn_x2_bak = g_figobj_unkn_x1_bak = -1;

		FIG_draw_pic();

		FIG_draw_figures();

		memcpy(g_buffer8_ptr, g_renderbuf_ptr, 64000);
	}

	while (flag != 0) {

		set_delay_timer();

		g_pic_copy.dst = g_vga_backbuffer = g_renderbuf_ptr;

		for (p_fighter = (struct struct_fighter*)g_fig_list_head; p_fighter; p_fighter = p_fighter->next) {
			if (p_fighter->visible == 2)
				p_fighter->visible = 1;
		}

		for (i = 0; i < 8; i++) {
			g_fig_figlist_readd[i] = -1;
			figlist_remove[i] = -1;
		}

		rect_bak = g_pic_copy_rect;
		g_pic_copy.x1 = g_pic_copy.v1 = 0;
		g_pic_copy.y1 = g_pic_copy.v2 = 0;
		g_pic_copy.x2 = 318;
		g_pic_copy.y2 = 149;
		g_pic_copy.src = g_buffer8_ptr;
		do_pic_copy(3);

		g_pic_copy.x1 = g_pic_copy.v1 = 80;
		g_pic_copy.y1 = g_pic_copy.v2 = 150;
		g_pic_copy.x2 = 318;
		g_pic_copy.y2 = 193;
		g_pic_copy.src = g_buffer8_ptr;
		do_pic_copy(3);

		p_fighter = (struct struct_fighter*)g_fig_list_head;

		do {
			p_weapon_gfx = 0;	/* NULL */

			obj_x = 10 - (p_fighter->width / 2) +
				(10 * (p_fighter->cbx + p_fighter->cby));

			obj_y = 118 - p_fighter->height + (p_fighter->cbx - p_fighter->cby) * 5;

			obj_x += p_fighter->offsetx;
			obj_y += p_fighter->offsety;

			p_figure_gfx = p_fighter->gfxbuf;

			if ((p_fighter->sheet != -1) && (g_fig_ani_state[p_fighter->sheet] != -1)) {

				sheet = g_fig_anisheets[p_fighter->sheet];

				if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -1) {

					p_figure_gfx = (uint8_t*)g_fig_gfxbuffers[p_fighter->sheet];
					g_fig_ani_state[p_fighter->sheet] = -1;
					p_fighter->sheet = p_fighter->wsheet = -1;

				} else {

					if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -7)
					{
						p_fighter->z = (*(sheet + 2 + g_fig_ani_state[p_fighter->sheet] * 3));

						g_fig_ani_state[p_fighter->sheet]++;

						g_fig_figlist_readd[p_fighter->sheet] = p_fighter->id;
					}

					if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -9)
					{
						play_voc(0xc8 + *(sheet + 2 + 3 * g_fig_ani_state[p_fighter->sheet]));
						g_fig_ani_state[p_fighter->sheet]++;
					}


					if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -5) {

						if (p_fighter->wsheet != -1) {

							p_weapon_anisheet = &g_fig_anisheets[p_fighter->wsheet][0];

							if (*(p_weapon_anisheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -9)
							{
								play_voc(0xc8 + *(p_weapon_anisheet + 2 + 3 * g_fig_ani_state[p_fighter->sheet]));
							}
						}

						g_fig_ani_state[p_fighter->sheet]++;

					} else if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -4) {

						*sheet = *(sheet + 2 + 3 * g_fig_ani_state[p_fighter->sheet]);

						g_fig_ani_state[p_fighter->sheet]++;

					} else if ((*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -3) ||
							(*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -6)) {

						/* get nvf no */
						viewdir_before = p_fighter->nvf_no;

						if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -3) {

							p_fighter->nvf_no += *(sheet + 3 + 3 * g_fig_ani_state[p_fighter->sheet]);
						} else {

							p_fighter->figure = g_gfxtab_figures_main[p_fighter->sprite_id][*(sheet + 2 + 3 * g_fig_ani_state[p_fighter->sheet])];
							p_fighter->nvf_no = *(sheet + 3 + 3 * g_fig_ani_state[p_fighter->sheet]);
						}

						if (p_fighter->figure >= 88) {
							/* fighter uses figure from MONSTER file */

							if (p_fighter->nvf_no > 3) {

								/* not standing still */
								p_fighter->offsetx = g_gfxtab_offsets_main[p_fighter->sprite_id][4].x;
								p_fighter->offsety = g_gfxtab_offsets_main[p_fighter->sprite_id][4].y;

								if (p_fighter->double_size != -1) {
									p_fighter->x1 = g_gfxtab_double_size_x1[1];
									p_fighter->x2 = g_gfxtab_double_size_x2[1];
								}

							} else {
								p_fighter->offsetx = g_gfxtab_offsets_main[p_fighter->sprite_id][p_fighter->nvf_no].x;
								p_fighter->offsety = g_gfxtab_offsets_main[p_fighter->sprite_id][p_fighter->nvf_no].y;

								if (p_fighter->double_size != -1) {
									p_fighter->x1 = g_gfxtab_double_size_x1[p_fighter->nvf_no];
									p_fighter->x2 = g_gfxtab_double_size_x2[p_fighter->nvf_no];
								}
							}

						} else {
							if (p_fighter->nvf_no == g_nvftab_figures_dead[p_fighter->sprite_id]) {

								p_fighter->offsetx = g_gfxtab_offsets_main[p_fighter->sprite_id][4].x;
								p_fighter->offsety = g_gfxtab_offsets_main[p_fighter->sprite_id][4].y;

							} else {
								viewdir_unconsc = p_fighter->nvf_no - g_nvftab_figures_unconscious[p_fighter->sprite_id];

								if (viewdir_unconsc >= 0) {

									p_fighter->offsetx = g_gfxtab_offsets_unconscious[p_fighter->sprite_id][viewdir_unconsc].x;
									p_fighter->offsety = g_gfxtab_offsets_unconscious[p_fighter->sprite_id][viewdir_unconsc].y;
								}
							}
						}

						obj_x = 10 - (p_fighter->width / 2) + (10 * (p_fighter->cbx + p_fighter->cby));

						obj_y = 118 - p_fighter->height + ((p_fighter->cbx - p_fighter->cby) * 5);

						obj_x += p_fighter->offsetx;
						obj_y += p_fighter->offsety;

						if ((p_fighter->sheet < 6) && (*(sheet + 0xf2) >= 0)) {
							nvf.src = (uint8_t*)load_fight_figs(p_fighter->figure);
						} else {
							nvf.src = g_spellobj_nvf_buf;
						}

						nvf.dst = p_fighter->gfxbuf;
						nvf.image_num = p_fighter->nvf_no;
						nvf.compression_type = 0;
						nvf.width = &width;
						nvf.height = &object_id_bak;

						process_nvf_extraction(&nvf);

						g_fig_ani_state[p_fighter->sheet]++;

						p_figure_gfx = p_fighter->gfxbuf;

						if (p_fighter->double_size > 20) {
							/* p_fighter point to the fighter entry of the tail of a double-size enemy */

							viewdir_after = (p_fighter->nvf_no > 3) ? 1 : p_fighter->nvf_no;

							p_fighter->offsetx += g_gfxtab_double_size_extra_ox[viewdir_after];
							p_fighter->offsety += g_gfxtab_double_size_extra_oy[viewdir_after];
							p_fighter->x1 = g_gfxtab_double_size_extra_x1[viewdir_after];
							p_fighter->x2 = g_gfxtab_double_size_extra_x2[viewdir_after];

							object_id_bak = get_cb_val(p_fighter->cbx, p_fighter->cby); /* enemy_id + 30 of the enemy the tail belongs to */

							FIG_set_cb_object(p_fighter->cby, p_fighter->cbx, p_fighter->object_id);

							/* update CBX depending on the view direction */
							if ( ((viewdir_after == 2) && ((viewdir_before == 1) || (viewdir_before == 3))) ||
								(((viewdir_after == 3) || (viewdir_after == 1)) && (viewdir_before == 0)))
							{
								p_fighter->cbx++;

							} else if ( ((viewdir_after == 0) && ((viewdir_before == 1) || (viewdir_before == 3))) ||
									(((viewdir_after == 3) || (viewdir_after == 1)) && (viewdir_before == 2)))
							{
								p_fighter->cbx--;
							}

							/* update CBY depending on the view direction */
							if ( ((viewdir_after == 3) && ((viewdir_before == 0) || (viewdir_before == 2))) ||
								(((viewdir_after == 0) || (viewdir_after == 2)) && (viewdir_before == 1)))
							{
								p_fighter->cby--;

							} else	if ( ((viewdir_after == 1) && ((viewdir_before == 0) || (viewdir_before == 2))) ||
									(((viewdir_after == 0) || (viewdir_after == 2)) && (viewdir_before == 3)))
							{
								p_fighter->cby++;

							} else if ((viewdir_after == 1) && (viewdir_before == 3))
							{
								p_fighter->cby = (p_fighter->cby + 2);
							}

							object_id = get_cb_val(p_fighter->cbx, p_fighter->cby); /* object id of the square the tail moves to */
							p_fighter->object_id = object_id; /* move it to fighter.object_id */
							FIG_set_cb_object(p_fighter->cby, p_fighter->cbx, object_id_bak); /* set object id of the target square to enemy_id + 30 */

							obj_x = 10 - (p_fighter->width / 2) + (10 * (p_fighter->cbx + p_fighter->cby));

							obj_y = 118 - p_fighter->height + ((p_fighter->cbx - p_fighter->cby) * 5);

							obj_x += p_fighter->offsetx;

							obj_y += p_fighter->offsety;

						}
					} else {

						/* move a hero/enemy */
						if (*(sheet + 1 + 3 * g_fig_ani_state[p_fighter->sheet]) == -2) {

							if (p_fighter->sheet < 6) {

								object_id_bak = get_cb_val(p_fighter->cbx, p_fighter->cby);

								/* copy fighter.object_id back to the chessboard */
								FIG_set_cb_object(p_fighter->cby, p_fighter->cbx, p_fighter->object_id);

								p_fighter->cbx = (p_fighter->cbx + *(sheet + 2 + 3 * g_fig_ani_state[p_fighter->sheet]));

								p_fighter->cby = (p_fighter->cby + *(sheet + 3 + 3 * g_fig_ani_state[p_fighter->sheet]));

								double_size_move_tail_first = 0;

								/* get the value from the cb where the actor wants to move to */
								object_id = get_cb_val(p_fighter->cbx, p_fighter->cby);

								if ((p_fighter->double_size > 20) && (object_id_bak - 20 == object_id)) {
									/* for a double-size enemy, either the head part or the tail part is moved first.
									 * This is the case that the tail part is moved first (the target square is the head part). */

#ifndef M302de_ORIGINAL_BUGFIX
									/* Original-Bug 5: */
									/* the removal of the following line is not strictly necessary, but it is not needed as a replacement is added further below. */
									p_fighter->object_id = 0;
#endif
									double_size_move_tail_first = 1;

									/* create pointer to the head part of the enemy */
									p_fighter_tmp = FIG_get_fighter(g_enemy_sheets[object_id - 10].fighter_id);

#ifdef M302de_ORIGINAL_BUGFIX
									/* Original-Bug 5: */
									/* The fighter.object_id entry of the head part will be overwritten by the next line in the original code.
									 * In this way, sometimes dead bodies are lost from the chessboard after a double-size enemy walks over it.
									 * The right thing is to copy it to the fighter.object_id of tail part. */
									p_fighter->object_id = p_fighter_tmp->object_id;
#endif
									p_fighter_tmp->object_id = object_id_bak;
									/* write cb_id of the tail part at fighter.object_id of the head part.
									 * when the head part moves lated, it will be written to the cb.
									 * possible bug: the overwritten fighter.object_id is lost! */
								} else {
									p_fighter->object_id = object_id;
								}

								/* check chessboard bounds */
								if ( (p_fighter->cbx >= 24) || (p_fighter->cby >= 24)
									|| (p_fighter->cbx < 0) || (p_fighter->cby < 0)
									|| (p_fighter->object_id < 0))
								{
									/* hero/enemy escapes */

										if (p_fighter->is_enemy == 1) {
											/* enemy escapes */
											p_enemy_sheet = FIG_get_enemy_sheet(p_fighter->id);
											if (p_enemy_sheet) {
												p_enemy_sheet->flags.dead = 1;
												p_enemy_sheet->bp = 0;
												figlist_remove[p_fighter->sheet] = p_enemy_sheet->fighter_id;

												if (p_fighter->double_size != -1) {
#ifdef M302de_ORIGINAL_BUGFIX
													/* Original-Bug 4:
													 * remove tail of the escaped double-size enemy from the chessboard
													 * For more on this bug, see Original-Bug 3 at seg032.cpp */
													p_fighter_tmp = FIG_get_fighter(g_fig_double_size_fighter_id_table[p_fighter->double_size]);
													FIG_set_cb_object(p_fighter_tmp->cby, p_fighter_tmp->cbx, p_fighter_tmp->object_id);
#endif
													figlist_remove[2 + p_fighter->sheet] = g_fig_double_size_fighter_id_table[p_fighter->double_size];
												}
											}
										} else {
											/* hero escapes */
											/* REMARK: hero is always != NULL */
											hero = FIG_get_hero_ptr(p_fighter->id);
											if (hero) {
												hero->action_id = FIG_ACTION_FLEE;
												hero->flags.scared = 1;

												/* set the dungeon position the hero escapes to.
												 * This depends on the direction the escape square on the battlefield has been entered.
												 * Note: Apparently, this is done in any fight, including seafights and wilderness fights where it doesn't make sense.
												 * The distinction is done only later. */
												hero->escape_position =
													g_fig_escape_position[hero->viewdir == 3 ? NORTH : (hero->viewdir + 1)];
												figlist_remove[p_fighter->sheet] = hero->fighter_id;

											}
										}

										p_fighter->object_id = 0;
										*((int8_t*)(sheet + 1 + 3 * (1 + g_fig_ani_state[p_fighter->sheet]))) = -1;

										if (p_fighter->double_size != -1) {

											g_fig_anisheets[p_fighter->sheet + 2][4 + 3 * g_fig_ani_state[2 + p_fighter->sheet]] = -1;

											g_fig_ani_state[2 + p_fighter->sheet] = -1;
										}

								} else {
									if (!double_size_move_tail_first) {
										FIG_set_cb_object(p_fighter->cby, p_fighter->cbx, object_id_bak);
									}
								}
							} else {
								p_fighter->cbx = (p_fighter->cbx +
									*(sheet + g_fig_ani_state[p_fighter->sheet] * 3 + 2));

								p_fighter->cby = (p_fighter->cby +
									*(sheet + g_fig_ani_state[p_fighter->sheet] * 3 + 3));
							}

							g_fig_figlist_readd[p_fighter->sheet] = p_fighter->id;

							g_fig_ani_state[p_fighter->sheet]++;
						}

						if (*(sheet + 1 + (g_fig_ani_state[p_fighter->sheet] * 3)) == -1) {

							p_figure_gfx = (uint8_t*)g_fig_gfxbuffers[p_fighter->sheet];
							g_fig_ani_state[p_fighter->sheet] = -1;
							p_fighter->sheet = p_fighter->wsheet = -1;

						} else {
							obj_x = 10 - (p_fighter->width / 2) + (10 * (p_fighter->cbx + p_fighter->cby));

							obj_y = 118 - p_fighter->height + ((p_fighter->cbx - p_fighter->cby) * 5);

							obj_x += p_fighter->offsetx;

							obj_y += p_fighter->offsety;

							obj_x += *(sheet + 2 + g_fig_ani_state[p_fighter->sheet] * 3);

							obj_y -= *(sheet + 3 + g_fig_ani_state[p_fighter->sheet] * 3);

							i = g_gfxtab_figures_main[p_fighter->sprite_id][*(sheet)];

							if ((p_fighter->sheet < 6) && (*(sheet + 0xf2) >= 0)) {
								nvf.src = (uint8_t*)load_fight_figs(i);
							} else {
								nvf.src = g_spellobj_nvf_buf;
							}

							nvf.dst = (uint8_t*)g_fig_gfxbuffers[p_fighter->sheet];
							nvf.image_num = *(sheet + 1 + g_fig_ani_state[p_fighter->sheet] * 3);
							nvf.compression_type = 0;
							nvf.width = &width;
							nvf.height = &object_id_bak;

							process_nvf_extraction(&nvf);

							if (p_fighter->wsheet != -1) {

								p_weapon_anisheet = &g_fig_anisheets[p_fighter->wsheet][0];

								if (*(p_weapon_anisheet + 1 + g_fig_ani_state[p_fighter->sheet] * 3) == -1)
								{
									p_fighter->wsheet = -1;
								} else {
									current_x1 = obj_x;
									current_y1 = obj_y;

									p_weapon_gfx = (uint8_t*)g_fig_gfxbuffers[p_fighter->wsheet];

									if (*(p_weapon_anisheet + 1 + 3 * (g_fig_ani_state[p_fighter->sheet])) != -5) {
										nvf.dst = (uint8_t*)g_fig_gfxbuffers[p_fighter->wsheet];
										nvf.src = g_weapons_nvf_buf;
										nvf.image_num = *(uint8_t*)(p_weapon_anisheet + 1 + g_fig_ani_state[p_fighter->sheet] * 3);
										nvf.compression_type = 0;
										nvf.width = &width;
										nvf.height = &object_id_bak;

										process_nvf_extraction(&nvf);

										current_x1 += p_fighter->width - 14;
										current_x1 += *(p_weapon_anisheet + 2 + g_fig_ani_state[p_fighter->sheet] * 3);
										current_y1 -= *(p_weapon_anisheet + 3 + g_fig_ani_state[p_fighter->sheet] * 3);
									}
								}
							}

							g_fig_ani_state[p_fighter->sheet]++;
						}

						if (p_fighter->sheet != -1) {
							p_figure_gfx = (uint8_t*)g_fig_gfxbuffers[p_fighter->sheet];
						}
					}
				}
			}
/* 0x17e5 */

			if (FIG_obj_needs_refresh(p_fighter, obj_x, obj_y)) {

				if ((p_fighter->sheet == -1) || (figlist_remove[p_fighter->sheet] == -1)) {

					if (p_fighter->sheet != -1) {

						g_figobj_unkn_x1 = g_figobj_unkn_x2;
						g_figobj_unkn_y1 = g_figobj_unkn_y2;

						g_figobj_unkn_x2 = obj_x - 8;
						g_figobj_unkn_y2 = obj_y - 8;
					}

					/* set Y1 */
					g_pic_copy_rect.y1 = obj_y + p_fighter->y1;
					if (g_pic_copy_rect.y1 < 0) g_pic_copy_rect.y1 = 0;

					/* set X1 */
					g_pic_copy_rect.x1 = obj_x + p_fighter->x1;
					if (g_pic_copy_rect.x1 < 0) g_pic_copy_rect.x1 = 0;

					/* set Y2 */
					g_pic_copy_rect.y2 = obj_y + p_fighter->y2;
					if (g_pic_copy_rect.y2 > (200 - 1)) g_pic_copy_rect.y2 = (200 - 1);

					/* set X2 */
					g_pic_copy_rect.x2 = obj_x + p_fighter->x2;
					if (g_pic_copy_rect.x2 > (320 - 2)) g_pic_copy_rect.x2 = (320 - 2);

					g_pic_copy.x1 = obj_x;
					g_pic_copy.y1 = obj_y;
					g_pic_copy.x2 = obj_x + p_fighter->width - 1;
					g_pic_copy.y2 = obj_y + p_fighter->height - 1;
					g_pic_copy.src = p_figure_gfx;

					do_pic_copy(2);	/* Critical */
				}


				/* NULL check on uint8_t* */
				if (p_weapon_gfx != 0)  {

					g_pic_copy.x1 = current_x1;
					g_pic_copy.y1 = current_y1;
					g_pic_copy.x2 = current_x1 + 13;
					g_pic_copy.y2 = current_y1 + 13;

					g_pic_copy.src = p_weapon_gfx;

					do_pic_copy(2);
				}
			}

		} while ((p_fighter = p_fighter->next));

		g_pic_copy_rect = rect_bak;
		g_figobj_unkn_x2_bak = g_figobj_unkn_x2;
		g_figobj_unkn_x2 = -1;
		g_figobj_unkn_x1_bak = g_figobj_unkn_x1;
		g_figobj_unkn_x1 = -1;
		g_figobj_unkn_y2_bak = g_figobj_unkn_y2;
		g_figobj_unkn_y2 = -1;
		g_figobj_unkn_y1_bak = g_figobj_unkn_y1;
		g_figobj_unkn_y1 = -1;

		for (i = 0; i < 8; i++) {
			if (g_fig_figlist_readd[i] != -1) {
				FIG_remove_from_list(g_fig_figlist_readd[i], 1);
				FIG_add_to_list(g_fig_figlist_readd[i]);
			}

			if (figlist_remove[i] != -1) {
				FIG_remove_from_list((signed char)figlist_remove[i], 0);
			}
		}

		flag = 0;

		for (i = 0; i < 8; i++) {
			if (g_fig_ani_state[i] != -1) {
				flag = 1;
				break;
			}
		}

		g_fig_star_last_count = -1;

		fight_printer();

		g_pic_copy.x1 = 0;
		g_pic_copy.y1 = 0;
		g_pic_copy.x2 = 319;
		g_pic_copy.y2 = 199;

		g_pic_copy.src = g_renderbuf_ptr;
		g_pic_copy.dst = g_vga_memstart;

		fight_delay();

		do_pic_copy(0);

		g_pic_copy.dst = g_renderbuf_ptr;
	}

	for (p_fighter = (struct struct_fighter*)g_fig_list_head; p_fighter; p_fighter = p_fighter->next) {
		if (p_fighter->visible != 0)
			p_fighter->visible = 1;
	}

	call_mouse();

	if (!val) {

		FIG_draw_pic();
		FIG_draw_figures();

		g_pic_copy.x1 = 0;
		g_pic_copy.y1 = 0;
		g_pic_copy.x2 = 319;
		g_pic_copy.y2 = 199;

		g_pic_copy.src = g_renderbuf_ptr;
		g_pic_copy.dst = g_vga_memstart;

		do_pic_copy(0);

		while (g_fig_continue_print == 1) {

/* We get in an endless loop here,
when the Timer IRQ cannot set g_fig_continue_print to 0.
So this call to wait_for_vsync() passes control
to the DOSBox-CPU and may run the timer.
 */
			/* REMARK: SPEEDFIX
			wait_for_vsync();
			*/
			if (fight_printer()) {
				g_pic_copy.x1 = 0;
				g_pic_copy.y1 = 0;
				g_pic_copy.x2 = 319;
				g_pic_copy.y2 = 199;

				g_pic_copy.src = g_renderbuf_ptr;
				g_pic_copy.dst = g_vga_memstart;

				do_pic_copy(0);
			}
		}

	} else {
		g_fig_continue_print = 0;
	}

	/* read TEMP/XX */
	/* TODO: should be O_BINARY | O_RDONLY */
	handle = _open(g_str_temp_xx_ptr, 0);
	_read(handle, g_buffer8_ptr, 64000);
	close(handle);

	g_pic_copy.dst = g_vga_backbuffer = g_vga_memstart;
}

static void set_delay_timer(void)
{
	/* set delay_timer to delay_factor */
	g_delay_timer = g_delay_factor;
}

static void fight_delay(void)
{
	CD_enable_repeat();

	while (g_delay_timer > 0) {
		g_delay_timer--;
		wait_for_vsync();
	}
}

#if !defined(__BORLANDC__)
}
#endif
