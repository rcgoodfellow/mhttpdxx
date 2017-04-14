## ----------------------------------------------------------------------------
###
#### libmicrohttpdxx - build file
###
##-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)
project(mhttpdxx)

set( CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} \
  -std=c11 \
  -Wall -Wextra" )

set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} \
  -g -O0 -fno-optimize-sibling-calls -fno-omit-frame-pointer" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
  -g -std=c++14 \
  -Wall -Wextra \
  -fdiagnostics-color=always" )

set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" )

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
  -g -O0 -fno-optimize-sibling-calls -fno-omit-frame-pointer" )

include_directories( "${mhttpdxx_SOURCE_DIR}/lib" )


install(TARGETS ARCHIVE DESTINATION lib)
install(DIRECTORY ./lib/ DESTINATION include/mhttpdxx 
  FILES_MATCHING PATTERN "*.hxx"
  PATTERN "build" EXCLUDE)

add_subdirectory(lib)
add_subdirectory(examples)

