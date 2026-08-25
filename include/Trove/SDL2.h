#ifndef SDL2_H
#define SDL2_H

#include "windows_types.h"

extern "C" {
	typedef enum {
		SDL_SCANCODE_W = 26,
		SDL_SCANCODE_A = 4,
		SDL_SCANCODE_S = 22,
		SDL_SCANCODE_D = 7,
		SDL_SCANCODE_LSHIFT = 225,
		SDL_SCANCODE_SPACE = 44
	} SDL_Scancode;

	typedef const unsigned char* (__stdcall* SDL_GetKeyboardState_t)(int* numkeys);
	static SDL_GetKeyboardState_t SDL_GetKeyboardState = nullptr;

	static bool SDL_LoadFunctions(const char* path) {
		void* h = LoadLibraryA(path);
		if (!h) return false;
		SDL_GetKeyboardState = (SDL_GetKeyboardState_t)GetProcAddress(h, "SDL_GetKeyboardState");
		if (!SDL_GetKeyboardState) return false;

		return true;
	}
}

#endif // SDL2_H