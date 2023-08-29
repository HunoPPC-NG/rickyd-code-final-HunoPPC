/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_soundsession.h"
#include "tinyxml2.h"

#define MIN_ALLOC_CHANNELS 8

RE_SoundSession::RE_SoundSession()
{
	if(channel_alloc.size() <= 0)
	{
		//first allocation
		channel_alloc.resize(MIN_ALLOC_CHANNELS);

		for(int i=0; i<MIN_ALLOC_CHANNELS; i++)
			channel_alloc[i]=false;
	}

	channel = get_free_channel();
	int max_chan = max_channels_used();
	Mix_AllocateChannels(max_chan);

	//std::cout << "RE_SoundSession: allocated "<< max_chan<<" channels\n";
	//std::cout << "RE_SoundSession: using channel "<< channel<<"\n";
}

RE_SoundSession::~RE_SoundSession()
{
	stop();
	channel_alloc[channel]=false;
	int max_chan = max_channels_used();
	Mix_AllocateChannels(max_chan);
}

int RE_SoundSession::max_channels_used()
{
	int ch= 0;
	for(int i=0; i<channel_alloc.size(); i++)
		if(channel_alloc[i])
		{
			ch = i+1;
		}

	if((ch < channel_alloc.size()) && (ch > MIN_ALLOC_CHANNELS))
		channel_alloc.resize(ch);
	return ch;
}

int RE_SoundSession::get_free_channel()
{
	int size= channel_alloc.size();
	for(int i=0; i<size; i++)
		if(!channel_alloc[i])
		{
			channel_alloc[i] = true;
			return i;
		}

	channel_alloc.resize(size+1);
	channel_alloc[size] = true;

	return size;
}

void RE_SoundSession::stop()
{
	if(is_playing())
		Mix_HaltChannel(channel);
}

void RE_SoundSession::play(RE_SoundBuffer *sound, int loops)
{
	if(sound == NULL)
		return;

 	int ret = Mix_PlayChannel(channel, sound->get_chunk() , loops);
	if(ret == -1)
	{
		std::cerr << "RE_SoundSession: Unable to play file: " << Mix_GetError() << "\n";
	}

}

bool RE_SoundSession::is_playing()
{
	return (Mix_Playing(channel) > 0);
}

