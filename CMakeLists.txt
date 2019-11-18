cmake_minimum_required(VERSION 3.0)
project(FOOGAME
  VERSION 0.1.0
)
find_package(gf REQUIRED)
add_executable(foogame
  foogame.cc
  # and the rest of your sources
)
target_link_libraries(foogame
  gf::gf0
)

#COUCOU
