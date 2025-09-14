/*
 *	Rewrite of DSA1 v3.02_de functions of seg003 (movement)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg003.cpp
 *
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg008.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
#include "t_map.h"
#endif


#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if 0
static const char no_way_msg[][41] = {
	"IN DIESE RICHTUNG GEHT ES NICHT WEITER.",
	"YOU CAN GO NO FURTHER IN THIS DIRECTION."
};
#endif

signed short update_direction(unsigned char mod)
{
	/* save old direction */
	gs_direction_bak = gs_direction;
	/* set new direction */
	gs_direction = ((gs_direction + mod) & 0x3);

	/* set bogus variable to 1 */
	g_direction_unkn = 1;

	return -1;
}

void move(void)
{
	volatile signed short boundary_flag;
	unsigned char mapval;
	volatile signed short i;
	Bit8u *p_map_small;
	Bit8u *p_map_large;
	Bit8u *p_vis_field;

	signed short x;
	signed short y;


	p_map_small = p_map_large = g_dng_map;

	/* direction */
#if defined(__BORLANDC__)
	p_vis_field = (Bit8u*)MK_FP(_DS, ((gs_direction == 0) ? VISUAL_FIELD_DIR0 :
				((gs_direction == 1) ? VISUAL_FIELD_DIR1 :
				((gs_direction == 2) ? VISUAL_FIELD_DIR2 : VISUAL_FIELD_DIR3))));
#endif

	for (i = 0; i < 29; i++, p_vis_field += 2) {
		boundary_flag = 0;
		x = gs_x_target + host_readbs(p_vis_field);
		y = gs_y_target + host_readbs(p_vis_field + 1);

		if (x < 0) {
			x = 0;
			boundary_flag = 1;
		} else {
			if (g_dng_map_size - 1 < x) {
				x = g_dng_map_size - 1;
				boundary_flag = 1;
			}
		}

		if (y < 0) {
			y = 0;
			boundary_flag = 1;
		} else {
			if (y > 15) {
				y = 15;
				boundary_flag = 1;
			}
		}

		mapval = (g_dng_map_size == 0x10) ?
			/* dungeon */
			host_readb(p_map_small + (y << 4) + x) :
			/* city */
			host_readb(p_map_large + (y << 5) + x);

		if (boundary_flag != 0) {
			g_visual_field_vals[i] = ((mapval == 0xa0) || (mapval == 0xb0) ? mapval : 0xb0);
		} else {
			g_visual_field_vals[i] = mapval;
		}
	}

	if (g_dng_map_size == 0x10) {
		/* dungeon mode */
		g_steptarget_front = host_readb(p_map_small + ((gs_y_target + host_readbs(p_vis_field + 1)) << 4) + gs_x_target + host_readbs(p_vis_field));

		g_steptarget_back = host_readb(p_map_small + ((gs_y_target + host_readbs(p_vis_field + 3)) << 4) + gs_x_target + host_readbs(p_vis_field + 2));
	} else {
		/* city mode */
		g_steptarget_front = host_readb(p_map_large + ((gs_y_target + host_readbs(p_vis_field + 1)) << 5) + gs_x_target + host_readbs(p_vis_field));

		g_steptarget_back = host_readb(p_map_large + ((gs_y_target + host_readbs(p_vis_field + 3)) << 5) + gs_x_target + host_readbs(p_vis_field + 2));
	}
}

void door_frame(signed short no, signed short x, signed short y, signed short frame)
{
	signed short width;
	signed short height;
	signed short l1;
	Bit8u *p1;
	Bit8u *p2;
	struct nvf_desc nvf;

	nvf.dst = (Bit8u*)((HugePt)g_buffer9_ptr3 + 0x2e248L);
	nvf.src = (Bit8u*)g_buffer9_ptr3;
	nvf.no = no;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	height -= frame;
	l1 = width;

	p1 = (Bit8u*)((((HugePt)g_buffer9_ptr3) + frame * width) + 0x2e248L);

	if ((x < 0) && ((x + width) > 0)) {
		width += x;
		p1 += __abs__(x);
		x = 0;
	}

	if ((y < 0) && ((y + height) > 0)) {
		height -= y;
		p1 += __abs__(y) * l1;
		y = 0;
	}

	if ( (x < 208) && (y < 135) && (x >= 0) && (y >= 0)) {

		if (x + width > 208) {
			width = 208 - x;
		}

		if (y + height > 135) {
			height = 135 - y;
		}

		p2 = g_renderbuf_ptr + 208 * y + x;

		copy_solid(p2, p1, width, height, 208, l1, 0);
	}
}

/**
 * \brief   looting of a corpse
 *
 * \param   chest_ptr   a pointer to the chest description
 * \param   text        output text
 * \param   flag        to mark this chest as done
 */
void loot_corpse(struct struct_chest* chest_ptr, char *text, Bit8s *flag)
{
	signed short answer;

	sprintf(g_text_output_buf, (char*)(!(*flag) ? get_ttx(523) : get_ttx(527)), text);

	if (!(*flag)) {

		g_textbox_width = 7;

		answer = GUI_radio(g_text_output_buf, 3, get_ttx(524), get_ttx(525), get_ttx(526)) - 1;

		g_textbox_width = 3;

		if (answer == 0) {

			/* examine the corpse */
			chest_ptr->loot((Bit8u*)chest_ptr);

			if (!(*flag))	{

				/* mark this corpse as done */
				*flag = 1;
				/* Boron - 20 */
				gs_gods_estimation[GOD_BORON] -= 20L;

			}

		} else if (answer == 1) {

			/* Borons Blessing */
			if (!(*flag)) {
				/* mark this corpse as done */
				*flag = 1;
				/* Boron + 20 */
				gs_gods_estimation[GOD_BORON] += 20L;
			}
		}
	} else {
		GUI_output(g_text_output_buf);
	}
}

void no_way(void)
{
	GUI_output(g_noway_string);
}

#if !defined(__BORLANDC__)
}
#endif
