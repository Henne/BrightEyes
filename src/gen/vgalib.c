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
static SDL_mutex *PixelsMutex = NULL;

static Uint32 *pixels = NULL;
static Uint8 *vga_bak = NULL;
static int pal_updated = 0;
static int win_resized = 0;

/* instrumentation to count calls for sdl_update_rect_window */
static int calls = 0, updates = 0;

static const int SHOW_DRIVERS = 0; // set to 1 for driver info

static void sdl_renderer_info(void)
{
	SDL_RendererInfo info;
	const int no_drivers = SDL_GetNumRenderDrivers();
	fprintf(stdout, "SDL_GetNumRenderDrivers() = %d\n", no_drivers);
	for (int i = 0; i < no_drivers; i++) {
		if (SDL_GetRenderDriverInfo(i, &info) == 0) {

			fprintf(stderr, "driver   = %d\n", i);
			fprintf(stderr, "name     = %s\n", info.name);
			fprintf(stderr, "flags    = 0x%02x\n", info.flags);
			fprintf(stderr, "#txt     = %02d\n", info.num_texture_formats);
			fprintf(stderr, "textures = ");
			for (int j = 0; j < info.num_texture_formats; j++) {
				switch (info.texture_formats[j]) {
				case SDL_PIXELFORMAT_INDEX1LSB: fprintf(stderr, "INDEX1LSB "); break;
				case SDL_PIXELFORMAT_INDEX1MSB: fprintf(stderr, "INDEX1MSB "); break;
				case SDL_PIXELFORMAT_INDEX4LSB: fprintf(stderr, "INDEX4LSB "); break;
				case SDL_PIXELFORMAT_INDEX4MSB: fprintf(stderr, "INDEX4MSB "); break;
				case SDL_PIXELFORMAT_INDEX8: fprintf(stderr, "INDEX8 "); break;
				case SDL_PIXELFORMAT_RGB332: fprintf(stderr, "RGB332 "); break;
				case SDL_PIXELFORMAT_RGB24: fprintf(stderr, "RGB24 "); break;
				case SDL_PIXELFORMAT_BGR24: fprintf(stderr, "BGR24 "); break;
				case SDL_PIXELFORMAT_RGB888: fprintf(stderr, "RGB888 "); break;
				case SDL_PIXELFORMAT_BGR888: fprintf(stderr, "BGR888 "); break;
				case SDL_PIXELFORMAT_ARGB4444: fprintf(stderr, "ARGB4444 "); break;
				case SDL_PIXELFORMAT_RGBA4444: fprintf(stderr, "RGBA4444 "); break;
				case SDL_PIXELFORMAT_ABGR4444: fprintf(stderr, "AGBR4444 "); break;
				case SDL_PIXELFORMAT_ARGB8888: fprintf(stderr, "ARGB8888 "); break;
				case SDL_PIXELFORMAT_RGBA8888: fprintf(stderr, "RGBA8888 "); break;
				case SDL_PIXELFORMAT_ABGR8888: fprintf(stderr, "AGBR8888 "); break;
				default: fprintf(stderr, "0x%x ", info.texture_formats[j]);
				}
			}
			fprintf(stderr, "\n");
			fprintf(stderr, "max res  = %dx%d\n\n", info.max_texture_width, info.max_texture_height);
		}
	}
}

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

		if (SHOW_DRIVERS)
			sdl_renderer_info();

		renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
		);

		texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ABGR8888,
			SDL_TEXTUREACCESS_STREAMING,
			//SDL_TEXTUREACCESS_STATIC,
			W_WIDTH,
			W_HEIGHT
		);

		PixelsMutex = SDL_CreateMutex();
		if (PixelsMutex == NULL) {
			fprintf(stderr, "Could not create PixelsMutex: %s\n", SDL_GetError());
			SDL_Quit();
			exit(-1);
		}

		vga_bak = calloc(O_WIDTH * O_HEIGHT * sizeof(Uint8), 1);
		if (vga_bak == NULL) {
			fprintf(stderr, "ERROR: cannot allocate vga_bak\n");
			exit(-1);
		}
		pixels = calloc(W_WIDTH * W_HEIGHT * sizeof(Uint32), 1);
		if (pixels == NULL) {
			fprintf(stderr, "ERROR: cannot allocate pixels\n");
			exit(-1);
		}

	} else {
		free(pixels);
		free(vga_bak);
		SDL_DestroyMutex(PixelsMutex);
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		fprintf(stderr, "sdl_update_rect_window() calls = %d updates = %d\n", calls, updates);
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
static void sdl_update_rect_pixels(const int x_in, const int y_in, const int width_in, const int height_in)
{
	int width = width_in;
	int height = height_in;

	if ((x_in + width) > O_WIDTH) width = O_WIDTH - x_in;
	if ((y_in + height) > O_HEIGHT) height = O_HEIGHT - y_in;

	if (RATIO == 1) {
		for (int y_o = y_in; y_o < y_in + height; y_o++) {
			int o_pos = O_WIDTH * y_o + x_in;
			for (int x_o = x_in; x_o < x_in + width; x_o++) {
				pixels[o_pos] = palette[g_vga_memstart[o_pos]];
				o_pos++;
			}
		}
	} else {
		for (int y_o = y_in; y_o < y_in + height; y_o++) {
			Uint32 *wp = pixels + RATIO * (RATIO * O_WIDTH * y_o + x_in);
			int o_pos = O_WIDTH * y_o + x_in;
			for (int x_o = x_in; x_o < x_in + width; x_o++) {
				/* fill the first line by hand */
				//int w_pos = RATIO * (RATIO * O_WIDTH * y_o + x_o);
				Uint32 col = palette[g_vga_memstart[o_pos]];
				int i = RATIO;
				while (i) {
					*wp++ = col;
					i--;
				}
				o_pos++;
			}

			/* copy it RATIO - 1 times */
			const Uint32 off_src = RATIO * (RATIO * O_WIDTH * y_o + x_in);
			for (int i = 1; i < RATIO; i++) {
				memcpy(pixels + off_src + RATIO * O_WIDTH * i,
					pixels + off_src, RATIO * width * sizeof(Uint32));
			}
		}
	}
}

void sdl_update_rect_window(const int x_in, const int y_in, const int width_in, const int height_in)
{
	calls++;
	if (pixels == NULL) return;

	if (SDL_LockMutex(PixelsMutex) == 0) {

		sdl_update_rect_pixels(x_in, y_in, width_in, height_in);

		if (memcmp(g_vga_memstart, vga_bak, O_WIDTH * O_HEIGHT) || pal_updated || win_resized) {
			if ((texture != NULL) && (renderer != NULL)) {
				updates++;
				SDL_UpdateTexture(texture, NULL, pixels, RATIO * O_WIDTH * sizeof(Uint32));
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
			}
		}

		pal_updated = 0;
		win_resized = 0;
		memcpy(vga_bak, g_vga_memstart, O_WIDTH * O_HEIGHT);

		if (SDL_UnlockMutex(PixelsMutex) == -1) {
			fprintf(stderr, "ERROR: Unlock Mutex in %s\n", __func__);
		}
	} else {
		fprintf(stderr, "ERROR: Lock Mutex in %s\n", __func__);
	}
}

SDL_Window* sdl_get_window(void)
{
	return window;
}

int sdl_get_ratio(void)
{
	return RATIO;
}

void sdl_change_window_size(SDL_mutex *timer_mutex)
{
	if (pixels == NULL) return;

	if (timer_mutex && (SDL_LockMutex(timer_mutex) == 0)) {

		if (SDL_LockMutex(PixelsMutex) == 0) {

			RATIO = (RATIO < MAX_RATIO) ? RATIO + 1 : 1;
			W_WIDTH = RATIO * O_WIDTH;
			W_HEIGHT = RATIO * O_HEIGHT;

			fprintf(stdout, "RATIO = %d W_WIDTH = %d W_HEIGHT = %d\n",
					RATIO, W_WIDTH, W_HEIGHT);


			free(pixels);
			pixels = NULL;
			SDL_DestroyTexture(texture);
			texture = NULL;
			SDL_DestroyRenderer(renderer);
			renderer = NULL;
			SDL_DestroyWindow(window);
			window = NULL;

			pixels = calloc(W_WIDTH * W_HEIGHT * sizeof(Uint32), 1);
			if (pixels == NULL) {
				fprintf(stderr, "ERROR: %s() failed to allocate pixels: %s\n",
							__func__, SDL_GetError());
				SDL_Quit();
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
				SDL_PIXELFORMAT_ABGR8888,
				SDL_TEXTUREACCESS_STREAMING,
				//SDL_TEXTUREACCESS_STATIC,
				W_WIDTH,
				W_HEIGHT
			);

			win_resized = 1;

			if (SDL_UnlockMutex(PixelsMutex) == -1) {
				fprintf(stderr, "ERROR: Unlock PixelsMutex in %s\n", __func__);
			}
		} else {
			fprintf(stderr, "ERROR: Lock PixelsMutex in %s\n", __func__);
		}

		if (SDL_UnlockMutex(timer_mutex) == -1) {
			fprintf(stderr, "ERROR: Unlock timer_mutex in %s\n", __func__);
		}
	} else {
		fprintf(stderr, "ERROR: Lock timer_mutex in %s\n", __func__);
	}
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
static inline Uint32 get_ABGR(const unsigned char *p) {
	return (p[0] << 2) | (p[1] << 10) | (p[2] << 18);
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
		palette[first_color + i] = get_ABGR(pointer + 3 * i);

	pal_updated = 1;
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
				d += O_WIDTH;
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

				d += O_WIDTH;
				s += width;
			}
			break;
		}
		/* copy each value using screen dimensions */
		case 3: {

			s += s_y * O_WIDTH + s_x;

			for (i = height; i > 0; i--) {
				memcpy(d, s, width);
				d += O_WIDTH;
				s += O_WIDTH;
			}
			break;
		}
		/* copy each value */
		default: {

			for (i = height; i > 0; i--) {
				memcpy(d, s, width);
				d += O_WIDTH;
				s += width;
			}
			break;
		}
	}
}

