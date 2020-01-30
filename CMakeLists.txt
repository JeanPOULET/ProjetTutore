cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wextra ")
set(KGB_DATA_DIR "/usr/local/share/games/KGB")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
${CMAKE_CURRENT_LIST_DIR}/./cmake
)

project(KGB
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Box2D REQUIRED)
find_package(SFML 2.4.2 COMPONENTS audio REQUIRED)
add_executable(KGB
  code/local/BackgroundMusic.cc
  code/KGB.cc
  code/local/ResourceManager.cc
  code/local/Singletons.cc
  code/local/Map.cc
  code/local/BabyHero.cc
  code/local/Enemy.cc
  code/local/Physics.cc
  code/local/b2dContactListener.cc


)

target_include_directories(KGB 
	PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/code
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(KGB
PUBLIC
	gf::gf0
  ${BOX2D_LIBRARIES}
  ${SFML_LIBRARIES}

)

install(
  TARGETS KGB
  RUNTIME DESTINATION games
)


install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/KGB"
  DESTINATION "/usr/local/share/games"
)
