#ifndef MLX_SHIM_LIB_H
#define MLX_SHIM_LIB_H

typedef struct lib_ctx {
	SDL_Renderer	*renderer;
	SDL_Window		*window;
	int				(*loop_fn)(void*);
	void			*loop_arg;
} 				lib_ctx;

typedef struct lib_img {
	SDL_Texture		*texture;
	void			*buffer;
	int				width;
	int				height;
}				lib_img;

#endif
