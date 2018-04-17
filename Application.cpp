#define CHAISCRIPT_NO_THREADS 1

#include <cstdlib>

#include "Nano.h"

static Nano nn;
static chaiscript::ChaiScript chai;

static void printText(const std::string text, int x, int y)
{
	nn.putText(text.c_str(), x, y);
}

static void clearScreen(int colour)
{
	nn.clearScreen(colour);
}

static void drawRect(int x, int y, int w, int h, int colour)
{
	nn.drawRect(colour, x, y, w, h);
}

static void loadWav(int index, std::string name)
{
	nn.setSound(index, name);
}

static void playWav(int index)
{
	nn.playSound(index);
}

static void loadBmp(int index, std::string name)
{
	nn.setSprite(index, name);
}

static void drawBmp(int index, int x, int y)
{
	nn.drawSprite(index, x, y);
}

static void loadMus(std::string name)
{
	nn.setMusicTrack(name);
}

static void startMus()
{
	nn.startMusic();
}

static void stopMus()
{
	nn.stopMusic();
}

static void togglePaused()
{
	nn.togglePauseMusic();
}

static void controlledDrawBmp(int srcIndex, int srcX, int srcY, int srcW, int srcH, int x, int y)
{
	nn.drawSpriteControlled(srcIndex, srcX, srcY, srcW, srcH, x, y);
}

static float random()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

static char* getText(const char* file)
{
	PHYSFS_file* cb = PHYSFS_openRead(file);
	PHYSFS_sint64 file_size = PHYSFS_fileLength(cb);
	char* myBuf = new char[file_size];
	PHYSFS_readBytes(cb, myBuf, file_size);
	return myBuf;
}

int main(int argc, char** argv) 
{
	PHYSFS_init(argv[0]);
	if (argc < 2)
		PHYSFS_addToSearchPath("cartridge.n9o", 1);
	else
		PHYSFS_addToSearchPath(argv[1], 1);
	chai.add_global_const(chaiscript::const_var(0), "BUTTON_UP");
	chai.add_global_const(chaiscript::const_var(1), "BUTTON_DOWN");
	chai.add_global_const(chaiscript::const_var(2), "BUTTON_LEFT");
	chai.add_global_const(chaiscript::const_var(3), "BUTTON_RIGHT");
	chai.add_global_const(chaiscript::const_var(4), "BUTTON_A"); //x, z, s, a | a, b, x, y
	chai.add_global_const(chaiscript::const_var(5), "BUTTON_B");
	chai.add_global_const(chaiscript::const_var(6), "BUTTON_C");
	chai.add_global_const(chaiscript::const_var(7), "BUTTON_D");

	chai.add(chaiscript::fun(&printText), "text");
	chai.add(chaiscript::fun(&clearScreen), "clr");
	chai.add(chaiscript::fun([]() { return clearScreen(0x000000); }), "clr");
	chai.add(chaiscript::fun(&drawRect), "draw");
	chai.add(chaiscript::fun(&random), "rand");

	chai.add(chaiscript::fun(&loadWav), "loadSound");
	chai.add(chaiscript::fun(&playWav), "playSound");
	chai.add(chaiscript::fun(&loadBmp), "loadSprite");
	chai.add(chaiscript::fun(&drawBmp), "drawSprite");
	chai.add(chaiscript::fun(&controlledDrawBmp), "drawSprite");

	chai.add(chaiscript::fun(&loadMus), "loadMusic");
	chai.add(chaiscript::fun(&startMus), "startMusic");
	chai.add(chaiscript::fun(&stopMus), "stopMusic");
	chai.add(chaiscript::fun(&togglePaused), "pauseMusic");

	std::function<void()> init = nullptr;
	std::function<void()> loop = nullptr;
	std::function<void(int)> keyPressed = nullptr;
	std::function<void(int)> keyReleased = nullptr;

	try
	{
		chai.eval(getText("game.chai"));
	}
	catch (const chaiscript::exception::eval_error &e) {
		std::cout << "Base chai exception: " << e.pretty_print() << '\n';
		nn.putText("Cartridge loading failed", 0, 48);
	}

	{
		try
		{
			init = chai.eval<std::function<void()>>("init");
			init();
		}
		catch (const chaiscript::exception::eval_error &e)
		{

			std::cout << "Cartridge loaded with no 'init' function. Continuing..." << std::endl << e.pretty_print() << std::endl;
		}
		try
		{
			keyPressed = chai.eval<std::function<void(int)>>("keyPressed");
		}
		catch (const chaiscript::exception::eval_error &e)
		{

			std::cout << "Cartridge loaded with no 'keyPressed' function. Continuing..." << std::endl << e.pretty_print() << std::endl;
			keyReleased = NULL;
		}
		try
		{
			keyReleased = chai.eval<std::function<void(int)>>("keyReleased");
		}
		catch (const chaiscript::exception::eval_error &e)
		{

			std::cout << "Cartridge loaded with no 'keyReleased' function. Continuing..." << std::endl << e.pretty_print() << std::endl;
			keyReleased = NULL;
		}
		try
		{
			loop = chai.eval<std::function<void()>>("loop");
		}
		catch (const chaiscript::exception::eval_error &e)
		{
			std::cout << "Cartridge loaded with no 'loop' function. Aborting." << std::endl << e.pretty_print() << std::endl;
			return -1;
		}
	}

	nn.loop(loop, keyPressed, keyReleased);
	nn.~Nano();
	return 0;
}

/*
catch (const chaiscript::exception::eval_error &e) {
std::cout << e.pretty_print() << '\n';
}
*/
