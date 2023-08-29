# Makefile created by HunoPPC 2017

CPP      = g++
CC       = gcc
RES      = 
OBJ      = application.o gameobject.o gameobject_background.o gameobject_background1.o gameobject_background2.o gameobject_background3.o gameobject_background4.o gameobject_background5.o gameobject_background6.o gameobject_background7.o gameobject_background8.o gameobject_background9.o gameobject_bat.o gameobject_blastwall.o gameobject_blastwall1.o gameobject_blastwall2.o gameobject_blastwall3.o gameobject_bomb.o gameobject_bombpack.o gameobject_bonus.o gameobject_bonus1.o gameobject_bonus2.o gameobject_bonus3.o gameobject_bonus4.o gameobject_bullet.o gameobject_bulletpack.o gameobject_climber.o gameobject_climber1.o gameobject_climber2.o gameobject_climber3.o gameobject_crane.o gameobject_electricity.o gameobject_elevator.o gameobject_explosion.o gameobject_factory.o gameobject_fallplatform.o gameobject_fallplatform1.o gameobject_fallplatform2.o gameobject_falltrap.o gameobject_fireball.o gameobject_flame.o gameobject_jewel.o gameobject_jumpplatform.o gameobject_laser.o gameobject_moving.o gameobject_movingwall.o gameobject_movingwall2.o gameobject_movingwall3.o gameobject_movingwall4.o gameobject_nokill.o gameobject_player.o gameobject_projectile.o gameobject_projectile1.o gameobject_projectile2.o gameobject_projectile3.o gameobject_roller.o gameobject_roller1.o gameobject_roller2.o gameobject_roller3.o gameobject_roller4.o gameobject_saw.o gameobject_spikerock.o gameobject_statickiller.o gameobject_statickiller1.o gameobject_statickiller2.o gameobject_traptrigger.o gameobject_traptrigger1.o gameobject_traptrigger2.o gameobject_vprojectile.o gameobject_vprojectile1.o gameobject_vprojectile2.o gameobject_wagon.o gameobject_walker.o gameobject_walker1.o gameobject_walker2.o gameobject_walker3.o gameobject_walker4.o gameobject_walker5.o intro.o keys_dialog.o map.o precomp.o program.o re_button.o re_events.o re_font.o re_gamecontext.o re_helpers.o re_inputbox.o re_levelmanager.o re_listbox.o re_optionparser.o re_point.o re_rect.o re_resource.o re_resourcemanager.o re_scores.o re_soundbuffer.o re_soundsession.o re_sprite.o re_tmxconverter.o score_dialog.o tilemap.o tinyxml2.o welcome.o welcome_dialog.o world.o $(RES)
LINKOBJ  = application.o gameobject.o gameobject_background.o gameobject_background1.o gameobject_background2.o gameobject_background3.o gameobject_background4.o gameobject_background5.o gameobject_background6.o gameobject_background7.o gameobject_background8.o gameobject_background9.o gameobject_bat.o gameobject_blastwall.o gameobject_blastwall1.o gameobject_blastwall2.o gameobject_blastwall3.o gameobject_bomb.o gameobject_bombpack.o gameobject_bonus.o gameobject_bonus1.o gameobject_bonus2.o gameobject_bonus3.o gameobject_bonus4.o gameobject_bullet.o gameobject_bulletpack.o gameobject_climber.o gameobject_climber1.o gameobject_climber2.o gameobject_climber3.o gameobject_crane.o gameobject_electricity.o gameobject_elevator.o gameobject_explosion.o gameobject_factory.o gameobject_fallplatform.o gameobject_fallplatform1.o gameobject_fallplatform2.o gameobject_falltrap.o gameobject_fireball.o gameobject_flame.o gameobject_jewel.o gameobject_jumpplatform.o gameobject_laser.o gameobject_moving.o gameobject_movingwall.o gameobject_movingwall2.o gameobject_movingwall3.o gameobject_movingwall4.o gameobject_nokill.o gameobject_player.o gameobject_projectile.o gameobject_projectile1.o gameobject_projectile2.o gameobject_projectile3.o gameobject_roller.o gameobject_roller1.o gameobject_roller2.o gameobject_roller3.o gameobject_roller4.o gameobject_saw.o gameobject_spikerock.o gameobject_statickiller.o gameobject_statickiller1.o gameobject_statickiller2.o gameobject_traptrigger.o gameobject_traptrigger1.o gameobject_traptrigger2.o gameobject_vprojectile.o gameobject_vprojectile1.o gameobject_vprojectile2.o gameobject_wagon.o gameobject_walker.o gameobject_walker1.o gameobject_walker2.o gameobject_walker3.o gameobject_walker4.o gameobject_walker5.o intro.o keys_dialog.o map.o precomp.o program.o re_button.o re_events.o re_font.o re_gamecontext.o re_helpers.o re_inputbox.o re_levelmanager.o re_listbox.o re_optionparser.o re_point.o re_rect.o re_resource.o re_resourcemanager.o re_scores.o re_soundbuffer.o re_soundsession.o re_sprite.o re_tmxconverter.o score_dialog.o tilemap.o tinyxml2.o welcome.o welcome_dialog.o world.o $(RES)
LIBS     = -lHJWSDL2 -lpthread -lSDL2_mixer -lsmpeg2 -lmikmod  -lmodplug -lvorbisfile -lvorbis -logg -lflac -lSDL2_image -lSDL2_gfx -lwebp -lpng -lz  -ltiff -ljpeg -lz -lm -lSDL2_ttf -lfreetype-lSDL2 -lauto -lm
INCS     = -DDATA_DIR=""
CXXINCS  = 
BIN      = RickD.exe
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS) 
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

