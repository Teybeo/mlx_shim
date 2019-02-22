#include "lib.h"

#include "SDL.h"

static int sdl_to_macos_key[SDL_NUM_SCANCODES];

void init_sdl_to_macos_keymap();
void send_mouse_event(SDL_MouseButtonEvent event, lib_win win);
void send_wheel_event(SDL_MouseWheelEvent event, lib_win win);
void send_key_event(SDL_KeyboardEvent event, lib_win win);

void	*mlx_init()
{
	lib_ctx*	ctx = calloc(sizeof(lib_ctx), 1);
	init_sdl_to_macos_keymap();
	SDL_Init(SDL_INIT_VIDEO);
	return ctx;
}

void	*mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title)
{
	lib_ctx *ctx = mlx_ptr;
	if (SDL_CreateWindowAndRenderer(size_x, size_y, SDL_WINDOW_SHOWN, &ctx->window.sdl_window, &ctx->renderer) != 0)
		puts(SDL_GetError());
	return &ctx->window;
}

void	*mlx_new_image(void *mlx_ptr, int width, int height)
{
	lib_ctx *ctx = mlx_ptr;

	lib_img* img = malloc(sizeof(lib_img));
	img->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_ARGB8888,
							SDL_TEXTUREACCESS_STREAMING, width, height);
	img->buffer = malloc(width * height * 4 * sizeof(char));
	img->width = width;
	img->height = height;
	return img;
}

int	mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr,
							   int x, int y)
{
	lib_ctx* ctx = mlx_ptr;
	lib_img* img = img_ptr;

	SDL_UpdateTexture(img->texture, NULL, img->buffer, img->width * sizeof(Uint32));
	SDL_RenderCopy(ctx->renderer, img->texture, NULL, NULL);
	SDL_RenderPresent(ctx->renderer);
	return 0;
}

int	mlx_hook(void *win_ptr, int x_event, int x_mask,
				int (*funct)(), void *param)
{
	lib_win* lib_win = win_ptr;
	lib_win->callback[x_event] = funct;
	lib_win->callback_arg[x_event] = param;
	return 0;
}

int	mlx_loop_hook (void *mlx_ptr, int (*funct_ptr)(), void *param)
{
	lib_ctx* ctx = mlx_ptr;
	ctx->loop_fn = funct_ptr;
	ctx->loop_arg = param;
	return 0;
}

int	mlx_loop (void *mlx_ptr)
{
	lib_ctx* ctx = mlx_ptr;
	while (1)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				return 0;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				send_mouse_event(ev.button, ctx->window);
				break;
			case SDL_MOUSEWHEEL:
				send_wheel_event(ev.wheel, ctx->window);
				break;
			case SDL_MOUSEMOTION:
				if (ctx->window.callback[MOUSE_MOTION] != NULL)
					ctx->window.callback[MOUSE_MOTION](ev.button.x, ev.button.y, ctx->window.callback_arg[MOUSE_MOTION]);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				send_key_event(ev.key, ctx->window);
					break;
			default:
				break;
			}
		}
		ctx->loop_fn(ctx->loop_arg);
	}
	return 0;
}

void send_key_event(SDL_KeyboardEvent event, lib_win win)
{
	int keycode = sdl_to_macos_key[event.keysym.scancode];
	int event_type = (event.type == SDL_KEYDOWN) ? KEY_DOWN : KEY_UP;

	// Don't send keys without a valid remapping (-1)
	if (win.callback[event_type] && keycode != -1)
		win.callback[event_type](keycode, win.callback_arg[event_type]);
}

void send_mouse_event(SDL_MouseButtonEvent event, lib_win win)
{
	int	button_id = event.button;
	int event_type;

	// Remap right-click
	if (button_id == SDL_BUTTON_RIGHT)
		button_id = 2;
	event_type = (event.type == SDL_MOUSEBUTTONDOWN) ? MOUSE_DOWN : MOUSE_UP;
	if (win.callback[event_type] != NULL)
		win.callback[event_type](button_id, event.x, event.y, win.callback_arg[event_type]);
}

void send_wheel_event(SDL_MouseWheelEvent event, lib_win win)
{
	int	button_id = (event.y > 0) ? 4 : 5;
	int x, y;

	SDL_GetMouseState(&x, &y);
	if (win.callback[MOUSE_DOWN])
		win.callback[MOUSE_DOWN](button_id, x, y, win.callback_arg[MOUSE_DOWN]);
}

