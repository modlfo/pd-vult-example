# Tested with puredata v0.47
project(pd-vult-example)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)

add_definitions(-DPD)

if(APPLE)
   include_directories(/Applications/Pd-0.47.app/Contents/Resources/src/)
   set(LIB_SUFFIX ".pd_darwin")
   set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup ${CMAKE_SHARED_LINKER_FLAGS}")
   set(LINK_LIBRARIES)
endif(APPLE)

if(UNIX AND NOT APPLE)
   include_directories(/usr/local/include)
   set(LIB_SUFFIX ".pd_linux")
   set(LINK_LIBRARIES m c)
endif(UNIX AND NOT APPLE)

macro(add_pd_object name files)
   add_library(${name} SHARED ${${files}})
   set_target_properties(${name} PROPERTIES
      OUTPUT_NAME "${name}~"
      SUFFIX ${LIB_SUFFIX}
      PREFIX "")
   target_link_libraries(${name} ${LINK_LIBRARIES})
endmacro(add_pd_object)

set(SRC
   vultin.c
   vultin.h
   phd.cpp
   phd.h
   )

add_pd_object(phd SRC)