application.o: application.cpp
	$(CPP) -c application.cpp -o application.o $(CXXFLAGS)

gameobject.o: gameobject.cpp
	$(CPP) -c gameobject.cpp -o gameobject.o $(CXXFLAGS)

gameobject_background.o: gameobject_background.cpp
	$(CPP) -c gameobject_background.cpp -o gameobject_background.o $(CXXFLAGS)

gameobject_background1.o: gameobject_background1.cpp
	$(CPP) -c gameobject_background1.cpp -o gameobject_background1.o $(CXXFLAGS)

gameobject_background2.o: gameobject_background2.cpp
	$(CPP) -c gameobject_background2.cpp -o gameobject_background2.o $(CXXFLAGS)

gameobject_background3.o: gameobject_background3.cpp
	$(CPP) -c gameobject_background3.cpp -o gameobject_background3.o $(CXXFLAGS)

gameobject_background4.o: gameobject_background4.cpp
	$(CPP) -c gameobject_background4.cpp -o gameobject_background4.o $(CXXFLAGS)

gameobject_background5.o: gameobject_background5.cpp
	$(CPP) -c gameobject_background5.cpp -o gameobject_background5.o $(CXXFLAGS)

gameobject_background6.o: gameobject_background6.cpp
	$(CPP) -c gameobject_background6.cpp -o gameobject_background6.o $(CXXFLAGS)

gameobject_background7.o: gameobject_background7.cpp
	$(CPP) -c gameobject_background7.cpp -o gameobject_background7.o $(CXXFLAGS)

gameobject_background8.o: gameobject_background8.cpp
	$(CPP) -c gameobject_background8.cpp -o gameobject_background8.o $(CXXFLAGS)

gameobject_background9.o: gameobject_background9.cpp
	$(CPP) -c gameobject_background9.cpp -o gameobject_background9.o $(CXXFLAGS)

gameobject_bat.o: gameobject_bat.cpp
	$(CPP) -c gameobject_bat.cpp -o gameobject_bat.o $(CXXFLAGS)

gameobject_blastwall.o: gameobject_blastwall.cpp
	$(CPP) -c gameobject_blastwall.cpp -o gameobject_blastwall.o $(CXXFLAGS)

