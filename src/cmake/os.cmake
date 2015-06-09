######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : 9 February 2012                                                        ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## operating system tests                                                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################
 
# this is just to cover our needs for now.

#------------------------------------------------------------------------------------#
#                                  ANDROID                                           #
#------------------------------------------------------------------------------------#

if(ANDROID)
  # from android.toolchain.cmake
  # The flags will be prepopulated with critical flags, so don't loose them.
  # Also be aware that toolchain also sets configuration-specific compiler
  # flags and linker flags.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${my_cxx_flags}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ${my_cxx_flags}")

  
endif(ANDROID)