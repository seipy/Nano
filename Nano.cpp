#include "HardGraphics.h"
#include "Nano.h"


void Nano::putText(const char* text, int posX, int posY)
{
	m_Dest.x = posX;
	m_Dest.y = posY;
	m_NativeFont.blitString(text, m_WinSurface, &m_Dest);
	SDL_UpdateWindowSurface(m_Window);
	//std::cout << text << std::endl;
}

SDL_RWops* Nano::loadFromArchive(const char* file)
{
	PHYSFS_file* cb = PHYSFS_openRead(file);
	PHYSFS_sint64 file_size = PHYSFS_fileLength(cb);
	char* myBuf = new char[file_size];
	PHYSFS_readBytes(cb, myBuf, file_size);
	return SDL_RWFromMem(myBuf, file_size);
}

Nano::Nano()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	m_Window = SDL_CreateWindow("Nano", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0);
	if (!m_Window)
		std::cout << "Failed to create a window! Error: " << SDL_GetError() << std::endl;
    m_WinSurface = SDL_CreateRGBSurface(0, 256, 256, 32, 0, 0, 0, 0);
    m_MainSurface = SDL_GetWindowSurface(m_Window);
	if (!m_WinSurface || !m_MainSurface)
		return;
    m_NativeFont.setFormat(m_WinSurface);
	SDL_RWops* z = SDL_RWFromMem(nanoPx, nanoSize);
	SDL_Surface* nanoImg = SDL_LoadBMP_RW(z, 1);
	if (!nanoImg)
		return;
	SDL_SetColorKey(nanoImg, SDL_TRUE, SDL_MapRGB(m_WinSurface->format, 0, 0, 0));
	SDL_SetWindowIcon(m_Window, nanoImg);

	m_Dest.x = 0; m_Dest.y = 0;
	m_Dest.w = 8; m_Dest.h = 8;

	SDL_BlitSurface(nanoImg, NULL, m_WinSurface, &m_Dest);
	m_Dest.y = 32;
	m_NativeFont.blitString("Nano v0.2A [LUDUM BUILD]", m_WinSurface, &m_Dest);
	m_NativeFont.blitString("Attempting to load cartridge...", m_WinSurface, &m_Dest);
	if (SDL_BlitScaled(m_WinSurface, NULL, m_MainSurface, NULL) != 0)
		std::cout << SDL_GetError() << std::endl;

	SDL_UpdateWindowSurface(m_Window);
	Mix_Init(MIX_INIT_OGG);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void Nano::loop(std::function<void()> loopFun, std::function<void(int key)> pressedFun, std::function<void(int key)> releasedFun)
{
    Uint32 next = 0;
    Uint32 now = 0;
    const int ticks = 16;
	int counter = 0;
	SDL_Event event;
	bool running = true;
	while (running)
	{
        next = SDL_GetTicks() + ticks;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if ( pressedFun != NULL && event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					pressedFun(0);
					break;
				case SDLK_DOWN:
					pressedFun(1);
					break;
				case SDLK_LEFT:
					pressedFun(2);
					break;
				case SDLK_RIGHT:
					pressedFun(3);
					break;
				case SDLK_s:
					pressedFun(4);
					break;
				case SDLK_z:
					pressedFun(5);
					break;
				case SDLK_a:
					pressedFun(6);
					break;
				case SDLK_x:
					pressedFun(7);
					break;
				}
			}
			else if (releasedFun != NULL && event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					releasedFun(0);
					break;
				case SDLK_DOWN:
					releasedFun(1);
					break;
				case SDLK_LEFT:
					releasedFun(2);
					break;
				case SDLK_RIGHT:
					releasedFun(3);
					break;
				case SDLK_s:
					releasedFun(4);
					break;
				case SDLK_z:
					releasedFun(5);
					break;
				case SDLK_a:
					releasedFun(6);
					break;
				case SDLK_x:
					releasedFun(7);
					break;
				}
			}
		}
		{
			try
			{
				loopFun();
			}
			catch (const chaiscript::exception::eval_error &e) {
				std::cout << e.pretty_print() << '\n';
			}
		} 
		//loopFun();
		if (SDL_BlitScaled(m_WinSurface, NULL, m_MainSurface, NULL) != 0)
			std::cout << SDL_GetError() << std::endl;
        now = SDL_GetTicks();
        if (next > now)
            SDL_Delay(next - now);
		next += ticks;
		SDL_UpdateWindowSurface(m_Window);
	}
}

Nano::~Nano()
{
	for (int i = 0; i < 8; i++)
	{
		Mix_FreeChunk(m_SoundEffects[i]);
		m_SoundEffects[i] = nullptr;
	}
	SDL_DestroyWindow(m_Window);
	m_Window = NULL;
	m_WinSurface = NULL;
	Mix_Quit();
	SDL_Quit();
}

void Nano::clearScreen(int colour)
{
	SDL_FillRect(m_WinSurface, NULL, colour);
}

void Nano::drawRect(int colour, int x, int y, int w, int h)
{
	SDL_Rect shape;
	shape.x = x; shape.y = y;
	shape.w = w; shape.h = h;
	SDL_FillRect(m_WinSurface, &shape, colour);
}

void Nano::setSound(int index, std::string file)
{
	m_SoundEffects[index] = Mix_LoadWAV_RW(loadFromArchive(file.c_str()), 1);
}

void Nano::playSound(int index)
{
	Mix_PlayChannel(-1, m_SoundEffects[index], 0);
}

void Nano::setSprite(int index, std::string file)
{
	m_Sprites[index] = SDL_ConvertSurface(SDL_LoadBMP_RW(loadFromArchive(file.c_str()), 1), m_WinSurface->format, 0);
	SDL_SetColorKey(m_Sprites[index], SDL_TRUE, SDL_MapRGB(m_WinSurface->format, 255, 0, 255));
}

void Nano::drawSprite(int index, int dx, int dy)
{
	m_Dest.x = dx;
	m_Dest.y = dy;
	SDL_BlitSurface(m_Sprites[index], NULL, m_WinSurface, &m_Dest);
}

void Nano::setMusicTrack(std::string file)
{
	m_MusicTrack = Mix_LoadMUS_RW(loadFromArchive(file.c_str()), 1);
	std::cout << Mix_GetError() << std::endl;
}

void Nano::startMusic()
{
	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(m_MusicTrack, -1);
}

void Nano::stopMusic()
{
	if (Mix_PlayingMusic() != 0)
		Mix_HaltMusic();
}

void Nano::togglePauseMusic()
{
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}

void Nano::drawSpriteControlled(int index, int srcX, int srcY, int srcW, int srcH, int x, int y)
{
	m_Src.x = srcX;
	m_Src.y = srcY;
	m_Src.w = srcW;
	m_Src.h = srcH;
	m_Dest.x = x;
	m_Dest.y = y;
	SDL_BlitSurface(m_Sprites[index], &m_Src, m_WinSurface, &m_Dest);
}
