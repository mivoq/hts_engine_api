######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : June 2012                                                              ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Main CMakeList                                                                   ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


#------------------------------------------------------------------------------------#
#                                Project name                                        #
#------------------------------------------------------------------------------------#

message(STATUS "cmake for project \"hts_engine\"")
project(hts_engine C)


#------------------------------------------------------------------------------------#
#                          CMake minimum version                                     #
#------------------------------------------------------------------------------------#

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


#------------------------------------------------------------------------------------#
#                             Get all CMake modules                                  #
#------------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


#------------------------------------------------------------------------------------#
#                        Top level system checks and options                         #
#------------------------------------------------------------------------------------#

# Operating system checks
include(os)

# System tests/checks
include(system)


#------------------------------------------------------------------------------------#
#                               Include directories                                  #
#------------------------------------------------------------------------------------#

include_directories(include)
include_directories(lib)


#------------------------------------------------------------------------------------#
#                             Library info defintions                                #
#------------------------------------------------------------------------------------#

add_definitions(-DPACKAGE_NAME="hts_engine_API")
add_definitions(-DPACKAGE_TARNAME="hts_engine_api")
add_definitions(-DPACKAGE_VERSION="1.05")
add_definitions(-DPACKAGE_STRING="hts_engine_API 1.05")
add_definitions(-DPACKAGE_BUGREPORT="hts-engine-users@lists.sourceforge.net")
add_definitions(-DAUDIO_PLAY_NONE=1)


#------------------------------------------------------------------------------------#
#                              hts_engine library (static)                           #
#------------------------------------------------------------------------------------#

add_library(HTSEngine STATIC
  lib/HTS_audio.c
  lib/HTS_engine.c
  lib/HTS_gstream.c
  lib/HTS_label.c
  lib/HTS_misc.c
  lib/HTS_model.c
  lib/HTS_pstream.c
  lib/HTS_sstream.c
  lib/HTS_vocoder.c
  lib/HTS_speect.c
)
  
set_target_properties(HTSEngine
  PROPERTIES
  COMPILE_FLAGS "${CMAKE_C_FLAGS} -fPIC")


#------------------------------------------------------------------------------------#
#                          hts_engine,  hts_engine_me binaries                       #
#------------------------------------------------------------------------------------#

add_executable(hts_engine bin/hts_engine.c)
add_executable(hts_engine_me bin/hts_engine_me.c)


if(MSVC)
	target_link_libraries(hts_engine HTSEngine)
	target_link_libraries(hts_engine_me HTSEngine)
else(MSVC)
	target_link_libraries(hts_engine HTSEngine m)
	target_link_libraries(hts_engine_me HTSEngine m)
endif(MSVC)

  