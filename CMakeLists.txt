cmake_minimum_required(VERSION 3.19)
project(knackpack-rect)


# print current cmake system. On win it can be "Windows-10.0.22621"
message(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")

# need only 3 possible project configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)

# detect platform: windows/linux
set(PLATF "None")
if (WIN32)
    set(PLATF "Windows")
endif()
if (UNIX)
    set(PLATF "Unix")
endif()

# Check we run on known platform
if(PLATF STREQUAL "None")
    message(STATUS "Run on unkonwn platform. Expected: windows or unix")
endif()


set(CMAKE_CXX_STANDARD 14)

# add specific compile falgs only for linux platform
if(PLATF STREQUAL "Unix")
  add_compile_options(-Werror -Wall -Wextra -pedantic -lm)
endif()

if(PLATF STREQUAL "Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

set(SOURCES 
  src/main.cpp 
  src/common.cpp
  src/common.h
  src/palette.cpp
  src/palette.h
  src/rect.cpp 
  src/rect.h
  src/rectset.cpp 
  src/rectset.h
  src/rectplacement.cpp 
  src/rectplacement.h
  src/binplacement.cpp 
  src/binplacement.h
  src/solverslow.cpp
  src/solverslow.h 
  src/solverfast.cpp
  src/solverfast.h
  src/bitmap.cpp
  src/bitmap.h)

add_executable(knackpack-rect ${SOURCES})