gameobject_blastwall1.o: gameobject_blastwall1.cpp
	$(CPP) -c gameobject_blastwall1.cpp -o gameobject_blastwall1.o $(CXXFLAGS)

gameobject_blastwall2.o: gameobject_blastwall2.cpp
	$(CPP) -c gameobject_blastwall2.cpp -o gameobject_blastwall2.o $(CXXFLAGS)

gameobject_blastwall3.o: gameobject_blastwall3.cpp
	$(CPP) -c gameobject_blastwall3.cpp -o gameobject_blastwall3.o $(CXXFLAGS)

gameobject_bomb.o: gameobject_bomb.cpp
	$(CPP) -c gameobject_bomb.cpp -o gameobject_bomb.o $(CXXFLAGS)

gameobject_bombpack.o: gameobject_bombpack.cpp
	$(CPP) -c gameobject_bombpack.cpp -o gameobject_bombpack.o $(CXXFLAGS)

gameobject_bonus.o: gameobject_bonus.cpp
	$(CPP) -c gameobject_bonus.cpp -o gameobject_bonus.o $(CXXFLAGS)

gameobject_bonus1.o: gameobject_bonus1.cpp
	$(CPP) -c gameobject_bonus1.cpp -o gameobject_bonus1.o $(CXXFLAGS)

gameobject_bonus2.o: gameobject_bonus2.cpp
	$(CPP) -c gameobject_bonus2.cpp -o gameobject_bonus2.o $(CXXFLAGS)

gameobject_bonus3.o: gameobject_bonus3.cpp
	$(CPP) -c gameobject_bonus3.cpp -o gameobject_bonus3.o $(CXXFLAGS)

gameobject_bonus4.o: gameobject_bonus4.cpp
	$(CPP) -c gameobject_bonus4.cpp -o gameobject_bonus4.o $(CXXFLAGS)

gameobject_bullet.o: gameobject_bullet.cpp
	$(CPP) -c gameobject_bullet.cpp -o gameobject_bullet.o $(CXXFLAGS)

gameobject_bulletpack.o: gameobject_bulletpack.cpp
	$(CPP) -c gameobject_bulletpack.cpp -o gameobject_bulletpack.o $(CXXFLAGS)

gameobject_climber.o: gameobject_climber.cpp
	$(CPP) -c gameobject_climber.cpp -o gameobject_climber.o $(CXXFLAGS)

gameobject_climber1.o: gameobject_climber1.cpp
	$(CPP) -c gameobject_climber1.cpp -o gameobject_climber1.o $(CXXFLAGS)

gameobject_climber2.o: gameobject_climber2.cpp
	$(CPP) -c gameobject_climber2.cpp -o gameobject_climber2.o $(CXXFLAGS)

gameobject_climber3.o: gameobject_climber3.cpp
	$(CPP) -c gameobject_climber3.cpp -o gameobject_climber3.o $(CXXFLAGS)

gameobject_crane.o: gameobject_crane.cpp
	$(CPP) -c gameobject_crane.cpp -o gameobject_crane.o $(CXXFLAGS)

gameobject_electricity.o: gameobject_electricity.cpp
	$(CPP) -c gameobject_electricity.cpp -o gameobject_electricity.o $(CXXFLAGS)

gameobject_elevator.o: gameobject_elevator.cpp
	$(CPP) -c gameobject_elevator.cpp -o gameobject_elevator.o $(CXXFLAGS)

gameobject_explosion.o: gameobject_explosion.cpp
	$(CPP) -c gameobject_explosion.cpp -o gameobject_explosion.o $(CXXFLAGS)

gameobject_factory.o: gameobject_factory.cpp
	$(CPP) -c gameobject_factory.cpp -o gameobject_factory.o $(CXXFLAGS)

gameobject_fallplatform.o: gameobject_fallplatform.cpp
	$(CPP) -c gameobject_fallplatform.cpp -o gameobject_fallplatform.o $(CXXFLAGS)

