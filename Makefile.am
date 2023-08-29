## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DLOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DSRC_DIR=\""$(srcdir)"\" \
	-DDATA_DIR=\"$(datadir)\"

AM_CFLAGS =\
	 -D_REENTRANT\
	 -D_FILE_OFFSET_BITS=64\
	 -Wall

AM_CPPFLAGS = -I$(srcdir)/.. $(DEPS_CFLAGS) $(EXTRA_CFLAGS)

bin_PROGRAMS = ricky

h_sources = \
	world.h \
	program.h \
	application.h \
	precomp.h \
	tinyxml2.h \
	re_helpers.h \
	re_optionparser.h \
	re_levelmanager.h \
	re_sprite.h \
	re_soundbuffer.h \
	re_soundsession.h \
	re_resource.h\
	re_resourcemanager.h\
	re_gamecontext.h \
	re_events.h \
	re_rect.h \
	re_point.h \
	re_font.h \
	re_tmxconverter.h \
	re_scores.h \
	re_inputbox.h \
	re_listbox.h \
	re_button.h \
	welcome.h \
	keys_dialog.h \
	welcome_dialog.h \
	score_dialog.h \
	intro.h \
	gameobject.h \
	gameobject_factory.h \
	gameobject_moving.h \
	gameobject_player.h \
	gameobject_bomb.h \
	gameobject_bullet.h \
	gameobject_roller.h \
	gameobject_roller1.h \
	gameobject_roller2.h \
	gameobject_roller3.h \
	gameobject_roller4.h \
	gameobject_bonus.h \
	gameobject_bonus1.h \
	gameobject_bonus2.h \
	gameobject_bonus3.h \
	gameobject_bonus4.h \
	gameobject_jewel.h \
	gameobject_climber.h \
	gameobject_climber1.h \
	gameobject_climber2.h \
	gameobject_climber3.h \
	gameobject_walker.h \
	gameobject_walker1.h \
	gameobject_walker2.h \
	gameobject_walker3.h \
	gameobject_walker4.h \
	gameobject_walker5.h \
	gameobject_projectile.h \
	gameobject_projectile1.h \
	gameobject_projectile2.h \
	gameobject_projectile3.h \
	gameobject_vprojectile.h \
	gameobject_vprojectile1.h \
	gameobject_vprojectile2.h \
	gameobject_statickiller.h \
	gameobject_statickiller1.h \
	gameobject_statickiller2.h \
	gameobject_bulletpack.h \
	gameobject_bombpack.h \
	gameobject_blastwall.h \
	gameobject_blastwall1.h \
	gameobject_blastwall2.h \
	gameobject_blastwall3.h \
	gameobject_movingwall.h \
	gameobject_movingwall2.h \
	gameobject_movingwall3.h \
	gameobject_movingwall4.h \
	gameobject_wagon.h \
	gameobject_crane.h \
	gameobject_fireball.h \
	gameobject_fallplatform.h \
	gameobject_fallplatform1.h \
	gameobject_fallplatform2.h \
	gameobject_spikerock.h \
	gameobject_falltrap.h \
	gameobject_traptrigger.h \
	gameobject_traptrigger1.h \
	gameobject_traptrigger2.h \
	gameobject_bat.h \
	gameobject_flame.h \
	gameobject_explosion.h \
	gameobject_jumpplatform.h \
	gameobject_elevator.h \
	gameobject_background.h \
	gameobject_background1.h \
	gameobject_background2.h \
	gameobject_background3.h \
	gameobject_background4.h \
	gameobject_background5.h \
	gameobject_background6.h \
	gameobject_background7.h \
	gameobject_background8.h \
	gameobject_background9.h \
	gameobject_laser.h \
	gameobject_electricity.h \
	gameobject_nokill.h \
	gameobject_saw.h \
	tilemap.h \
	map.h

