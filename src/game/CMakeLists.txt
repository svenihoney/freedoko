add_subdirectory(gameplay_actions)
set(LOCAL_SOURCES
  game.announcement.cpp
  game.cpp
  game.gameplay.cpp
  game.info.cpp
  game.marriage.cpp
  game.player.cpp
  game.swines.cpp
  game.team.cpp
  game.trick.cpp
  reservation.cpp
  specialpoint.cpp
  game_summary.cpp
  gameplay.cpp
  gameplay_action.cpp
)
toglobalsources(${LOCAL_SOURCES})