char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel,
						   int *size_line, int *endian)
{
	return ((lib_img*)img_ptr)->buffer;
}

int	mlx_clear_window(void *mlx_ptr, void *win_ptr)
{
	lib_ctx *ctx = mlx_ptr;
	SDL_RenderClear(ctx->renderer);
	return 0;
}

int	mlx_string_put(void *mlx_ptr, void *win_ptr, int x, int y, int color,
					  char *string)
{
	return 0;
}

int	mlx_do_key_autorepeatoff(void *mlx_ptr)
{
	return 0;
}

int	mlx_do_key_autorepeaton(void *mlx_ptr)
{
	return 0;
}

int	mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	return 0;
}

// Based on https://eastmanreference.com/complete-list-of-applescript-key-codes
void	init_sdl_to_macos_keymap()
{
	memset(sdl_to_macos_key, -1, sizeof(sdl_to_macos_key));
	sdl_to_macos_key[SDL_SCANCODE_A] = 0;
	sdl_to_macos_key[SDL_SCANCODE_B] = 11;
	sdl_to_macos_key[SDL_SCANCODE_C] = 8;
	sdl_to_macos_key[SDL_SCANCODE_D] = 2;
	sdl_to_macos_key[SDL_SCANCODE_E] = 14;
	sdl_to_macos_key[SDL_SCANCODE_F] = 3;
	sdl_to_macos_key[SDL_SCANCODE_G] = 5;
	sdl_to_macos_key[SDL_SCANCODE_H] = 4;
	sdl_to_macos_key[SDL_SCANCODE_I] = 34;
	sdl_to_macos_key[SDL_SCANCODE_J] = 38;
	sdl_to_macos_key[SDL_SCANCODE_K] = 40;
	sdl_to_macos_key[SDL_SCANCODE_L] = 37;
	sdl_to_macos_key[SDL_SCANCODE_M] = 46;
	sdl_to_macos_key[SDL_SCANCODE_N] = 45;
	sdl_to_macos_key[SDL_SCANCODE_O] = 31;
	sdl_to_macos_key[SDL_SCANCODE_P] = 35;
	sdl_to_macos_key[SDL_SCANCODE_Q] = 12;
	sdl_to_macos_key[SDL_SCANCODE_R] = 15;
	sdl_to_macos_key[SDL_SCANCODE_S] = 1;
	sdl_to_macos_key[SDL_SCANCODE_T] = 17;
	sdl_to_macos_key[SDL_SCANCODE_U] = 32;
	sdl_to_macos_key[SDL_SCANCODE_V] = 9;
	sdl_to_macos_key[SDL_SCANCODE_W] = 13;
	sdl_to_macos_key[SDL_SCANCODE_X] = 7;
	sdl_to_macos_key[SDL_SCANCODE_Y] = 16;
	sdl_to_macos_key[SDL_SCANCODE_Z] = 6;
	sdl_to_macos_key[SDL_SCANCODE_ESCAPE] = 53;
	sdl_to_macos_key[SDL_SCANCODE_UP] = 13;
	sdl_to_macos_key[SDL_SCANCODE_DOWN] = 1;
	sdl_to_macos_key[SDL_SCANCODE_RIGHT] = 2;
	sdl_to_macos_key[SDL_SCANCODE_LEFT] = 0;
	sdl_to_macos_key[SDL_SCANCODE_R] = 15;
	sdl_to_macos_key[SDL_SCANCODE_G] = 5;
	sdl_to_macos_key[SDL_SCANCODE_EQUALS] = 7;
	sdl_to_macos_key[SDL_SCANCODE_RIGHTBRACKET] = 8;
//	sdl_to_macos_key[SDL_SCANCODE_ZOOM] = 12;
//	sdl_to_macos_key[SDL_SCANCODE_DEZOOM] = 6;
	sdl_to_macos_key[SDL_SCANCODE_P] = 35;
	sdl_to_macos_key[SDL_SCANCODE_KP_MINUS] = 78;
	sdl_to_macos_key[SDL_SCANCODE_KP_PLUS] = 69;
	sdl_to_macos_key[SDL_SCANCODE_1] = 18;
	sdl_to_macos_key[SDL_SCANCODE_2] = 19;
	sdl_to_macos_key[SDL_SCANCODE_3] = 20;
	sdl_to_macos_key[SDL_SCANCODE_KP_1] = 83;
	sdl_to_macos_key[SDL_SCANCODE_KP_2] = 84;
}
