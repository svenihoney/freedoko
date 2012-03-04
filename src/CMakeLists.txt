include_directories(SYSTEM
  ${GTKMM_INCLUDE_DIRS} 
  ${GNET_INCLUDE_DIRS}
  ${ALUT_INCLUDE_DIRS} 
)

if(GTKMM_FOUND)
  add_definitions( -DUSE_UI_GTKMM )
endif(GTKMM_FOUND)

if(ALUT_FOUND)
  add_definitions( -DUSE_SOUND -DUSE_SOUND_ALUT )
  add_subdirectory(sound)
  set(SOUND_LIB fd_sound)
endif(ALUT_FOUND)

if(GNET_FOUND)
  add_definitions( -DUSE_NETWORK )
  add_subdirectory(network)
  set(NETWORK_LIB fd_network)
endif(GNET_FOUND)

if(PUBLIC_DATA_DIRECTORY)
  add_definitions( -DPUBLIC_DATA_DIRECTORY_VALUE=${PUBLIC_DATA_DIRECTORY} )
endif(PUBLIC_DATA_DIRECTORY)

if(CMAKE_SYSTEM_NAME MATCHES Linux.*)
  add_definitions( -DLINUX )
endif(CMAKE_SYSTEM_NAME MATCHES Linux.*)

link_directories(
	    ${GTKMM_LIBRARY_DIRS} ${GNET_LIBRARY_DIRS} ${ALUT_LIBRARY_DIRS} )

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/logo.png
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/logo.free.png ${CMAKE_CURRENT_BINARY_DIR}/logo.png
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/icon.png ${CMAKE_CURRENT_BINARY_DIR}/icon.png
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/logo.free.png
)
ADD_CUSTOM_TARGET(copy_logos DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/logo.png)

macro(toglobalsources LOCAL_SOURCES)
  foreach(SRC ${ARGV})
    list(APPEND GLOBAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${SRC})
  endforeach(SRC)
  set(FREEDOKO_SOURCES ${FREEDOKO_SOURCES} ${GLOBAL_SOURCES} PARENT_SCOPE)
endmacro(toglobalsources)

add_subdirectory(card)
add_subdirectory(game)

