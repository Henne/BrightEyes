/*
 *	Rewrite of DSA1 Generator v1.05_de seg005 (Rasterlib)
 *	Functions rewritten: 2/2
 *
 *	We only rewrite those which differ from the game.
 *	These functions were written in assembler and are
 *      just a clean C-implementation.
*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

extern unsigned char *g_vga_memstart;

extern signed short g_mouse_posx;
extern signed short g_mouse_posy;
extern signed short g_mouse_moved;

extern signed short g_mouse1_event1;
extern signed short g_mouse1_event2;
extern signed short g_mouse2_event;

extern signed short g_in_key_ext;
extern signed short g_in_key_ascii;

static const int RATIO = 1;
static const int O_WIDTH = 320;
static const int O_HEIGHT = 200;
static const int W_WIDTH = RATIO * 320;
static const int W_HEIGHT = RATIO * 200;

static Uint32 palette[256] = {0};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static Uint32 *pixels = NULL;

int g_lets_quit = 0;

void set_video_mode(unsigned short mode)
{
	if (mode == 0x13) {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
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

		pixels = calloc(W_WIDTH * W_HEIGHT * sizeof(Uint32), 1);
	} else {
		free(pixels);
		pixels = NULL;
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

void update_sdl_window(void)
{
	if (pixels == NULL) return;

	int pos = 0;
	for (int y = 0; y < O_HEIGHT; y++) {
		pos = y * O_WIDTH;
		for (int x = 0; x < O_WIDTH; x++) {
			pixels[pos] = palette[g_vga_memstart[pos]];
			pos++;
		}
	}

	SDL_UpdateTexture(texture, NULL, pixels, W_WIDTH * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(16);
}

void sdl_event_loop(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			g_lets_quit = 1;
		} else if (event.type == SDL_MOUSEMOTION) {
			g_mouse_moved = 1;
			/* Assume 320x200 */
			g_mouse_posx = event.motion.x;
			g_mouse_posy = event.motion.y;
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == 1) {
				g_mouse1_event1 = 1;
				g_mouse1_event2 = 1;
			}
			if (event.button.button == 3) {
				g_mouse2_event = 1;
			}
		} else if (event.type == SDL_KEYDOWN) {
			SDL_Keymod m = SDL_GetModState();
			if (m & KMOD_CTRL) {
				switch (event.key.keysym.sym) {
					case SDLK_F3: g_in_key_ext = 0x60; break;
					case SDLK_F4: g_in_key_ext = 0x61; break;
				}
			}

			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: g_in_key_ext = 0x01; break;
				case SDLK_1: g_in_key_ext = 0x02; break;
				case SDLK_2: g_in_key_ext = 0x03; break;
				case SDLK_3: g_in_key_ext = 0x04; break;
				case SDLK_4: g_in_key_ext = 0x05; break;
				case SDLK_5: g_in_key_ext = 0x06; break;
				case SDLK_RETURN: g_in_key_ext = 0x1c; break;
				case SDLK_j: g_in_key_ext = 0x24; break;
				case SDLK_y: g_in_key_ext = 0x2c; break;
				case SDLK_n: g_in_key_ext = 0x31; break;
				case SDLK_UP: g_in_key_ext = 0x48; break;
				case SDLK_LEFT: g_in_key_ext = 0x4b; break;
				case SDLK_RIGHT: g_in_key_ext = 0x4d; break;
				case SDLK_DOWN: g_in_key_ext = 0x50; break;
				case SDLK_PAGEUP: g_in_key_ext = 0x49; break;
				case SDLK_PAGEDOWN: g_in_key_ext = 0x51; break;
			}

			//g_in_key_ascii = event.key.keysym.sym;
		}
	}
}

void set_video_page(unsigned short mode)
{
}

void save_display_stat(signed short *p)
{
}

static inline Uint32 get_ARGB(const unsigned char *p) {
	return (p[2] << 2) | (p[1] << 10) | (p[0] << 18);
}

void set_color(const unsigned char *ptr, const unsigned char color)
{
	palette[color] = get_ARGB(ptr);

	update_sdl_window();
}

void set_palette(const unsigned char *ptr, const unsigned char first_color, const unsigned short colors)
{
	signed int i;
	for (i = 0; i < colors; i++)
		palette[first_color + i] = get_ARGB(ptr + 3 * i);

	update_sdl_window();
}

