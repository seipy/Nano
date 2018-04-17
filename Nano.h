#pragma once
#include <chaiscript/chaiscript.hpp>
#include <physFS/physfs.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

#include "NativeFont.h"

#define doDebug(x) try{ x; } catch (const chaiscript::exception::eval_error &e) { std::cout << e.pretty_print() << '\n'; }

class Nano
{
public:
	Nano();
	void loop(std::function<void()> loopFun, std::function<void(int)> pressedFun, std::function<void(int)> releasedFun);
	~Nano();

	void putText(const char* text, int posX, int posY);
	static SDL_RWops* loadFromArchive(const char* file);
	void clearScreen(int colour);
	void drawRect(int colour, int x, int y, int w, int h);
	void setSound(int index, std::string file);
	void playSound(int index);
	void setSprite(int index, std::string file);
	void drawSprite(int index, int dx, int dy);
	void setMusicTrack(std::string file);
	void startMusic();
	void stopMusic();
	void togglePauseMusic();
	void drawSpriteControlled(int index, int srcX, int srcY, int srcW, int srcH, int x, int y);

	SDL_Window* getWindow() const { return m_Window; }
	SDL_Surface* getWinSurface() const { return m_WinSurface; }
	NativeFont getNativeFont() const { return m_NativeFont; }
	SDL_Rect getDestRect() const { return m_Dest; }
private:
	SDL_Window* m_Window;
	SDL_Surface* m_WinSurface;
    SDL_Surface* m_MainSurface;
	NativeFont m_NativeFont;
	SDL_Rect m_Dest;
	SDL_Rect m_Src;

	Mix_Music* m_MusicTrack;
	SDL_Surface* m_Sprites[16];
	Mix_Chunk* m_SoundEffects[16];
};

