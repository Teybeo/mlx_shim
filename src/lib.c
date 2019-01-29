#include <stddef.h>

#include "SDL.h"
#include "lib.h"

void	*mlx_init()
{
	lib_ctx*	ctx = malloc(sizeof(lib_ctx));
	ctx->loop_arg = NULL;
	ctx->loop_fn = NULL;
	ctx->renderer = NULL;
	ctx->window = NULL;
	SDL_Init(SDL_INIT_VIDEO);
	return ctx;
}

void	*mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title)
{
	lib_ctx *ctx = mlx_ptr;
	if (SDL_CreateWindowAndRenderer(size_x, size_y, SDL_WINDOW_SHOWN, &ctx->window, &ctx->renderer) != 0)
		printf("OLOLO\n");
	return ctx->window;
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
			if (ev.type == SDL_QUIT)
				return 0;
		}
		puts("Henlo");
		ctx->loop_fn(ctx->loop_arg);
	}
	return 0;
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

int	mlx_hook(void *win_ptr, int x_event, int x_mask,
				int (*funct)(), void *param)
{
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