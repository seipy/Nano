#include "NativeFont.h"
#include "FontRes.h"

NativeFont::NativeFont()
{
	SDL_RWops * z = SDL_RWFromMem(fontmap, fontmapSize);
    if (!z)
        std::cout << "Help" << std::endl;
    fontImg = SDL_LoadBMP_RW(z, 1); // SDL_LoadBMP("res/fontmap.bmp");
	rect = SDL_Rect();
	rect.w = 8;
	rect.h = 8;
	//SDL_RWclose(z);
}

void NativeFont::_DB_DRAW(SDL_Surface* surface)
{
	rect.x = 0;
	rect.y = 0;
	SDL_BlitSurface(fontImg, NULL, surface, &rect);
}

void NativeFont::setFormat(SDL_Surface* surface)
{
	SDL_SetColorKey(fontImg, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
}

void NativeFont::blit(unsigned char input, SDL_Surface* surface, SDL_Rect* dest)
{
	rect.x = 8 * input;
	SDL_BlitSurface(fontImg, &rect, surface, dest);
}

void NativeFont::blitString(const char* input, SDL_Surface* surface, SDL_Rect* dest)
{
	for (int i = 0; input[i] != '\0'; i++) {

		rect.x = 8 * input[i];

		int result = SDL_BlitSurface(fontImg, &rect, surface, dest);

		if (result < 0) {
			std::cout << "Problem!" << std::endl;
		}
		else
		{
			dest->x += 8;
		}

	}
	dest->x = 0;
	dest->y += 8;
}

NativeFont::~NativeFont()
{
	//SDL_FreeSurface(fontImg);
}
