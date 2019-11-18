make_minimum_required(VERSION 3.0)
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
:diffget RE
<<<<<<< HEAD
## coucou
=======
#COUCOU
>>>>>>> 3e49œe727ac12ad5f4b85932e6099694b7a62a647
