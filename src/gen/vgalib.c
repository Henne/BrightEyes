/*
 *	Rewrite of DSA1 Generator v1.05_de seg005 (Rasterlib)
 *	Functions rewritten: 2/2
 *
 *	We only rewrite those which differ from the game.
 *	These functions were written in assembler and are
 *      just a clean C-implementation.
*/
#if !defined(__BORLANDC__)
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vgalib.h"

#if !defined(__BORLANDC__)
extern unsigned char *g_vga_memstart;

static const int DEF_RATIO = 3;
static int RATIO = DEF_RATIO;
static int W_WIDTH = DEF_RATIO * O_WIDTH;
static int W_HEIGHT = DEF_RATIO * O_HEIGHT;

static Uint32 palette[256] = {0};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static Uint32 pixels[MAX_RATIO * MAX_RATIO * O_WIDTH * O_HEIGHT];
#endif

void set_video_mode(unsigned short mode)
{
#if !defined(__BORLANDC__)
	if (mode == 0x13) {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
			fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
			exit(-1);
		}

		window = SDL_CreateWindow(
			"BrightEyes",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			W_WIDTH,
			W_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		if (window == NULL) {
			fprintf(stderr, "Could not create Window: %s\n", SDL_GetError());
			SDL_Quit();
			exit(-1);
		}

		renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
		);

		texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			//SDL_TEXTUREACCESS_STATIC,
			W_WIDTH,
			W_HEIGHT
		);

	} else {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
#else
	asm {
		push ds
		push es
		push si
		push di
		push bp

		mov ah, 0x00
		mov al, byte ptr mode
		int 0x10

		pop bp
		pop di
		pop si
		pop es
		pop ds
	}
#endif
}

