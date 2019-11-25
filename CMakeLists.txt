cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_CXX_STANDARD 14)

project(game
  VERSION 0.1.0
)
find_package(gf REQUIRED)

add_executable(game
  code/game.cc
  code/local/Square.cc
  code/local/Singletons.cc

)

target_link_libraries(game gf::gf0)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/Image"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
