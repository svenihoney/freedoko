add_subdirectory(actor)
add_subdirectory(FreeDoko)
add_subdirectory(gnet)
if(USE_DOKOLOUNGE)
add_subdirectory(DokoLounge)
endif(USE_DOKOLOUNGE)

set(LOCAL_SOURCES
  server.os.cpp
  listener.cpp
  actor.cpp
  server.cpp
  types.cpp
  connection.cpp
  interpreter.cpp
)
toglobalsources(${LOCAL_SOURCES})
