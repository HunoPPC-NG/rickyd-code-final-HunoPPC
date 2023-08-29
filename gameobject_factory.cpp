/*
** RickyD
**
** Author: Paulo Assis <pj.assis@gmail.com>
**
** License: GPL v2 or later
*/

#include "gameobject_factory.h"
#include "gameobject_player.h"
#include "gameobject_bomb.h"
#include "gameobject_bullet.h"
#include "gameobject_roller1.h"
#include "gameobject_roller2.h"
#include "gameobject_roller3.h"
#include "gameobject_roller4.h"
#include "gameobject_bonus1.h"
#include "gameobject_bonus2.h"
#include "gameobject_bonus3.h"
#include "gameobject_bonus4.h"
#include "gameobject_jewel.h"
#include "gameobject_climber1.h"
#include "gameobject_climber2.h"
#include "gameobject_climber3.h"
#include "gameobject_walker1.h"
#include "gameobject_walker2.h"
#include "gameobject_walker3.h"
#include "gameobject_walker4.h"
#include "gameobject_walker5.h"
#include "gameobject_projectile1.h"
#include "gameobject_projectile2.h"
#include "gameobject_projectile3.h"
#include "gameobject_vprojectile1.h"
#include "gameobject_vprojectile2.h"
#include "gameobject_statickiller1.h"
#include "gameobject_statickiller2.h"
#include "gameobject_bulletpack.h"
#include "gameobject_bombpack.h"
#include "gameobject_blastwall1.h"
#include "gameobject_blastwall2.h"
#include "gameobject_blastwall3.h"
#include "gameobject_movingwall.h"
#include "gameobject_movingwall2.h"
#include "gameobject_movingwall3.h"
#include "gameobject_movingwall4.h"
#include "gameobject_wagon.h"
#include "gameobject_crane.h"
#include "gameobject_fireball.h"
#include "gameobject_fallplatform1.h"
#include "gameobject_fallplatform2.h"
#include "gameobject_spikerock.h"
#include "gameobject_falltrap.h"
#include "gameobject_traptrigger1.h"
#include "gameobject_traptrigger2.h"
#include "gameobject_bat.h"
#include "gameobject_flame.h"
#include "gameobject_explosion.h"
#include "gameobject_jumpplatform.h"
#include "gameobject_elevator.h"
#include "gameobject_background1.h"
#include "gameobject_background2.h"
#include "gameobject_background3.h"
#include "gameobject_background4.h"
#include "gameobject_background5.h"
#include "gameobject_background6.h"
#include "gameobject_background7.h"
#include "gameobject_background8.h"
#include "gameobject_background9.h"
#include "gameobject_laser.h"
#include "gameobject_electricity.h"
#include "gameobject_nokill.h"
#include "gameobject_saw.h"

GameObjectFactory::GameObjectFactory()
{
	//rick 1 and 2 common objects
	Register("Player", &GameObject_Player::Create);
	Register("Bomb", &GameObject_Bomb::Create);
	Register("Bullet", &GameObject_Bullet::Create);

	Register("Bulletpack", &GameObject_Bulletpack::Create);
	Register("Bombpack", &GameObject_Bombpack::Create);

	Register("Bonus1", &GameObject_Bonus1::Create);
	Register("Bonus2", &GameObject_Bonus2::Create);
	Register("Bonus3", &GameObject_Bonus3::Create);
	Register("Bonus4", &GameObject_Bonus4::Create);

	Register("StaticKiller1", &GameObject_StaticKiller1::Create);
	Register("StaticKiller2", &GameObject_StaticKiller2::Create);

	Register("Roller1", &GameObject_Roller1::Create);
	Register("Roller2", &GameObject_Roller2::Create);
	Register("Roller3", &GameObject_Roller3::Create);
	Register("Roller4", &GameObject_Roller4::Create);

	Register("Climber1", &GameObject_Climber1::Create);
	Register("Climber2", &GameObject_Climber2::Create);
	Register("Climber3", &GameObject_Climber3::Create);

	Register("Walker1", &GameObject_Walker1::Create);
	Register("Walker2", &GameObject_Walker2::Create);
	Register("Walker3", &GameObject_Walker3::Create);
	Register("Walker4", &GameObject_Walker4::Create);
	Register("Walker5", &GameObject_Walker5::Create);

	Register("Projectile1", &GameObject_Projectile1::Create);
	Register("Projectile2", &GameObject_Projectile2::Create);
	Register("Projectile3", &GameObject_Projectile3::Create);

	Register("Vprojectile1", &GameObject_Vprojectile1::Create);
	Register("Vprojectile2", &GameObject_Vprojectile2::Create);

	Register("Traptrigger1", &GameObject_Traptrigger1::Create);
	Register("Traptrigger2", &GameObject_Traptrigger2::Create);

	Register("Falltrap", &GameObject_Falltrap::Create);

	Register("Blastwall1", &GameObject_Blastwall1::Create);
	Register("Blastwall2", &GameObject_Blastwall2::Create);
	Register("Blastwall3", &GameObject_Blastwall3::Create);

	Register("Background1", &GameObject_Background1::Create);
	Register("Background2", &GameObject_Background2::Create);
	Register("Background3", &GameObject_Background3::Create);
	Register("Background4", &GameObject_Background4::Create);
	Register("Background5", &GameObject_Background5::Create);
	Register("Background6", &GameObject_Background6::Create);
	Register("Background7", &GameObject_Background7::Create);
	Register("Background8", &GameObject_Background8::Create);
	Register("Background9", &GameObject_Background9::Create);

	Register("Elevator", &GameObject_Elevator::Create);

	Register("Spikerock", &GameObject_Spikerock::Create);

	Register("Flame", &GameObject_Flame::Create);
	Register("Explosion", &GameObject_Explosion::Create);
	Register("Fallplatform1", &GameObject_Fallplatform1::Create);
	Register("Fallplatform2", &GameObject_Fallplatform2::Create);
	Register("JumpPlatform", &GameObject_JumpPlatform::Create);
	Register("Nokill", &GameObject_Nokill::Create);

	//rick 1 specific objects
	Register("Movingwall", &GameObject_Movingwall::Create);
	Register("Movingwall2", &GameObject_Movingwall2::Create);
	Register("Movingwall3", &GameObject_Movingwall3::Create);
	Register("Movingwall4", &GameObject_Movingwall4::Create);

	Register("Jewel", &GameObject_Jewel::Create);

	Register("Wagon", &GameObject_Wagon::Create);
	Register("Crane", &GameObject_Crane::Create);
	Register("Fireball", &GameObject_Fireball::Create);
	Register("Bat", &GameObject_Bat::Create);

	//rick 2 specific objects
	Register("Laser", &GameObject_Laser::Create);
	Register("Electricity", &GameObject_Electricity::Create);
	Register("Saw", &GameObject_Saw::Create);
}

void GameObjectFactory::Register(const std::string &objectName,  CreateGameObjectFn pfnCreate)
{
	m_FactoryMap[objectName] = pfnCreate;
}

GameObject *GameObjectFactory::CreateGameObject(const std::string &objectName, World *world, tinyxml2::XMLElement* object_element)
{
	GameObject* g_obj = NULL;
	FactoryMap::iterator it = m_FactoryMap.find(objectName);
	if( it != m_FactoryMap.end() )
	{
		g_obj = it->second(world, object_element);
	}

	return g_obj;
}
