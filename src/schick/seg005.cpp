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
#include <unistd.h>
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

/* static prototypes */
static void set_delay_timer(void);
static void fight_delay(void);


/**
 * \param     fighter pointer to a FIGHTER object
 * \param[in] x       x coordinate on the screen
 * \param[in] y       y coordinate on the screen
 */
static signed short FIG_obj_needs_refresh(struct struct_fighter *fighter, const signed short x, const signed short y)
{
	struct struct_fighter *list_i;
	signed short ox;
	signed short oy;

	if (fighter->visible) {

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
void FIG_set_star_color(Bit8u *ptr, unsigned short count, unsigned char color)
{
	Bit8u *p;

	color += 0x80;

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
static char* FIG_name_3rd_case(unsigned short type, volatile unsigned short pos)
{
	if (type == 2) {
		return (char*)get_hero(pos) + HERO_NAME2;
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
static char* FIG_name_4th_case(unsigned short type, volatile unsigned short pos)
{

	if (type == 2)
		return (char*)get_hero(pos) + HERO_NAME2;
	else
		return (char*)GUI_names_grammar(2, pos, 1);
}

/**
 * \brief   returns name in the 1st german case
 *
 * \param   type        1 = enemy / 2 = hero
 * \param   pos         position
 * \return              "ein Magier" if the enemy is a "Magier".
 */
static char *FIG_name_1st_case(unsigned short type, volatile unsigned short pos)
{

	if (type == 2)
		return (char*)get_hero(pos) + HERO_NAME2;
	else
		return (char*)GUI_names_grammar(0, pos, 1);
}

#define idx (g_fig_msg_dtps[ds_readws(FIG_MSG_DATA + 4 * g_fig_star_counter) - 1])

unsigned short fight_printer(void)
{
	signed short fg_bak;
	signed short bg_bak;
	Bit8u* gfx_pos_bak;
	Bit16u x;
	char str[6];
	Bit8u* gfx_dst_bak;

	Bit16s f_action;

	if (ds_readw(FIG_MSG_DATA) == 0)
		g_fig_continue_print = 0;

	if (!g_fig_star_timer && g_fig_star_printed) {

		g_fig_star_counter++;

		g_fig_star_printed = 0;

		g_fig_star_timer = g_autofight ? 10 : g_delay_factor * 6;

		if (!ds_readw(FIG_MSG_DATA + g_fig_star_counter * 4))
			g_fig_continue_print = 0;
	}

	if (g_fig_continue_print) {

		if (g_fig_star_counter != g_fig_star_last_count) {

			g_fig_star_printed = 1;

			f_action = ds_readw(FIG_MSG_DATA + g_fig_star_counter * 4);

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
			if (ds_readw((FIG_MSG_DATA + 2) + g_fig_star_counter * 4)) {

				set_textcolor(0xff, g_fig_star_colors[f_action - 1] + 0x80);

				my_itoa(ds_readws((FIG_MSG_DATA + 2) + g_fig_star_counter * 4), str, 10);

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
//					case 1:	/* heroes attack fails */
//					case 3: /* enemy attack fails */

					sprintf(g_text_output_buf, get_tx(idx),
						FIG_name_3rd_case(g_fig_actor_grammar.type, g_fig_actor_grammar.id));

				} else if (f_action == 2 || f_action == 4 || f_action == 7) {
//					case 2: /* hero parade fails */
//					case 4: /* enemy parade fails */
//					case 7:	/* hero get unconscious */

					sprintf(g_text_output_buf, get_tx(idx),
						FIG_name_3rd_case(g_fig_target_grammar.type, g_fig_target_grammar.id));



				} else if (f_action == 8 || f_action == 11) {
//					case 8:		/* enemy hits hero */
//					case 11:	/* hero hits enemy */

					sprintf(g_text_output_buf, get_tx(idx),
						FIG_name_1st_case(g_fig_actor_grammar.type, g_fig_actor_grammar.id),
						FIG_name_4th_case(g_fig_actor_grammar.type, g_fig_actor_grammar.id));
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

void draw_fight_screen(Bit16u val)
{
	signed short i;
	signed short obj_id;
	signed short width;
	signed short flag;
	signed short current_x1;
	signed short current_y1;
	signed short obj_x;
	signed short obj_y;

	Bit8u* p_figure_gfx;
	Bit8u* p_weapon_gfx;
	struct struct_fighter *list_ii;

	struct struct_rect rect_bak;
	Bit8u *hero;
	struct enemy_sheet *p_enemy_sheet;

	signed short viewdir_before;
	signed short viewdir_after;
	signed short target_id;
	signed char twofielded_move_tail_first;
	struct struct_fighter *p_fighter_tmp;
	signed short viewdir_unconsc;
	Bit8u *sheet;
	Bit8u *p_weapon_anisheet;
	signed short handle;
	struct nvf_desc nvf;
	signed short figlist_remove[8];

	update_mouse_cursor();

	list_ii = (struct struct_fighter*)g_fig_list_head;

	do {
		/* Check for each list entry if a sprite is needed */

		if (list_ii->reload == -1) {

			nvf.src = (Bit8u*)load_fight_figs(list_ii->figure);
			nvf.dst = list_ii->gfxbuf;
			nvf.no = list_ii->nvf_no;
			nvf.type = 0;
			nvf.width = (Bit8u*)&width;
			nvf.height = (Bit8u*)&obj_id;

			process_nvf(&nvf);

			list_ii->reload = 0;
		}

	} while (list_ii = list_ii->next);

	/* set elements array[0] of array[9] */
	g_fig_gfxbuffers[0] = g_fightobj_buf_seek_ptr;
	g_fig_ani_state[0] = -1;

	for (i = 1; i < 8; i++) {
		/* copy a pointer to the next position */
		g_fig_gfxbuffers[i] = (HugePt)g_fig_gfxbuffers[i - 1] + 0x508L;
		g_fig_ani_state[i] = -1;
	}

	list_ii = (struct struct_fighter*)g_fig_list_head;
	flag = 0;

	do {
#if !defined(__BORLANDC__)
		D1_LOG(" loop Figure = %3d Sheet_ID : %d 0xf : %d 0x12: %d object: %d\n",
				list_ii->figure,
				list_ii->sheet, list_ii->wsheet,
				list_ii->visible, list_ii->obj_id);
#endif

		if (list_ii->sheet != -1) {
			/* Has a sheet id */

			if (list_ii->visible) {
				list_ii->visible = (3);
			}

			flag = 1;

			g_fig_ani_state[list_ii->sheet] = 0;

			memcpy((Bit8u*)g_fig_gfxbuffers[list_ii->sheet],
				list_ii->gfxbuf,
				list_ii->width * list_ii->height);
		}

		if (list_ii->wsheet != -1) {
			memset((Bit8u*)g_fig_gfxbuffers[list_ii->wsheet], 0, 0x508);
		}


	} while (list_ii = list_ii->next);

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

		for (list_ii = (struct struct_fighter*)g_fig_list_head; list_ii; list_ii = list_ii->next) {
			if (list_ii->visible == 2)
				list_ii->visible = 1;
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

		list_ii = (struct struct_fighter*)g_fig_list_head;

		do {
			p_weapon_gfx = 0;	/* NULL */

			obj_x = 10 - (list_ii->width / 2) +
				(10 * (list_ii->cbx + list_ii->cby));

			obj_y = 118 - list_ii->height + (list_ii->cbx - list_ii->cby) * 5;

			obj_x += list_ii->offsetx;
			obj_y += list_ii->offsety;

			p_figure_gfx = list_ii->gfxbuf;

			if ((list_ii->sheet != -1) &&
				(g_fig_ani_state[list_ii->sheet] != -1)) {

				sheet = p_datseg + FIG_ANISHEETS + list_ii->sheet * 0xf3;

				if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -1) {

					p_figure_gfx = (Bit8u*)g_fig_gfxbuffers[list_ii->sheet];
					g_fig_ani_state[list_ii->sheet] = -1;
					list_ii->sheet = (list_ii->wsheet = (-1));

				} else {

					if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -7)
					{
						list_ii->z = (host_readbs(sheet + 2 + g_fig_ani_state[list_ii->sheet] * 3));

						g_fig_ani_state[list_ii->sheet]++;

						g_fig_figlist_readd[list_ii->sheet] = list_ii->id;
					}

					if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -9)
					{
						play_voc(0xc8 + host_readbs(sheet + 2 + 3 * g_fig_ani_state[list_ii->sheet]));
						g_fig_ani_state[list_ii->sheet]++;
					}


					if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -5) {

						if (list_ii->wsheet != -1) {

							p_weapon_anisheet = p_datseg + FIG_ANISHEETS + list_ii->wsheet * 0xf3;

							if (host_readbs(p_weapon_anisheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -9)
							{
								play_voc(0xc8 + host_readbs(p_weapon_anisheet + 2 + 3 * g_fig_ani_state[list_ii->sheet]));
							}
						}

						g_fig_ani_state[list_ii->sheet]++;

					} else if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -4) {

						host_writeb(sheet, host_readb(sheet + 2 + 3 * g_fig_ani_state[list_ii->sheet]));

						g_fig_ani_state[list_ii->sheet]++;

					} else if ((host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -3) ||
							(host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -6)) {

						/* get nvf no */
						viewdir_before = list_ii->nvf_no;

						if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -3) {

							list_ii->nvf_no += host_readbs(sheet + 3 + 3 * g_fig_ani_state[list_ii->sheet]);
						} else {

							list_ii->figure = (ds_readbs(GFXTAB_FIGURES_MAIN + list_ii->sprite_no * 5 + host_readbs(sheet + 2 + 3 * g_fig_ani_state[list_ii->sheet])));
							list_ii->nvf_no = host_readbs(sheet + 3 + 3 * g_fig_ani_state[list_ii->sheet]);
						}

						if (list_ii->figure >= 88) {
							/* fighter uses figure from MONSTER file */

							if (list_ii->nvf_no > 3) {

								/* not standing still */
								list_ii->offsetx = (ds_readbs((GFXTAB_OFFSETS_MAIN + 8) + list_ii->sprite_no * 10));
								list_ii->offsety = (ds_readbs((GFXTAB_OFFSETS_MAIN + 9) + list_ii->sprite_no * 10));

								if (list_ii->twofielded != -1) {
									list_ii->x1 = ds_readb((GFXTAB_TWOFIELDED_X1 + 1));
									list_ii->x2 = ds_readb((GFXTAB_TWOFIELDED_X2 + 1));
								}

							} else {
								list_ii->offsetx = ds_readbs(GFXTAB_OFFSETS_MAIN + list_ii->sprite_no * 10 + list_ii->nvf_no * 2);
								list_ii->offsety = ds_readbs((GFXTAB_OFFSETS_MAIN + 1) + list_ii->sprite_no * 10 + list_ii->nvf_no * 2);

								if (list_ii->twofielded != -1) {
									list_ii->x1 = ds_readb(GFXTAB_TWOFIELDED_X1 + list_ii->nvf_no);
									list_ii->x2 = ds_readb(GFXTAB_TWOFIELDED_X2 + list_ii->nvf_no);
								}
							}

						} else {
							if (list_ii->nvf_no == g_nvftab_figures_dead[list_ii->sprite_no]) {

								list_ii->offsetx = ds_readbs((GFXTAB_OFFSETS_MAIN + 8) + list_ii->sprite_no * 10);
								list_ii->offsety = ds_readbs((GFXTAB_OFFSETS_MAIN + 9) + list_ii->sprite_no * 10);

							} else {
								viewdir_unconsc = list_ii->nvf_no - g_nvftab_figures_unconscious[list_ii->sprite_no];

								if (viewdir_unconsc >= 0) {
									list_ii->offsetx = ds_readbs(GFXTAB_OFFSETS_UNCONSCIOUS + list_ii->sprite_no * 8 + viewdir_unconsc * 2);
									list_ii->offsety = ds_readbs((GFXTAB_OFFSETS_UNCONSCIOUS + 1) + list_ii->sprite_no * 8 + viewdir_unconsc * 2);
								}
							}
						}

						obj_x = 10 - (list_ii->width / 2) + (10 * (list_ii->cbx + list_ii->cby));

						obj_y = 118 - list_ii->height + ((list_ii->cbx - list_ii->cby) * 5);

						obj_x += list_ii->offsetx;
						obj_y += list_ii->offsety;

						if ((list_ii->sheet < 6) && (host_readbs(sheet + 0xf2) >= 0)) {
							nvf.src = (Bit8u*)load_fight_figs(list_ii->figure);
						} else {
							nvf.src = g_spellobj_nvf_buf;
						}

						nvf.dst = list_ii->gfxbuf;
						nvf.no = list_ii->nvf_no;
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&obj_id;

						process_nvf(&nvf);

						g_fig_ani_state[list_ii->sheet]++;

						p_figure_gfx = list_ii->gfxbuf;

						if (list_ii->twofielded > 20) {
							/* list_i is the fighter entry of the tail of a twofielded enemy */

							viewdir_after = (list_ii->nvf_no > 3) ? 1 : list_ii->nvf_no;

							list_ii->offsetx += ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OX + viewdir_after);
							list_ii->offsety += ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OY + viewdir_after);
							list_ii->x1 = ds_readbs(GFXTAB_TWOFIELDED_EXTRA_X1 + viewdir_after);
							list_ii->x2 = ds_readbs(GFXTAB_TWOFIELDED_EXTRA_X2 + viewdir_after);

							obj_id = get_cb_val(list_ii->cbx, list_ii->cby); /* enemy_id + 30 of the enemy the tail belongs to */

							FIG_set_cb_field(list_ii->cby, list_ii->cbx, list_ii->obj_id);

							/* update CBX depending on the view direction */
							if ( ((viewdir_after == 2) && ((viewdir_before == 1) || (viewdir_before == 3))) ||
								(((viewdir_after == 3) || (viewdir_after == 1)) && (viewdir_before == 0)))
							{
								list_ii->cbx++;

							} else if ( ((viewdir_after == 0) && ((viewdir_before == 1) || (viewdir_before == 3))) ||
									(((viewdir_after == 3) || (viewdir_after == 1)) && (viewdir_before == 2)))
							{
								list_ii->cbx--;
							}

							/* update CBY depending on the view direction */
							if ( ((viewdir_after == 3) && ((viewdir_before == 0) || (viewdir_before == 2))) ||
								(((viewdir_after == 0) || (viewdir_after == 2)) && (viewdir_before == 1)))
							{
								list_ii->cby--;

							} else	if ( ((viewdir_after == 1) && ((viewdir_before == 0) || (viewdir_before == 2))) ||
									(((viewdir_after == 0) || (viewdir_after == 2)) && (viewdir_before == 3)))
							{
								list_ii->cby++;

							} else if ((viewdir_after == 1) && (viewdir_before == 3))
							{
								list_ii->cby = (list_ii->cby + 2);
							}

							target_id = get_cb_val(list_ii->cbx, list_ii->cby); /* object id of the square the tail moves to */
							list_ii->obj_id = (signed char)target_id; /* move it to FIGHTER_OBJ_ID */
							FIG_set_cb_field(list_ii->cby, list_ii->cbx, obj_id); /* set object id of the target square to enemy_id + 30 */

							obj_x = 10 - (list_ii->width / 2) + (10 * (list_ii->cbx + list_ii->cby));

							obj_y = 118 - list_ii->height + ((list_ii->cbx - list_ii->cby) * 5);

							obj_x += list_ii->offsetx;

							obj_y += list_ii->offsety;

						}
					} else {

						/* move a hero/enemy */
						if (host_readbs(sheet + 1 + 3 * g_fig_ani_state[list_ii->sheet]) == -2) {

							if (list_ii->sheet < 6) {

								obj_id = get_cb_val(list_ii->cbx, list_ii->cby);

								/* copy FIGHTER_OBJ_ID back to the chessboard */
								FIG_set_cb_field(list_ii->cby, list_ii->cbx, list_ii->obj_id);

								list_ii->cbx = (list_ii->cbx + host_readbs(sheet + 2 + 3 * g_fig_ani_state[list_ii->sheet]));

								list_ii->cby = (list_ii->cby + host_readbs(sheet + 3 + 3 * g_fig_ani_state[list_ii->sheet]));

								twofielded_move_tail_first = 0;

								/* get the value from the cb where the actor wants to move to */
								target_id = get_cb_val(list_ii->cbx, list_ii->cby);

								if ((list_ii->twofielded > 20) && (obj_id - 20 == target_id)) {
									/* for a two-fielded enemy, either the head part or the tail part is moved first.
									 * This is the case that the tail part is moved first (the target square is the head part). */

#ifndef M302de_ORIGINAL_BUGFIX
									/* Original-Bug 5: */
									/* the removal of the following line is not strictly necessary, but it is not needed as a replacement is added further below. */
									list_ii->obj_id = 0;
#endif
									twofielded_move_tail_first = 1;

									/* create pointer to the head part of the enemy */
									p_fighter_tmp = FIG_get_fighter(g_enemy_sheets[target_id - 10].mon_id);

#ifdef M302de_ORIGINAL_BUGFIX
									/* Original-Bug 5: */
									/* The FIGHTER_OBJ_ID entry of the head part will be overwritten by the next line in the original code.
									 * In this way, sometimes dead bodies are lost from the chessboard after a two-fielded enemy walks over it.
									 * The right thing is to copy it to the FIGHTER_OBJ_ID of tail part. */
									list_ii->obj_id = ((signed char)(p_fighter_tmp->obj_id));
#endif
									p_fighter_tmp->obj_id = (signed char)obj_id;
									/* write cb_id of the tail part at FIGHTER_OBJ_ID of the head part.
									 * when the head part moves lated, it will be written to the cb.
									 * possible bug: the overwritten FIGHTER_OBJ_ID is lost! */
								} else {
									list_ii->obj_id = ((signed char)target_id);
								}

								/* check chessboard bounds */
								if ( (list_ii->cbx >= 24) || (list_ii->cby >= 24)
									|| (list_ii->cbx < 0) || (list_ii->cby < 0)
									|| (list_ii->obj_id < 0))
								{
									/* hero/enemy escapes */

										if (list_ii->is_enemy == 1) {
											/* enemy escapes */
											p_enemy_sheet = FIG_get_enemy_sheet(list_ii->id);
											if (p_enemy_sheet) {
												p_enemy_sheet->flags.dead = 1;
												p_enemy_sheet->bp = 0;
												figlist_remove[list_ii->sheet] = p_enemy_sheet->fighter_id;

												if (list_ii->twofielded != -1) {
#ifdef M302de_ORIGINAL_BUGFIX
													/* Original-Bug 4:
													 * remove tail of the escaped two-fielded enemy from the chessboard
													 * For more on this bug, see Original-Bug 3 at seg032.cpp */
													p_fighter_tmp = FIG_get_fighter(g_fig_twofielded_table[list_ii->twofielded]);
													FIG_set_cb_field(p_fighter_tmp->cby, p_fighter_tmp->cbx, p_fighter_tmp->obj_id);
#endif
													figlist_remove[2 + list_ii->sheet] = g_fig_twofielded_table[list_ii->twofielded];
												}
											}
										} else {
											/* hero escapes */
											hero = (Bit8u*)FIG_get_hero_ptr(list_ii->id);
											if (hero) {
												host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
												or_ptr_bs(hero + HERO_FLAGS2, 1); /* set 'scared' flag */

												/* set the dungeon position the hero escapes to.
												 * This depends on the direction the escape square on the battlefield has been entered.
												 * Note: Apparently, this is done in any fight, including seafights and wilderness fights where it doesn't make sense.
												 * The distinction is done only later. */
												host_writew(hero + HERO_ESCAPE_POSITION,
													g_fig_flee_position[(host_readbs(hero + HERO_VIEWDIR) == 3) ? NORTH : (host_readbs(hero + HERO_VIEWDIR) + 1)]);
												figlist_remove[list_ii->sheet] = host_readbs(hero + HERO_FIGHTER_ID);

											}
										}

										list_ii->obj_id = 0;
										host_writebs(sheet + 1 + 3 * (1 + g_fig_ani_state[list_ii->sheet]), -1);

										if (list_ii->twofielded != -1) {
											ds_writeb((FIG_ANISHEETS + 4 + 2*0xf3) +
													(list_ii->sheet * 0xf3 +
													3 * g_fig_ani_state[2 + list_ii->sheet]), -1);

											g_fig_ani_state[2 + list_ii->sheet] = -1;
										}

								} else {
									if (!twofielded_move_tail_first) {
										FIG_set_cb_field(list_ii->cby, list_ii->cbx, obj_id);
									}
								}
							} else {
								list_ii->cbx = (list_ii->cbx +
									host_readbs(sheet + g_fig_ani_state[list_ii->sheet] * 3 + 2));

								list_ii->cby = (list_ii->cby +
									host_readbs(sheet + g_fig_ani_state[list_ii->sheet] * 3 + 3));
							}

							g_fig_figlist_readd[list_ii->sheet] = list_ii->id;

							g_fig_ani_state[list_ii->sheet]++;
						}

						if (host_readbs(sheet + 1 + (g_fig_ani_state[list_ii->sheet] * 3)) == -1) {

							p_figure_gfx = (Bit8u*)g_fig_gfxbuffers[list_ii->sheet];
							g_fig_ani_state[list_ii->sheet] = -1;
							list_ii->sheet = list_ii->wsheet = -1;

						} else {
							obj_x = 10 - (list_ii->width / 2) +
								(10 * (list_ii->cbx + list_ii->cby));

							obj_y = 118 - list_ii->height + ((list_ii->cbx - list_ii->cby) * 5);

							obj_x += list_ii->offsetx;

							obj_y += list_ii->offsety;

							obj_x += host_readbs(sheet + 2 + g_fig_ani_state[list_ii->sheet] * 3);

							obj_y -= host_readbs(sheet + 3 + g_fig_ani_state[list_ii->sheet] * 3);

							i = ds_readbs(GFXTAB_FIGURES_MAIN + (list_ii->sprite_no * 5) + host_readbs(sheet));

							if ((list_ii->sheet < 6) && (host_readbs(sheet + 0xf2) >= 0)) {
								nvf.src = (Bit8u*)load_fight_figs(i);
							} else {
								nvf.src = g_spellobj_nvf_buf;
							}

							nvf.dst = (Bit8u*)g_fig_gfxbuffers[list_ii->sheet];
							nvf.no = host_readbs(sheet + 1 + g_fig_ani_state[list_ii->sheet] * 3);
							nvf.type = 0;
							nvf.width = (Bit8u*)&width;
							nvf.height = (Bit8u*)&obj_id;

							process_nvf(&nvf);

							if (list_ii->wsheet != -1) {

								p_weapon_anisheet = p_datseg + FIG_ANISHEETS + list_ii->wsheet * 0xf3;

								if (host_readbs(p_weapon_anisheet + 1 + g_fig_ani_state[list_ii->sheet] * 3) == -1)
								{
									list_ii->wsheet = -1;
								} else {
									current_x1 = obj_x;
									current_y1 = obj_y;

									p_weapon_gfx = (Bit8u*)g_fig_gfxbuffers[list_ii->wsheet];

									if (host_readbs(p_weapon_anisheet + 1 + 3 * (g_fig_ani_state[list_ii->sheet])) != -5) {
										nvf.dst = (Bit8u*)g_fig_gfxbuffers[list_ii->wsheet];
										nvf.src = g_weapons_nvf_buf;
										nvf.no = host_readb(p_weapon_anisheet + 1 + g_fig_ani_state[list_ii->sheet] * 3);
										nvf.type = 0;
										nvf.width = (Bit8u*)&width;
										nvf.height = (Bit8u*)&obj_id;

										process_nvf(&nvf);

										current_x1 += list_ii->width - 14;
										current_x1 += host_readbs(p_weapon_anisheet + 2 + g_fig_ani_state[list_ii->sheet] * 3);
										current_y1 -= host_readbs(p_weapon_anisheet + 3 + g_fig_ani_state[list_ii->sheet] * 3);
									}
								}
							}

							g_fig_ani_state[list_ii->sheet]++;
						}

						if (list_ii->sheet != -1) {
							p_figure_gfx = (Bit8u*)g_fig_gfxbuffers[list_ii->sheet];
						}
					}
				}
			}
/* 0x17e5 */

			if (FIG_obj_needs_refresh(list_ii, obj_x, obj_y)) {

				if ((list_ii->sheet == -1) || (figlist_remove[list_ii->sheet] == -1)) {

					if (list_ii->sheet != -1) {

						g_figobj_unkn_x1 = g_figobj_unkn_x2;
						g_figobj_unkn_y1 = g_figobj_unkn_y2;

						g_figobj_unkn_x2 = obj_x - 8;
						g_figobj_unkn_y2 = obj_y - 8;
					}

					/* set Y1 */
					g_pic_copy_rect.y1 = obj_y + list_ii->y1;
					if (g_pic_copy_rect.y1 < 0) g_pic_copy_rect.y1 = 0;

					/* set X1 */
					g_pic_copy_rect.x1 = obj_x + list_ii->x1;
					if (g_pic_copy_rect.x1 < 0) g_pic_copy_rect.x1 = 0;

					/* set Y2 */
					g_pic_copy_rect.y2 = obj_y + list_ii->y2;
					if (g_pic_copy_rect.y2 > (200 - 1)) g_pic_copy_rect.y2 = (200 - 1);

					/* set X2 */
					g_pic_copy_rect.x2 = obj_x + list_ii->x2;
					if (g_pic_copy_rect.x2 > (320 - 2)) g_pic_copy_rect.x2 = (320 - 2);

					g_pic_copy.x1 = obj_x;
					g_pic_copy.y1 = obj_y;
					g_pic_copy.x2 = obj_x + list_ii->width - 1;
					g_pic_copy.y2 = obj_y + list_ii->height - 1;
					g_pic_copy.src = p_figure_gfx;

					do_pic_copy(2);	/* Critical */
				}


				/* NULL check on Bit8u* */
				if (p_weapon_gfx != 0)  {

					g_pic_copy.x1 = current_x1;
					g_pic_copy.y1 = current_y1;
					g_pic_copy.x2 = current_x1 + 13;
					g_pic_copy.y2 = current_y1 + 13;

					g_pic_copy.src = p_weapon_gfx;

					do_pic_copy(2);
				}
			}

		} while (list_ii = list_ii->next);

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

	for (list_ii = (struct struct_fighter*)g_fig_list_head; list_ii; list_ii = list_ii->next) {
		if (list_ii->visible != 0)
			list_ii->visible = 1;
	}

	refresh_screen_size();

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
#ifdef M302de_SPEEDFIX
			wait_for_vsync();
#endif
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
	handle = open(g_str_temp_xx_ptr, 0);
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
	seg001_02c4();

	while (g_delay_timer > 0) {
		g_delay_timer--;
		wait_for_vsync();
	}

}

#if !defined(__BORLANDC__)
}
#endif
