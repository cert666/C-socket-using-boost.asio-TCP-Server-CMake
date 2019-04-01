cmake_minimum_required(VERSION 2.8)

string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )

project(web_socket CXX)

option(PRECOMILE_HEADERS "PRECOMILE_HEADERS" OFF )
option(ENABLE_BUILD_EXAMPLES, "Build example applications" OFF)


# Set C++ language version
set(CMAKE_CXX_STANDARD 11)

SET(64BIT_SUPPORT OFF CACHE BOOL "64BIT_Support Option")

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR     lib     CACHE PATH "Installation directory for libraries"   )
set(INSTALL_BIN_DIR     bin     CACHE PATH "Installation directory for executables" )
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake )
else()
  set(DEF_INSTALL_CMAKE_DIR lib/web_socket)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")


# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

if( MSVC )
 SET( STRICT_CXX_FLAGS "" )

 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} /W4 /wd4127" )
else()
# the definition of strict CXX flags for the GCC
 SET( STRICT_CXX_FLAGS "" )

 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -std=c++11")
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -pthread")
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Werror" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Werror" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wall" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wextra" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Woverloaded-virtual" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wuninitialized" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wmissing-declarations" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wdiv-by-zero" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wendif-labels" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Winline" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Winvalid-pch" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wlogical-op" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wmissing-include-dirs" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wpacked" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wpointer-arith" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wswitch-default" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wwrite-strings" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wundef" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wold-style-cast" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -Wno-inline" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -pedantic" )
 SET( STRICT_CXX_FLAGS "${STRICT_CXX_FLAGS} -pedantic-errors" )
endif()

if( NOT (WIN32 AND NOT CYGWIN ) )
   SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC" )
endif()


# stores additional scripts and cmake-modules
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" )


# glog macros
add_definitions(-DHAVE_SNPRINTF)

include_directories(SYSTEM "glog")
include_directories(SYSTEM ${GLOG_INCLUDEDIR})

# glog macros end

if(WIN32 AND NOT CYGWIN)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif()


# ####
#
# Boost configuration
#

set(Boost_USE_STATIC_LIBS On)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_DEBUG_RUNTIME OFF)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost ${BOOST_VERSION})

include_directories( SYSTEM
  ${Boost_INCLUDE_DIRS})

include_directories(
  "lib/include"
  "${CMAKE_CURRENT_BINARY_DIR}/lib/include")


link_directories(${Boost_LIBRARY_DIRS})

# a custom function which is used by all targets
function( setup_boost target )

   set(Boost_USE_STATIC_LIBS ON )
   set(Boost_USE_MULTITHREADED ON)
   set(Boost_USE_STATIC_RUNTIME ON )

   if(CMAKE_BUILD_TYPE STREQUAL debug)
      set(Boost_USE_DEBUG_RUNTIME ON )
   else()
      set(Boost_USE_DEBUG_RUNTIME OFF )
   endif()

   find_package(Boost ${BOOST_VERSION}  REQUIRED
      COMPONENTS
        date_time
        system
        thread
        container
        atomic
        log
        filesystem
        chrono
        regex
        random )
   if(Boost_FOUND)

      if(CMAKE_BUILD_TYPE STREQUAL debug)
         target_link_libraries( ${target} ${Boost_SYSTEM_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_THREAD_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_CONTAINER_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_ATOMIC_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_DATE_TIME_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_LOG_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_FILESYSTEM_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_CHRONO_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_REGEX_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_RANDOM_LIBRARY_DEBUG} )
         target_link_libraries( ${target} ${Boost_DATE_TIME_LIBRARY_DEBUG} )


      else()
         target_link_libraries( ${target} ${Boost_SYSTEM_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_THREAD_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_CONTAINER_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_ATOMIC_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_DATE_TIME_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_LOG_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_FILESYSTEM_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_CHRONO_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_REGEX_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_RANDOM_LIBRARY_RELEASE} )
         target_link_libraries( ${target} ${Boost_DATE_TIME_LIBRARY_RELEASE} )
      endif()

      if( NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
         target_link_libraries(${target} pthread)
      endif()

    else()
         message( FATAL_ERROR "Failed to find boost" )
   endif()


endfunction()
#
# end of boost configuration
# #####


#GLOG

link_directories(${GLOG_LIBRARYDIR})

function( setup_test target )

  if( NOT WIN32 )
    target_link_libraries( ${target} pthread)
  endif()

  target_link_libraries(${target} web_sock_static )
  target_link_libraries (${target} glog)

  if( WIN32 AND NOT MSVC )
     target_link_libraries( ${target} ws2_32 )
  endif()

     setup_boost( ${target} )

  # set up include-directories


  add_test( ${target} ${target} )

  install( TARGETS ${target}
    EXPORT web_socket_targets
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
   COMPONENT testing )


  if( NOT WIN32 )
    target_link_libraries( ${target} pthread)
  endif()

endfunction()


function( setup_application target )
  
  target_link_libraries(${target} web_sock_static)
  target_link_libraries(${target} glog )

  if( WIN32 AND NOT MSVC )
     target_link_libraries( ${target} ws2_32 )
  endif()

     setup_boost( ${target} )

  install(TARGETS ${target}
    # IMPORTANT: Add the bar executable to the "export-set"
    EXPORT web_socket_targets
    RUNTIME DESTINATION  "${INSTALL_BIN_DIR}"
   COMPONENT bin )

  if( NOT WIN32 )
    target_link_libraries( ${target} pthread)
  endif()


endfunction()



#
#  In case of MSVC ensure that the /MT flag is set!
#  Everything is build runtime-static
##
if( MSVC )
  foreach(flag_var
          CMAKE_CXX_FLAGSC
          CMAKE_CXX_FLAGS_DEBUG
          CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL
          CMAKE_CXX_FLAGS_RELWITHDEBINFO )
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
  add_definitions(/DNOMINMAX)
endif()


# ############################################3
# Add sub-directories
# ##
add_subdirectory(lib)

if(ENABLE_BUILD_EXAMPLES)
    add_subdirectory(examples/server)
    add_subdirectory(examples/client)
endif()

export(PACKAGE web_socket )

# Install the export set for use with the install-tree
install(EXPORT web_socket_targets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