#if !defined(__BORLANDC__)
void update_sdl_window(void)
{
	if (RATIO == 1) {
		int pos = 0;
		for (int y = 0; y < O_HEIGHT; y++) {
			pos = y * O_WIDTH;
			for (int x = 0; x < O_WIDTH; x++) {
				pixels[pos] = palette[g_vga_memstart[pos]];
				pos++;
			}
		}
	} else {
		int o_pos = 0;
		for (int y_o = 0; y_o < O_HEIGHT; y_o++) {
			o_pos = y_o * O_WIDTH;
			for (int x_o = 0; x_o < O_WIDTH; x_o++) {

				/* fill the first line by hand */
				int w_pos = RATIO * (y_o * W_WIDTH + x_o);
				int col = palette[g_vga_memstart[o_pos]];
				for (int i = 0; i < RATIO; i++) {
					pixels[w_pos + i] = col;
				}
				o_pos++;
			}

			/* copy it RATIO - 1 times */
			for (int i = 1; i < RATIO; i++) {
				memcpy(pixels + RATIO * y_o * W_WIDTH + i * W_WIDTH,
					       pixels + RATIO * y_o * W_WIDTH,
					       W_WIDTH * sizeof(Uint32));
			}
		}
	}

	SDL_UpdateTexture(texture, NULL, pixels, W_WIDTH * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	//SDL_Delay(16);
}

int sdl_get_ratio(void)
{
	return RATIO;
}

void sdl_change_window_size(void)
{
	RATIO = (RATIO < MAX_RATIO) ? RATIO + 1 : 1;
	W_WIDTH = RATIO * O_WIDTH;
	W_HEIGHT = RATIO * O_HEIGHT;

	fprintf(stdout, "RATIO = %d W_WIDTH = %d W_HEIGHT = %d\n",
			RATIO, W_WIDTH, W_HEIGHT);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	memset(pixels, 0, W_WIDTH * W_HEIGHT * sizeof(Uint32));

	window = SDL_CreateWindow(
		"BrightEyes",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		W_WIDTH,
		W_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (window == NULL) {
		fprintf(stderr, "Could not create Window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		//SDL_TEXTUREACCESS_STATIC,
		W_WIDTH,
		W_HEIGHT
	);

	update_sdl_window();
}
#endif

void set_video_page(unsigned short page)
{
#if defined(__BORLANDC__)
	asm {
		push ds
		push es
		push si
		push di

		mov al, byte ptr page
		mov ah, 0x05
		int 0x10

		pop di
		pop si
		pop es
		pop ds
	}
#endif
}

void save_display_stat(signed short *pointer)
{
#if defined(__BORLANDC__)
	asm {
		push ds
		push es
		push si
		push di
		push bp

		les di, pointer
		mov ah, 0x0f
		int 0x10
		mov dx,ax
		xor ax,ax

		mov al, bh
		stosw
		mov al, dl
		stosw
		mov al, dh
		stosw

		push es
		mov ax, 0x1130
		mov bh, 0x02
		int 0x10
		xor dh, dh
		inc dx
		pop es
		mov es:[di], dx

		pop bp
		pop di
		pop si
		pop es
		pop ds
	}
#endif
}

#if !defined(__BORLANDC__)
static inline Uint32 get_ARGB(const unsigned char *p) {
	return (p[2] << 2) | (p[1] << 10) | (p[0] << 18);
}
#endif

void set_palette(const unsigned char *pointer, const unsigned char first_color, const unsigned short colors)
{
#if defined(__BORLANDC__)
		asm {
			push ds
			push es
			push si
			push di

			mov dx, 0x3c8
			mov al, byte ptr first_color
			out dx, al
			lds si, pointer
			mov dx, 0x3c9
			mov cx, word ptr colors
		}
set_palette_loop1:
		asm {
			lodsb
			out dx, al
			lodsb
			out dx, al
			lodsb
			out dx, al
			loop set_palette_loop1

			pop di
			pop si
			pop es
			pop ds
		}
#else
	signed int i;

	for (i = 0; i < colors; i++)
		palette[first_color + i] = get_ARGB(pointer + 3 * i);

	update_sdl_window();
#endif
}

void pic_copy(unsigned char *dst, const signed short d_x, const signed short d_y,
		const signed short s_x, const signed short s_y, const signed short width,
		const signed short height, unsigned char *src, const signed short mode)
{
	unsigned char *d = dst + 320 * d_y + d_x;
	unsigned char *s = src;
	int i, j;

	switch (mode) {
		/* this is not used in GEN */
		/* copy only values < 0xc8 */
		case 1: {

			for (i = height; i > 0; i--) {
				for (j = 0; j < width; j++) {
					if (d[j] < 0xc8)
						d[j] = s[j];
				}
				s += width;
				d += 320;
			}
			break;
		}
		/* copy only values != 0 */
		case 2: {

			for (i = height; i > 0; i--) {

				for (j = 0; j < width; j++) {
					if (s[j]) {
						d[j] = s[j];
					}
				}

				d += 320;
				s += width;
			}
			break;
		}
		/* copy each value using screen dimensions */
		case 3: {

			s += s_y * 320 + s_x;

			for (i = height; i > 0; i--) {
				memcpy(d, s, width);
				d += 320;
				s += 320;
			}
			break;
		}
		/* copy each value */
		default: {

			for (i = height; i > 0; i--) {
				memcpy(d, s, width);
				d += 320;
				s += width;
			}
			break;
		}
	}
}

void fill_rect(unsigned char *p_in, const signed short color, const signed short width, const signed short height)
{
	unsigned char *p = p_in;
	signed short h;

	for (h = height; h > 0; h--) {
		memset(p, color, width);
		p += 320;
	}
}

const unsigned short swap_u16(const unsigned short val)
{
	return ((val & 0xff) << 8) | ((val >> 8) % 0xff);
}

void copy_to_screen(unsigned char *src, unsigned char *dst, const signed short width, const signed short height, const signed short mode)
{
	signed short h;

	if (mode == 0) {

		for (h = height; h > 0; h--) {
			memcpy(dst, src, width);
			dst += 320;
			src += width;
		}

	} else if (mode == 2) {

		for (h = height; h > 0; h--) {
			memcpy(dst, src, width);
			dst += width;
			src += 320;
		}
	}
}

#if defined(__BORLANDC__)
#if 0
extern F_LXLSH;
unsigned char* normalize_ptr(unsigned char *pointer)
{
	asm {
		push ds
		push es
		push si
		push di

		xor dx,dx
		mov ax, word ptr pointer+2
		mov cx, 4
		call far ptr F_LXLSH@
		mov word ptr pointer+2, 0
		add ax, word ptr pointer
		adc dx, word ptr pointer+2
		mov dx, 0x0c
		call far ptr F_LXLSH@
		mov ax, word ptr pointer

		pop di
		pop si
		pop es
		pop ds
	}

}
#endif
#endif
