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

#if defined(__GNUC__)
#include <omp.h>
#endif

#include "vgalib.h"

#if !defined(__BORLANDC__)

extern unsigned char *g_vga_memstart;

enum { DEF_RATIO = 3 };
static int RATIO = DEF_RATIO;
static int W_WIDTH = DEF_RATIO * O_WIDTH;
static int W_HEIGHT = DEF_RATIO * O_HEIGHT;

static Uint32 palette[256] = {0};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_mutex *PixelsMutex = NULL;

static uint32_t *pixels = NULL;
static uint8_t *vga_bak = NULL;
static int forced_update = 0;
static int pal_logic = 0;
static int pal_updated = 0;
static int win_resized = 0;

/* instrumentation to count calls for sdl_update_rect_window */
static int calls = 0, updates = 0;

void sdl_toggle_pal_logic(void)
{
	if (pal_logic) pal_logic = 0; else pal_logic = 1;
}

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
			for (Uint32 j = 0; j < info.num_texture_formats; j++) {
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
				case SDL_PIXELFORMAT_ABGR8888: fprintf(stderr, "ABGR8888 "); break;
				default: fprintf(stderr, "0x%x ", info.texture_formats[j]);
				}
			}
			fprintf(stderr, "\n");
			fprintf(stderr, "max res  = %dx%d\n\n", info.max_texture_width, info.max_texture_height);
		}
	}
}