cpp_sources = \
	world.cpp \
	program.cpp \
	application.cpp \
	precomp.cpp \
	tinyxml2.cpp \
	re_helpers.cpp \
	re_optionparser.cpp \
	re_levelmanager.cpp \
	re_sprite.cpp \
	re_soundbuffer.cpp \
	re_soundsession.cpp \
	re_resource.cpp\
	re_resourcemanager.cpp\
	re_gamecontext.cpp \
	re_events.cpp \
	re_rect.cpp \
	re_point.cpp \
	re_font.cpp \
	re_tmxconverter.cpp \
	re_scores.cpp \
	re_inputbox.cpp \
	re_listbox.cpp \
	re_button.cpp \
	welcome.cpp \
	keys_dialog.cpp \
	welcome_dialog.cpp \
	score_dialog.cpp \
	intro.cpp \
	gameobject.cpp \
	gameobject_factory.cpp \
	gameobject_moving.cpp \
	gameobject_player.cpp \
	gameobject_bomb.cpp \
	gameobject_bullet.cpp \
	gameobject_roller.cpp \
	gameobject_roller1.cpp \
	gameobject_roller2.cpp \
	gameobject_roller3.cpp \
	gameobject_roller4.cpp \
	gameobject_bonus.cpp \
	gameobject_bonus1.cpp \
	gameobject_bonus2.cpp \
	gameobject_bonus3.cpp \
	gameobject_bonus4.cpp \
	gameobject_jewel.cpp \
	gameobject_climber.cpp \
	gameobject_climber1.cpp \
	gameobject_climber2.cpp \
	gameobject_climber3.cpp \
	gameobject_walker.cpp \
	gameobject_walker1.cpp \
	gameobject_walker2.cpp \
	gameobject_walker3.cpp \
	gameobject_walker4.cpp \
	gameobject_walker5.cpp \
	gameobject_projectile.cpp \
	gameobject_projectile1.cpp \
	gameobject_projectile2.cpp \
	gameobject_projectile3.cpp \
	gameobject_vprojectile.cpp \
	gameobject_vprojectile1.cpp \
	gameobject_vprojectile2.cpp \
	gameobject_statickiller.cpp \
	gameobject_statickiller1.cpp \
	gameobject_statickiller2.cpp \
	gameobject_bulletpack.cpp \
	gameobject_bombpack.cpp \
	gameobject_blastwall.cpp \
	gameobject_blastwall1.cpp \
	gameobject_blastwall2.cpp \
	gameobject_blastwall3.cpp \
	gameobject_movingwall.cpp \
	gameobject_movingwall2.cpp \
	gameobject_movingwall3.cpp \
	gameobject_movingwall4.cpp \
	gameobject_wagon.cpp \
	gameobject_crane.cpp \
	gameobject_fireball.cpp \
	gameobject_fallplatform.cpp \
	gameobject_fallplatform1.cpp \
	gameobject_fallplatform2.cpp \
	gameobject_spikerock.cpp \
	gameobject_falltrap.cpp \
	gameobject_traptrigger.cpp \
	gameobject_traptrigger1.cpp \
	gameobject_traptrigger2.cpp \
	gameobject_bat.cpp \
	gameobject_flame.cpp \
	gameobject_explosion.cpp \
	gameobject_jumpplatform.cpp \
	gameobject_elevator.cpp \
	gameobject_background.cpp \
	gameobject_background1.cpp \
	gameobject_background2.cpp \
	gameobject_background3.cpp \
	gameobject_background4.cpp \
	gameobject_background5.cpp \
	gameobject_background6.cpp \
	gameobject_background7.cpp \
	gameobject_background8.cpp \
	gameobject_background9.cpp \
	gameobject_laser.cpp \
	gameobject_electricity.cpp \
	gameobject_nokill.cpp \
	gameobject_saw.cpp \
	tilemap.cpp \
	map.cpp

ricky_SOURCES = $(h_sources) $(cpp_sources)

if HAVE_WINDRES
ricky_RC = $(top_srcdir)/sources/resource.rc
ricky_RCO = resource.rc.o

$(ricky_RCO):
	$(WINDRES) --define TOP_SRCDIR="$(top_srcdir)" \
		$(ricky_RC) -O coff -o $(ricky_RCO)

endif

ricky_CXXFLAGS = $(INCLUDES) $(CPPFLAGS) $(CFLAGS)

ricky_LDFLAGS = $(LIBINTL)

ricky_LDADD = $(DEPS_LIBS) $(EXTRA_LIBS) $(ricky_RCO)

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
