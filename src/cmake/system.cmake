######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 February 2012                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## system tests                                                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 

#------------------------------------------------------------------------------------#
#                         Include CMake system checking macros                       #
#------------------------------------------------------------------------------------#

include(CheckIncludeFiles)
include(CheckTypeSize)
include(TestBigEndian)


#------------------------------------------------------------------------------------#
#                         Check for system include files                             #
#------------------------------------------------------------------------------------#


include(ac_header_stdc)

if(HAVE_LIBM)
  add_definitions(-DHAVE_LIBM=1)
endif(HAVE_LIBM)

if(STDC_HEADERS)
  add_definitions(-DSTDC_HEADERS=1)
endif(STDC_HEADERS)

if(HAVE_SYS_TYPES_H)
  add_definitions(-DHAVE_SYS_TYPES_H=1)
endif(HAVE_SYS_TYPES_H)

if(HAVE_SYS_STAT_H)
  add_definitions(-DHAVE_SYS_STAT_H=1)
endif(HAVE_SYS_STAT_H)

if(HAVE_STDLIB_H)
  add_definitions(-DHAVE_STDLIB_H=1)
endif(HAVE_STDLIB_H)

if(HAVE_STRING_H)
  add_definitions(-DHAVE_STRING_H=1)
endif(HAVE_STRING_H)

if(HAVE_STRING_H)
  add_definitions(-DHAVE_MEMORY_H=1)
endif(HAVE_STRING_H)

if(HAVE_STRINGS_H)
  add_definitions(-DHAVE_STRINGS_H=1)
endif(HAVE_STRINGS_H)

if(HAVE_INTTYPES_H)
 add_definitions(-DHAVE_INTTYPES_H=1)
endif(HAVE_INTTYPES_H)

if(HAVE_STDINT_H)
 add_definitions(-DHAVE_STDINT_H=1)
endif(HAVE_STDINT_H)

check_include_files(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
  add_definitions(-DHAVE_UNISTD_H=1)
endif(HAVE_UNISTD_H)


#------------------------------------------------------------------------------------#
#                             Test system endianess                                  #
#------------------------------------------------------------------------------------#
  
# Test endianess
test_big_endian(SPCT_BIG_ENDIAN)
if(NOT SPCT_BIG_ENDIAN)
  set(SPCT_LITTLE_ENDIAN 1)
endif(NOT SPCT_BIG_ENDIAN)