void fill_rect(unsigned char *p_in, const signed short color, const signed short width, const signed short height)
{
	unsigned char *p;
	signed short h;

	if ((width == O_WIDTH) && (height == O_HEIGHT)) {
		/* fullscreen fill */
		memset(p_in, color, O_WIDTH * O_HEIGHT);

	} else {
		p = p_in;
		for (h = height; h > 0; h--) {
			memset(p, color, width);
			p += O_WIDTH;
		}
	}
}

const unsigned short swap_u16(const unsigned short val)
{
	return ((val & 0xff) << 8) | ((val >> 8) & 0xff);
}

void vgalib_copy_to_screen(unsigned char *dst_in, unsigned char *src_in, const int width, const int height)
{
	unsigned char *src = src_in;
	unsigned char *dst = dst_in;
	signed short h;

	/* mode 0: copy from a buffer to screen */

	if ((width == O_WIDTH) && (height == O_HEIGHT)) {
		/* special case: full screen copy */
		memcpy(dst_in, src_in, O_WIDTH * O_HEIGHT);
	} else {

		/* regular case */
		for (h = height; h > 0; h--) {
			memcpy(dst, src, width);
			dst += O_WIDTH;
			src += width;
		}
	}
}

void vgalib_copy_from_screen(unsigned char *dst_in, unsigned char *src_in, const int width, const int height)
{
	unsigned char *src = src_in;
	unsigned char *dst = dst_in;
	int h;

	for (h = height; h > 0; h--) {
		memcpy(dst, src, width);
		dst += width;
		src += O_WIDTH;
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
