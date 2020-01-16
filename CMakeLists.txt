cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wextra -pedantic ")
set(KGB_DATA "${CMAKE_INSTALL_PREFIX}/share/games/KGB")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
${CMAKE_CURRENT_LIST_DIR}/./cmake
)

project(KGB
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Box2D)

add_executable(KGB
  code/KGB.cc
  code/local/Square.cc
  code/local/ResourceManager.cc
  code/local/Singletons.cc
  #code/local/Map.cc
  code/local/BabyHero.cc
  code/local/Enemy.cc
  #code/local/Debug.cc
  code/local/Physics.cc
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

)

install(
  TARGETS KGB
  RUNTIME DESTINATION games
)


install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/KGB"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
