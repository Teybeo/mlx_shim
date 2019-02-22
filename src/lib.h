#ifndef MLX_SHIM_LIB_H
#define MLX_SHIM_LIB_H

#include <SDL_video.h>
#include <SDL_render.h>

typedef int (*function_ptr)();

#define MAX_EVENT 32

#define KEY_DOWN 2
#define KEY_UP 3
#define MOUSE_DOWN 4
#define MOUSE_UP 5
#define MOUSE_MOTION 6

typedef struct lib_win {
	SDL_Window		*sdl_window;
	function_ptr	callback[MAX_EVENT];
	void			*(callback_arg[MAX_EVENT]);
} lib_win;

typedef struct lib_ctx {
	SDL_Renderer	*renderer;
	lib_win			window;
	int				(*loop_fn)(void*);
	void			*loop_arg;
} lib_ctx;

typedef struct lib_img {
	SDL_Texture		*texture;
	void			*buffer;
	int				width;
	int				height;
} lib_img;

#endif
