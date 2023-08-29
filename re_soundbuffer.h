/*
** RickyD
** 
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#ifndef header_re_soundbuffer
#define header_re_soundbuffer

#include "precomp.h"
#include "re_resource.h"
#include "re_resourcemanager.h"

class RE_SoundBuffer: public RE_Resource
{
public:
	RE_SoundBuffer(std::string xml_path, RE_ResourceManager *resource);
	RE_SoundBuffer(std::string wav_file);
	~RE_SoundBuffer();
	void set_volume(float volume);
	Mix_Chunk* get_chunk(){return sound;}


private:
	void load(std::string wav_file);
	Mix_Chunk *sound;
};

#endif