add_executable(FreeDoko
  basistypes.cpp  
  constants.cpp 
  debug.cpp 
  freedoko.cpp 
  runtime.cpp
  utils.cpp

${FREEDOKO_SOURCES}
class/getopt/getopt.cpp
class/getopt/option.cpp
class/readconfig/readconfig.cpp
misc/bug_report.cpp
misc/references_check.cpp
misc/setting.cardsorder.cpp
misc/setting.cpp
misc/setting.theme.cpp
misc/translation.cpp
misc/translator.cpp
os/auto_bug_report.cpp
os/bug_report.cpp
os/bug_report_replay.cpp
os/chatter.cpp
os/gameplay.cpp
os/os.cpp
os/party_points.cpp
os/seed.cpp
party/party.cpp
party/rule.cpp
player/ai/ai.cpp
player/ai/aiDb.cpp
player/ai/cards_information.cpp
player/ai/cards_information.heuristics.cpp
player/ai/cards_information.of_player.cpp
player/aiconfig.cpp
player/ai.dummy/ai.cpp
player/ai/gametree.cpp
player/ai/heuristics.best_winning.cpp
player/ai/heuristics.cpp
player/ai.random/ai.cpp
player/ai/team_information.cpp
player/ai/team_information.heuristics.cpp
player/ai/trickweighting.cpp
player/ai/weighting.cpp
player/ai/w_monte_carlo.cpp
player/ai/w_virtual_games.cpp
player/heuristicsmap.cpp
player/human/human.cpp
player/namegen.cpp
player/network/network_player.cpp
player/player.cpp
player/playersDb.cpp
player/rating/rating.cpp
ui/gtkmm/about.cpp
ui/gtkmm/aiconfig.cpp
ui/gtkmm/base.cpp
ui/gtkmm/bug_report.cpp
ui/gtkmm/bug_report_replay.cpp
ui/gtkmm/cards.cpp
ui/gtkmm/cardset_license.cpp
ui/gtkmm/card_suggestion.cpp
ui/gtkmm/changelog.cpp
ui/gtkmm/chatter.cpp
ui/gtkmm/error.cpp
ui/gtkmm/first_run.cpp
ui/gtkmm/full_trick.cpp
ui/gtkmm/game_debug.cpp
ui/gtkmm/game_finished.cpp
ui/gtkmm/gameinfo.cpp
ui/gtkmm/gameinfo.announcement.cpp
ui/gtkmm/gameinfo_dialog.cpp
ui/gtkmm/gameinfo.gametype.cpp
ui/gtkmm/gameinfo.genscher.cpp
ui/gtkmm/gameinfo.hyperswines.cpp
ui/gtkmm/gameinfo.marriage.cpp
ui/gtkmm/gameinfo.swines.cpp
ui/gtkmm/game_overview.cpp
ui/gtkmm/gameplay_actions.cpp
ui/gtkmm/game_review.cpp
ui/gtkmm/game_summary.cpp
ui/gtkmm/genscher_selection.cpp
ui/gtkmm/hand.cpp
ui/gtkmm/help.cpp
ui/gtkmm/htin.cpp
ui/gtkmm/icongroup.cpp
ui/gtkmm/icons.cpp
ui/gtkmm/last_trick.cpp
ui/gtkmm/license.cpp
ui/gtkmm/main_window.cpp
ui/gtkmm/menu.cpp
ui/gtkmm/name.cpp
ui/gtkmm/network.cpp
ui/gtkmm/network_log.cpp
ui/gtkmm/party_finished.cpp
ui/gtkmm/party_points.cpp
ui/gtkmm/party_points.graph.cpp
ui/gtkmm/party_settings.cpp
ui/gtkmm/party_summary.cpp
ui/gtkmm/player.cpp
ui/gtkmm/players.cpp
ui/gtkmm/players_db.cpp
ui/gtkmm/poverty.cpp
ui/gtkmm/preferences.cards_order.cpp
ui/gtkmm/preferences.cpp
ui/gtkmm/program_updated.cpp
ui/gtkmm/reservation.cpp
ui/gtkmm/rules.cpp
ui/gtkmm/splash_screen.cpp
ui/gtkmm/support.cpp
ui/gtkmm/table.cpp
ui/gtkmm/table.draw.cpp
ui/gtkmm/table.parts.cpp
ui/gtkmm/thrower.cpp
ui/gtkmm/translations.cpp
ui/gtkmm/trick.cpp
ui/gtkmm/trick_drawing.cpp
ui/gtkmm/trickpile.cpp
ui/gtkmm/trick_summary.cpp
ui/gtkmm/ui.cpp
ui/gtkmm/ui.game.cpp
ui/gtkmm/ui.game.gameplay.cpp
ui/gtkmm/ui.init.cpp
ui/gtkmm/ui.network.cpp
ui/gtkmm/ui.party.cpp
ui/gtkmm/ui.poverty.cpp
ui/gtkmm/ui.trick.cpp
ui/gtkmm/ui.update.cpp
ui/gtkmm/utils.cpp
ui/gtkmm/widgets/filemenu.cpp
ui/gtkmm/widgets/pixbuf_rotations.cpp
ui/gtkmm/widgets/rotations.cpp
ui/gtkmm/widgets/scaled_pixbuf.cpp
ui/gtkmm/widgets/scaled_pixbuf_rotations.cpp
ui/gtkmm/widgets/sticky_dialog.cpp
ui/gtkmm/widgets/stock_button.cpp
ui/help.cpp
ui/text/bug_report.cpp
ui/text/general.cpp
ui/text/help.cpp
ui/text/settings.cpp
ui/text/ui.cpp
ui/text/ui.game.cpp
ui/text/ui.game.gameplay.cpp
ui/text/ui.genscher.cpp
ui/text/ui.init.cpp
ui/text/ui.io.cpp
ui/text/ui.party.cpp
ui/text/ui.poverty.cpp
ui/text/ui.swines.cpp
ui/text/ui.trick.cpp
ui/text/ui.update.cpp
ui/ui.cpp
ui/ui.dummy.cpp
ui/ui.wrap.cpp
utils/date.cpp
utils/file.cpp
utils/stop_watch.cpp
utils/string.cpp
utils/time.cpp
utils/version.cpp
utils/windows.cpp
)

add_dependencies(FreeDoko copy_logos)

target_link_libraries(FreeDoko 
  ${SOUND_LIB} ${NETWORK_LIB}
  ${GTKMM_LIBRARIES} ${GNET_LIBRARIES} ${ALUT_LIBRARIES})

install(TARGETS FreeDoko
        RUNTIME DESTINATION games)
install(FILES icon.png
        DESTINATION ${DATA_INSTALL_DIR})
install(FILES logo.free.png
        DESTINATION ${DATA_INSTALL_DIR}
		RENAME logo.png )

