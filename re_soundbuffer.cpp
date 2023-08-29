/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "re_soundbuffer.h"
#include "tinyxml2.h"

//SDL_mixer Max volume
#ifndef MIX_MAX_VOLUME
#define MIX_MAX_VOLUME 128
#endif

RE_SoundBuffer::RE_SoundBuffer(std::string xml_path, RE_ResourceManager *resource)
{
	sound = NULL;

	tinyxml2::XMLElement* buffer_elem = resource->getElement(xml_path);
	if(!buffer_elem)
	{
		std::cerr << "RE_SoundBuffer: couldn't create sound buffer:"<< xml_path <<"\n";
		return;
	}

	std::string wav_file = buffer_elem->Attribute( "file" );
	wav_file = resource->get_data_dir() + wav_file;

	load(wav_file);
}

RE_SoundBuffer::RE_SoundBuffer(std::string wav_file)
{
	load(wav_file);
}

RE_SoundBuffer::~RE_SoundBuffer()
{
	if(sound)
		Mix_FreeChunk(sound);
}

void RE_SoundBuffer::load(std::string wav_file)
{
	sound = Mix_LoadWAV(wav_file.c_str());
	if(sound == NULL)
	{
		std::cerr << "RE_SoundBuffer: unable to load " << wav_file << " : " << Mix_GetError() << "\n";
	}
}

//set volume from 0 to 128
void RE_SoundBuffer::set_volume(float volume)
{
	if(sound == NULL)
		return;

	if(volume > 1)
		volume = 1;

	int vol = floor(volume * MIX_MAX_VOLUME);
	Mix_VolumeChunk(sound, vol);
}

