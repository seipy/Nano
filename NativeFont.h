#pragma once
#include <SDL/SDL.h>
#include <iostream>
#include <string>

class NativeFont
{
public:
	NativeFont();
	void _DB_DRAW(SDL_Surface* surface);
    void setFormat(SDL_Surface* surface);
	void blit(unsigned char input, SDL_Surface* surface, SDL_Rect* dest);
	void blitString(const char* input, SDL_Surface* surface, SDL_Rect* dest);
	~NativeFont();
private:
	SDL_Surface* fontImg = nullptr;
	SDL_Rect rect;
};

