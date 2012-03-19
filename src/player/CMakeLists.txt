add_subdirectory(rating)
add_subdirectory(human)
add_subdirectory(ai)
add_subdirectory(ai.dummy)
add_subdirectory(ai.random)
add_subdirectory(aiconfigs.preset)
add_subdirectory(network)
set(LOCAL_SOURCES
  namegen.cpp
  player.cpp
  playersDb.cpp
  aiconfig.cpp
  heuristicsmap.cpp
)
set_source_files_properties(player/aiconfig.cpp
  PROPERTIES OBJECT_DEPENDS {CMAKE_CURRENT_BINARY_DIR}/aiconfigs.preset/novice )

toglobalsources(${LOCAL_SOURCES})
