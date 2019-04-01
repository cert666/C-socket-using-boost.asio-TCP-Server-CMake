string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )

add_executable(server_exe main.cpp)

setup_application( server_exe )

if( MSVC )
 if(CMAKE_BUILD_TYPE STREQUAL debug)
   set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMTD.lib\" )
 else()
   set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMT.lib\" )
 endif()
endif()
