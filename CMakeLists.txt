cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
${CMAKE_CURRENT_LIST_DIR}/./cmake
)

project(game
  VERSION 0.1.0
)

find_package(gf REQUIRED)
find_package(Box2D)

add_executable(game
  code/game.cc
  code/local/Square.cc
  code/local/Singletons.cc
)

target_include_directories(game 
	PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/code
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(game gf::gf0
	${BOX2D_LIBRARIES}

)



install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/Image"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