gameobject_fallplatform1.o: gameobject_fallplatform1.cpp
	$(CPP) -c gameobject_fallplatform1.cpp -o gameobject_fallplatform1.o $(CXXFLAGS)

gameobject_fallplatform2.o: gameobject_fallplatform2.cpp
	$(CPP) -c gameobject_fallplatform2.cpp -o gameobject_fallplatform2.o $(CXXFLAGS)

gameobject_falltrap.o: gameobject_falltrap.cpp
	$(CPP) -c gameobject_falltrap.cpp -o gameobject_falltrap.o $(CXXFLAGS)

gameobject_fireball.o: gameobject_fireball.cpp
	$(CPP) -c gameobject_fireball.cpp -o gameobject_fireball.o $(CXXFLAGS)

gameobject_flame.o: gameobject_flame.cpp
	$(CPP) -c gameobject_flame.cpp -o gameobject_flame.o $(CXXFLAGS)

gameobject_jewel.o: gameobject_jewel.cpp
	$(CPP) -c gameobject_jewel.cpp -o gameobject_jewel.o $(CXXFLAGS)

gameobject_jumpplatform.o: gameobject_jumpplatform.cpp
	$(CPP) -c gameobject_jumpplatform.cpp -o gameobject_jumpplatform.o $(CXXFLAGS)

gameobject_laser.o: gameobject_laser.cpp
	$(CPP) -c gameobject_laser.cpp -o gameobject_laser.o $(CXXFLAGS)

gameobject_moving.o: gameobject_moving.cpp
	$(CPP) -c gameobject_moving.cpp -o gameobject_moving.o $(CXXFLAGS)

gameobject_movingwall.o: gameobject_movingwall.cpp
	$(CPP) -c gameobject_movingwall.cpp -o gameobject_movingwall.o $(CXXFLAGS)

gameobject_movingwall2.o: gameobject_movingwall2.cpp
	$(CPP) -c gameobject_movingwall2.cpp -o gameobject_movingwall2.o $(CXXFLAGS)

gameobject_movingwall3.o: gameobject_movingwall3.cpp
	$(CPP) -c gameobject_movingwall3.cpp -o gameobject_movingwall3.o $(CXXFLAGS)

gameobject_movingwall4.o: gameobject_movingwall4.cpp
	$(CPP) -c gameobject_movingwall4.cpp -o gameobject_movingwall4.o $(CXXFLAGS)

gameobject_nokill.o: gameobject_nokill.cpp
	$(CPP) -c gameobject_nokill.cpp -o gameobject_nokill.o $(CXXFLAGS)

gameobject_player.o: gameobject_player.cpp
	$(CPP) -c gameobject_player.cpp -o gameobject_player.o $(CXXFLAGS)

gameobject_projectile.o: gameobject_projectile.cpp
	$(CPP) -c gameobject_projectile.cpp -o gameobject_projectile.o $(CXXFLAGS)

gameobject_projectile1.o: gameobject_projectile1.cpp
	$(CPP) -c gameobject_projectile1.cpp -o gameobject_projectile1.o $(CXXFLAGS)

gameobject_projectile2.o: gameobject_projectile2.cpp
	$(CPP) -c gameobject_projectile2.cpp -o gameobject_projectile2.o $(CXXFLAGS)

gameobject_projectile3.o: gameobject_projectile3.cpp
	$(CPP) -c gameobject_projectile3.cpp -o gameobject_projectile3.o $(CXXFLAGS)

gameobject_roller.o: gameobject_roller.cpp
	$(CPP) -c gameobject_roller.cpp -o gameobject_roller.o $(CXXFLAGS)

gameobject_roller1.o: gameobject_roller1.cpp
	$(CPP) -c gameobject_roller1.cpp -o gameobject_roller1.o $(CXXFLAGS)

