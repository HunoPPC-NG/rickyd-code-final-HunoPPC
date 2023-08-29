/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_soundsession
#define header_re_soundsession

#include "precomp.h"
#include "re_soundbuffer.h"

static std::vector<bool> channel_alloc;

class RE_SoundSession
{
public:
	RE_SoundSession();
	~RE_SoundSession();
	//loops => -1 infinite loop; 0 play once; 1 play twice; ...
	void play(RE_SoundBuffer* sound, int loops = 0);
	void stop();
	bool is_playing();

private:
	int get_free_channel();
	int max_channels_used();
	int channel;
};

#endif
