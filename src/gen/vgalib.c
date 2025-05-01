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

extern signed short g_mouse1_event2;
extern signed short g_mouse_rightclick_event;

enum FB_VALUES {
	O_WIDTH = 320, O_HEIGHT = 200, MAX_RATIO = 8
};

static const int DEF_RATIO = 3;
static int RATIO = DEF_RATIO;
static int W_WIDTH = DEF_RATIO * O_WIDTH;
static int W_HEIGHT = DEF_RATIO * O_HEIGHT;

static Uint32 palette[256] = {0};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static Uint32 pixels[MAX_RATIO * MAX_RATIO * O_WIDTH * O_HEIGHT];

void set_video_mode(unsigned short mode)
{
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
}

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

int sdl_event_loop(const int cmd)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			/* return CTRL+Q */
			return (0x10 << 8) | 0x11;
		} else if (event.type == SDL_MOUSEMOTION) {
			g_mouse_moved = 1;
			/* Assume 320x200 */
			g_mouse_posx = event.motion.x / RATIO;
			g_mouse_posy = event.motion.y / RATIO;

		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == 1) {
				g_mouse1_event2 = 1;
			}
			if (event.button.button == 3) {
				g_mouse_rightclick_event = 1;
			}

		} else if (event.type == SDL_KEYDOWN) {
			if (cmd == 1) {
				// check if a key was pressed
				int pressed = (event.type == SDL_KEYDOWN) ? 1 : 0;
				SDL_Delay(10);
				//fprintf(stdout, "%s(%d) = %d %d\n", __func__, cmd, pressed, key_cnt);
				SDL_PushEvent(&event);
				return pressed;
			} else {

				SDL_Keymod m = SDL_GetModState();

				if (m & KMOD_CTRL) {
					switch (event.key.keysym.sym) {
						case SDLK_q:  return (0x10 << 8) | 0x11; break;
						case SDLK_F3: return (0x60 << 8); break;
						case SDLK_F4: return (0x61 << 8); break;
					}
				}

				switch (event.key.keysym.sym) {
					case SDLK_TAB:      sdl_change_window_size(); break;
					case SDLK_ESCAPE:   return (0x01 << 8) | 0x1b; break; //OK
					case SDLK_1:        return (0x02 << 8) | 0x31; break; //OK
					case SDLK_2:        return (0x03 << 8) | 0x32; break; //OK
					case SDLK_3:        return (0x04 << 8) | 0x33; break; //OK
					case SDLK_4:        return (0x05 << 8) | 0x34; break; //OK
					case SDLK_5:        return (0x06 << 8) | 0x35; break; //OK
					case SDLK_RETURN:   return (0x1c << 8) | 0x0d; break; //OK
					case SDLK_j:        return (0x24 << 8) | 0x6a; break; //OK
					case SDLK_y:        return (0x15 << 8) | 0x79; break; //DE
					case SDLK_z:        return (0x2c << 8) | 0x7a; break; //DE
					case SDLK_n:        return (0x31 << 8) | 0x6e; break; //OK
					case SDLK_UP:       return (0x48 << 8); break; //OK
					case SDLK_LEFT:     return (0x4b << 8); break; //OK
					case SDLK_RIGHT:    return (0x4d << 8); break; //OK
					case SDLK_DOWN:     return (0x50 << 8); break; //OK
					case SDLK_PAGEUP:   return (0x49 << 8); break; //OK
					case SDLK_PAGEDOWN: return (0x51 << 8); break; //OK
					case 0xe4:          return (0x28 << 8) | 0x84; break; //AE
					case 0xf6:          return (0x27 << 8) | 0x94; break; //OE
					case 0xfc:          return (0x1a << 8) | 0x81; break; //UE
					default:	    return event.key.keysym.sym & 0xff;
				}
			}
		}
	}

	return 0;
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
}

unsigned char* normalize_ptr(unsigned char *ptr)
{
	return ptr;
}
