Nano
-----

Nano is a free, open source game engine / fantasy console inspired by Pico-8.
(1 internet point if you get the unix joke)

Dependencies:

* SDL2
* SDL2_Mixer
* Chaiscript
* PhysicsFS (with 7-zip support)
* libOgg/libVorbis/libVorbisFile

Contributing:

If you really want to, I mean, this is horrible code.
Seriously though, I could use some help fixing the timing on the game loop. (Nano.cpp, lines 59-154)

Licenses:

Licenses for all the libraries used are in LICENSES.txt, and this projects license (BSD) is in LICENSE.txt (I know, confusing names)

Creating for:

As of yet, no development guides are available. These will be made after LD41, because I'm a bit busy right now...

Extra notes:

If you happen to have a (windows) static-linkable copy of SDL2_Mixer built with libOgg and libVorbis statically linked, please... tell me? (idk, I don't like dlls)

If you would like to build for your platform, send the binary over and I can put it as a release.

Releases are compressed using UPX -9.