gameobject_roller2.o: gameobject_roller2.cpp
	$(CPP) -c gameobject_roller2.cpp -o gameobject_roller2.o $(CXXFLAGS)

gameobject_roller3.o: gameobject_roller3.cpp
	$(CPP) -c gameobject_roller3.cpp -o gameobject_roller3.o $(CXXFLAGS)

gameobject_roller4.o: gameobject_roller4.cpp
	$(CPP) -c gameobject_roller4.cpp -o gameobject_roller4.o $(CXXFLAGS)

gameobject_saw.o: gameobject_saw.cpp
	$(CPP) -c gameobject_saw.cpp -o gameobject_saw.o $(CXXFLAGS)

gameobject_spikerock.o: gameobject_spikerock.cpp
	$(CPP) -c gameobject_spikerock.cpp -o gameobject_spikerock.o $(CXXFLAGS)

gameobject_statickiller.o: gameobject_statickiller.cpp
	$(CPP) -c gameobject_statickiller.cpp -o gameobject_statickiller.o $(CXXFLAGS)

gameobject_statickiller1.o: gameobject_statickiller1.cpp
	$(CPP) -c gameobject_statickiller1.cpp -o gameobject_statickiller1.o $(CXXFLAGS)

gameobject_statickiller2.o: gameobject_statickiller2.cpp
	$(CPP) -c gameobject_statickiller2.cpp -o gameobject_statickiller2.o $(CXXFLAGS)

gameobject_traptrigger.o: gameobject_traptrigger.cpp
	$(CPP) -c gameobject_traptrigger.cpp -o gameobject_traptrigger.o $(CXXFLAGS)

gameobject_traptrigger1.o: gameobject_traptrigger1.cpp
	$(CPP) -c gameobject_traptrigger1.cpp -o gameobject_traptrigger1.o $(CXXFLAGS)

gameobject_traptrigger2.o: gameobject_traptrigger2.cpp
	$(CPP) -c gameobject_traptrigger2.cpp -o gameobject_traptrigger2.o $(CXXFLAGS)

gameobject_vprojectile.o: gameobject_vprojectile.cpp
	$(CPP) -c gameobject_vprojectile.cpp -o gameobject_vprojectile.o $(CXXFLAGS)

gameobject_vprojectile1.o: gameobject_vprojectile1.cpp
	$(CPP) -c gameobject_vprojectile1.cpp -o gameobject_vprojectile1.o $(CXXFLAGS)

gameobject_vprojectile2.o: gameobject_vprojectile2.cpp
	$(CPP) -c gameobject_vprojectile2.cpp -o gameobject_vprojectile2.o $(CXXFLAGS)

gameobject_wagon.o: gameobject_wagon.cpp
	$(CPP) -c gameobject_wagon.cpp -o gameobject_wagon.o $(CXXFLAGS)

gameobject_walker.o: gameobject_walker.cpp
	$(CPP) -c gameobject_walker.cpp -o gameobject_walker.o $(CXXFLAGS)

gameobject_walker1.o: gameobject_walker1.cpp
	$(CPP) -c gameobject_walker1.cpp -o gameobject_walker1.o $(CXXFLAGS)

gameobject_walker2.o: gameobject_walker2.cpp
	$(CPP) -c gameobject_walker2.cpp -o gameobject_walker2.o $(CXXFLAGS)

gameobject_walker3.o: gameobject_walker3.cpp
	$(CPP) -c gameobject_walker3.cpp -o gameobject_walker3.o $(CXXFLAGS)

gameobject_walker4.o: gameobject_walker4.cpp
	$(CPP) -c gameobject_walker4.cpp -o gameobject_walker4.o $(CXXFLAGS)

gameobject_walker5.o: gameobject_walker5.cpp
	$(CPP) -c gameobject_walker5.cpp -o gameobject_walker5.o $(CXXFLAGS)

intro.o: intro.cpp
	$(CPP) -c intro.cpp -o intro.o $(CXXFLAGS)