void sdl_init_video(void)
{
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
		SDL_RENDERER_SOFTWARE
	);

	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		W_WIDTH,
		W_HEIGHT
	);

	PixelsMutex = SDL_CreateMutex();
	if (PixelsMutex == NULL) {
		fprintf(stderr, "Could not create PixelsMutex: %s\n", SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

	vga_bak = (uint8_t*)calloc(O_WIDTH * O_HEIGHT * sizeof(Uint8), 1);
	if (vga_bak == NULL) {
		fprintf(stderr, "ERROR: cannot allocate vga_bak\n");
		exit(-1);
	}
	pixels = (uint32_t*)calloc(W_WIDTH * W_HEIGHT * sizeof(Uint32), 1);
	if (pixels == NULL) {
		fprintf(stderr, "ERROR: cannot allocate pixels\n");
		exit(-1);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void sdl_exit_video(void)
{
	if (SDL_LockMutex(PixelsMutex) == 0) {

		free(pixels);

		if (SDL_UnlockMutex(PixelsMutex) == -1) {
			fprintf(stderr, "ERROR: Unlock Mutex in %s\n", __func__);
		}
	} else {
		fprintf(stderr, "ERROR: Lock Mutex in %s\n", __func__);
	}

	free(vga_bak);
	SDL_DestroyMutex(PixelsMutex);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	fprintf(stderr, "sdl_update_rect_window() calls = %d updates = %d\n", calls, updates);
}

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

#if defined(__GNUC__)
	double start, time_ms;
#endif

	if (forced_update || memcmp(g_vga_memstart, vga_bak, O_WIDTH * O_HEIGHT) || pal_updated || win_resized) {

		if (SDL_LockMutex(PixelsMutex) == 0) {

			updates++;
#if defined(__GNUC__)
			start = omp_get_wtime();
#endif
			sdl_update_rect_pixels(x_in, y_in, width_in, height_in);
#if defined(__GNUC__)
			time_ms = (omp_get_wtime() - start) * 1000;
			fprintf(stderr, "%s() scaling   = %f ms\n", __func__, time_ms);
#endif

			if ((texture != NULL) && (renderer != NULL)) {
				SDL_UpdateTexture(texture, NULL, pixels, W_WIDTH * sizeof(Uint32));
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
#if defined(__GNUC__)
				start = omp_get_wtime();
#endif
				SDL_RenderPresent(renderer);
#if defined(__GNUC__)
				time_ms = (omp_get_wtime() - start) * 1000;
				fprintf(stderr, "%s() rendering = %f ms\n", __func__, time_ms);
#endif
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
}

void sdl_forced_update(void)
{
	forced_update = 1;
	sdl_update_rect_window(0, 0, O_WIDTH, O_HEIGHT);
	forced_update = 0;
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

			SDL_SetWindowSize(window, W_WIDTH, W_HEIGHT);

			pixels = (uint32_t*)calloc(W_WIDTH * W_HEIGHT * sizeof(Uint32), 1);
			if (pixels == NULL) {
				fprintf(stderr, "ERROR: %s() failed to allocate pixels: %s\n",
							__func__, SDL_GetError());
				SDL_Quit();
				exit(-1);
			}

			renderer = SDL_CreateRenderer(
				window,
				-1,
				SDL_RENDERER_SOFTWARE
			);

			texture = SDL_CreateTexture(
				renderer,
				SDL_PIXELFORMAT_ABGR8888,
				SDL_TEXTUREACCESS_STREAMING,
				W_WIDTH,
				W_HEIGHT
			);

			win_resized = 1;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);

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

	sdl_update_rect_window(0, 0, O_WIDTH, O_HEIGHT);
}

static inline Uint32 get_ABGR(const unsigned char *p)
{
	return (p[0] << 2) | (p[1] << 10) | (p[2] << 18);
}

static inline Uint32 get_ABGR_grey(const unsigned char *p)
{
	const int avg = 4 * (p[0] + p[1] +  p[2]) / 3;
	return (avg) | (avg << 8) | (avg << 16);
}

void set_palette(const unsigned char *pointer, const unsigned char first_color, const unsigned short colors)
{
	signed int i;

	for (i = 0; i < colors; i++)
		palette[first_color + i] = get_ABGR(pointer + 3 * i);

	if (pal_logic) {
		pal_updated = 1;
		sdl_update_rect_window(0, 0, O_WIDTH, O_HEIGHT);
	}
}
#endif

#if defined(__BORLANDC__)
void set_video_mode(unsigned short mode)
{
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
}

void set_video_page(unsigned short page)
{
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
}

void save_display_stat(signed short *pointer)
{
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
}

void set_palette(const unsigned char *pointer, const unsigned char first_color, const unsigned short colors)
{
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
}
#endif

/**
 * \brief fills an area on a screen
 * \param[in] dst_in pointer to the start position on the screen
 * \param[in] color the color value to fill the screen with
 * \param[in] width width of the area in pixels
 * \param[in] height height of the area in pixels
 */
void vgalib_fill_rect(unsigned char *dst_in, const int color, const int width, const int height)
{
	unsigned char *dst;
	int h;

	if ((width == O_WIDTH) && (height == O_HEIGHT)) {
		/* fullscreen fill */
		memset(dst_in, color, O_WIDTH * O_HEIGHT);
	} else {
		dst = dst_in;
		for (h = height; h > 0; h--) {
			memset(dst, color, width);
			dst += O_WIDTH;
		}
	}

#if !defined(__BORLANDC__)
	/* check if its really the vga memory, dst can also be another buffer */
	const int offset = dst_in - g_vga_memstart;
	if ((0 <= offset) && (offset <= O_WIDTH * O_HEIGHT)) {
		const int y = offset / O_WIDTH;
		const int x = offset - y * O_WIDTH;

		sdl_update_rect_window(x, y, width, height);
	}
#endif
}

/**
 * \brief copy the content from an array to a screen
 * \param[in] dst_in pointer to the start position on the screen
 * \param[in] src_in pointer to the array
 * \param[in] width width of the area in pixels
 * \param[in] height height of the area in pixels
 * \note works the same way as pic_copy() with mode 0
 */
void vgalib_copy_to_screen(unsigned char *dst_in, unsigned char *src_in, const int width, const int height)
{
	unsigned char *src = src_in;
	unsigned char *dst = dst_in;
	int h;

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

#if !defined(__BORLANDC__)
	/* check if its really the vga memory, dst can also be another buffer */
	const int offset = dst_in - g_vga_memstart;
	if ((0 <= offset) && (offset <= O_WIDTH * O_HEIGHT)) {
		const int y = offset / O_WIDTH;
		const int x = offset - y * O_WIDTH;

		sdl_update_rect_window(x, y, width, height);
	}
#endif
}

/**
 * \brief copy the content from an array to a screen
 * \param[in] dst_in pointer to the start position on the screen
 * \param[in] src_in pointer to the array
 * \param[in] width width of the area in pixels
 * \param[in] height height of the area in pixels
 * \note works the same way as pic_copy() with mode 1
 */
void vgalib_copy_to_screen_bounded(unsigned char *dst_in, unsigned char *src_in, const int width, const int height)
{
	unsigned char *src = src_in;
	unsigned char *dst = dst_in;
	int h, j;


	/* regular case */
	for (h = height; h > 0; h--) {
		for (j = 0; j < width; j++) {
			if (dst[j] < 0xc8) dst[j] = src[j];
		}
		dst += O_WIDTH;
		src += width;
	}

#if !defined(__BORLANDC__)
	/* check if its really the vga memory, dst can also be another buffer */
	const int offset = dst_in - g_vga_memstart;
	if ((0 <= offset) && (offset <= O_WIDTH * O_HEIGHT)) {
		const int y = offset / O_WIDTH;
		const int x = offset - y * O_WIDTH;

		sdl_update_rect_window(x, y, width, height);
	}
#endif
}

/**
 * \brief copy the content from an array to a screen
 * \param[in] dst_in pointer to the start position on the screen
 * \param[in] src_in pointer to the array
 * \param[in] width width of the area in pixels
 * \param[in] height height of the area in pixels
 * \note works the same way as pic_copy() with mode 2
 */
void vgalib_copy_to_screen_nonzero(unsigned char *dst_in, unsigned char *src_in, const int width, const int height)
{
	unsigned char *src = src_in;
	unsigned char *dst = dst_in;
	int h, j;


	/* regular case */
	for (h = height; h > 0; h--) {
		for (j = 0; j < width; j++) {
			if (src[j]) dst[j] = src[j];
		}
		dst += O_WIDTH;
		src += width;
	}

#if !defined(__BORLANDC__)
	/* check if its really the vga memory, dst can also be another buffer */
	const int offset = dst_in - g_vga_memstart;
	if ((0 <= offset) && (offset <= O_WIDTH * O_HEIGHT)) {
		const int y = offset / O_WIDTH;
		const int x = offset - y * O_WIDTH;

		sdl_update_rect_window(x, y, width, height);
	}
#endif
}

/**
 * \brief copies an area from one screen to another
 * \param[in] dst_in pointer to the start position on the screen
 * \param[in] src_in pointer to the start position on the screen
 * \param[in] width width of the area in pixels
 * \param[in] height height of the area in pixels
 * \note formerly known as pic_copy() with mode 3
 */
void vgalib_screen_copy(unsigned char *dst_in, unsigned char *src_in, const int width, const int height)
{
	unsigned char *dst = dst_in;
	unsigned char *src = src_in;
	int i;

	for (i = height; i > 0; i--) {
		memcpy(dst, src, width);
		dst += O_WIDTH;
		src += O_WIDTH;
	}

#if !defined(__BORLANDC__)
	/* check if its really the vga memory, dst can also be another buffer */
	const int offset = dst_in - g_vga_memstart;
	if ((0 <= offset) && (offset <= O_WIDTH * O_HEIGHT)) {
		const int y = offset / O_WIDTH;
		const int x = offset - y * O_WIDTH;

		sdl_update_rect_window(x, y, width, height);
	}
#endif
}

/**
 * \brief copy the content from a screen to an array
 * \param[in] dst_in pointer to the array
 * \param[in] src_in pointer to the start position on the screen
 * \param[in] width width of the area in pixels
 * \param[in] height height of the area in pixels
 */
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