void draw_h_line(unsigned short offset, unsigned short count, unsigned short color)
{
	//unsigned char *ptr = MK_FP(0xa000, offset);
	unsigned char *ptr = g_vga_memstart + offset;
	unsigned short i;

	for (i = 0; i < count; i++)
		ptr[i] = color;

	update_sdl_window();
}

void draw_h_spaced_dots(unsigned short offset, unsigned short width, signed short color, unsigned short space)
{
	//unsigned char *ptr = MK_FP(0xa000, offset);
	unsigned char *ptr = g_vga_memstart + offset;
	unsigned short i;

	for (i = 0; i < width; i++) {
		ptr[0] = color;
		ptr += space;
	}

	update_sdl_window();
}

void pic_copy(unsigned char *dst, unsigned short x, unsigned short y, unsigned short d1, unsigned short d2,
		unsigned short v1, unsigned short v2, unsigned short d3, unsigned short d4,
		unsigned short w, unsigned short h, unsigned char *src, unsigned short mode)
{
	unsigned char *d; // es:di
	unsigned char *s; // ds:si

	d = dst + y * 320 + x;
	s = src;

	switch (mode) {
		/* this is not used in GEN */
		case 1: {
			signed short diff; // bx
			signed short i;	   // cx	

			diff = 320 - w;
			do {
				for (i = w; i; i--) {
					if (d[0] < 0xc8)
						d[0] = s[0];
					s++;
					d++;
				}
				d += diff;
			} while (--h > 0);
			break;
		}
		case 2: {
			unsigned short diff, i;
			unsigned char al;

			diff = 320 - w;

			do {
				for (i = w; i != 0; i--) {
					if ((al = *s++))
						d[0] = al;
					d++;
				}
				d += diff;
			} while (--h > 0);

			break;
		}
		case 3: {
			unsigned short diff, i;

			s += v2 * 320 + v1;
			diff = 320 - w;

			do {
				for (i = w; i > 0; i--) {
					d[0] = s[0];
					d++;
					s++;
				}
				d += diff;
				s += diff;
			} while (--h > 0);
			break;
		}
		default: {
			signed short diff; // bx
			signed short i;    // cx

			diff = 320 - w;

			do {
				for (i = w; i; i--) {
					d[0] = s[0];
					d++;
					s++;
				}
				d += diff;
			} while (--h > 0);

			break;
		}
	}

	update_sdl_window();
}

#if defined(__BORLANDC__)
void save_rect(unsigned short p_seg, unsigned short p_off, unsigned char *src, signed short diffX, signed short diffY)
#else
void save_rect(unsigned char *p_in, unsigned char *src, signed short diffX, signed short diffY)
#endif
{

}

#if defined(__BORLANDC__)
void fill_rect(unsigned short p_seg, unsigned short p_off, signed short color, signed short width, signed short height)
#else
void fill_rect(unsigned char *p_in, signed short color, signed short width, signed short height)
#endif
{
#if defined(__BORLANDC__)
	unsigned char *p = MK_FP(p_seg, p_off);
#else
	unsigned char *p = p_in;
#endif
	signed short x;

	for (; height; height--) {
		for (x = 0; x < width; x++) {
			p[0] = color;
			p++;
		}
		p += 320 - width;
	}

	update_sdl_window();
}

unsigned short swap_u16(unsigned short val)
{
	return (val << 8) | (val >> 8);
}

void copy_to_screen(unsigned char *src, unsigned char *dst, signed short w, signed short h, signed short mode)
{
	signed short v1;
	signed short v2;
	signed short i;

	v1 = v2 = 320 - w;

	switch (mode & 0x7f) {
		case 0:	v2 = 0;
			break;
		case 2: v1 = 0;
			break;
	}

	if (mode & 0x80) {
		/* This does not happen */
		//D1_ERR("%s mode bit set\n", __func__);
		//exit(0);
	} else {
		for (; h; h--) {
			for (i = 0; i < w; i++) {
				dst[0] = src[0];
				dst++;
				src++;
			}
			dst += v1;
			src += v2;
		}
	}

	update_sdl_window();
}

unsigned char* normalize_ptr(unsigned char *ptr)
{
	return ptr;
}