keys_dialog.o: keys_dialog.cpp
	$(CPP) -c keys_dialog.cpp -o keys_dialog.o $(CXXFLAGS)

map.o: map.cpp
	$(CPP) -c map.cpp -o map.o $(CXXFLAGS)

precomp.o: precomp.cpp
	$(CPP) -c precomp.cpp -o precomp.o $(CXXFLAGS)

program.o: program.cpp
	$(CPP) -c program.cpp -o program.o $(CXXFLAGS)

re_button.o: re_button.cpp
	$(CPP) -c re_button.cpp -o re_button.o $(CXXFLAGS)

re_events.o: re_events.cpp
	$(CPP) -c re_events.cpp -o re_events.o $(CXXFLAGS)

re_font.o: re_font.cpp
	$(CPP) -c re_font.cpp -o re_font.o $(CXXFLAGS)

re_gamecontext.o: re_gamecontext.cpp
	$(CPP) -c re_gamecontext.cpp -o re_gamecontext.o $(CXXFLAGS)

re_helpers.o: re_helpers.cpp
	$(CPP) -c re_helpers.cpp -o re_helpers.o $(CXXFLAGS)

re_inputbox.o: re_inputbox.cpp
	$(CPP) -c re_inputbox.cpp -o re_inputbox.o $(CXXFLAGS)

re_levelmanager.o: re_levelmanager.cpp
	$(CPP) -c re_levelmanager.cpp -o re_levelmanager.o $(CXXFLAGS)

re_listbox.o: re_listbox.cpp
	$(CPP) -c re_listbox.cpp -o re_listbox.o $(CXXFLAGS)

re_optionparser.o: re_optionparser.cpp
	$(CPP) -c re_optionparser.cpp -o re_optionparser.o $(CXXFLAGS)

re_point.o: re_point.cpp
	$(CPP) -c re_point.cpp -o re_point.o $(CXXFLAGS)

re_rect.o: re_rect.cpp
	$(CPP) -c re_rect.cpp -o re_rect.o $(CXXFLAGS)

re_resource.o: re_resource.cpp
	$(CPP) -c re_resource.cpp -o re_resource.o $(CXXFLAGS)

re_resourcemanager.o: re_resourcemanager.cpp
	$(CPP) -c re_resourcemanager.cpp -o re_resourcemanager.o $(CXXFLAGS)

re_scores.o: re_scores.cpp
	$(CPP) -c re_scores.cpp -o re_scores.o $(CXXFLAGS)

re_soundbuffer.o: re_soundbuffer.cpp
	$(CPP) -c re_soundbuffer.cpp -o re_soundbuffer.o $(CXXFLAGS)

re_soundsession.o: re_soundsession.cpp
	$(CPP) -c re_soundsession.cpp -o re_soundsession.o $(CXXFLAGS)

re_sprite.o: re_sprite.cpp
	$(CPP) -c re_sprite.cpp -o re_sprite.o $(CXXFLAGS)

re_tmxconverter.o: re_tmxconverter.cpp
	$(CPP) -c re_tmxconverter.cpp -o re_tmxconverter.o $(CXXFLAGS)

score_dialog.o: score_dialog.cpp
	$(CPP) -c score_dialog.cpp -o score_dialog.o $(CXXFLAGS)

tilemap.o: tilemap.cpp
	$(CPP) -c tilemap.cpp -o tilemap.o $(CXXFLAGS)

tinyxml2.o: tinyxml2.cpp
	$(CPP) -c tinyxml2.cpp -o tinyxml2.o $(CXXFLAGS)

welcome.o: welcome.cpp
	$(CPP) -c welcome.cpp -o welcome.o $(CXXFLAGS)

welcome_dialog.o: welcome_dialog.cpp
	$(CPP) -c welcome_dialog.cpp -o welcome_dialog.o $(CXXFLAGS)

world.o: world.cpp
	$(CPP) -c world.cpp -o world.o $(CXXFLAGS)


