string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )

if( NOT MSVC )
  # Please note: we use some c++11 features in the simulation setup!
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  if( ${PRECOMILE_HEADERS} STREQUAL ON )
     include(cotire)
  endif()
else()
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290 /wd4512")
endif()


if( ${64BIT_SUPPORT} STREQUAL ON )
   add_definitions(-DSDL_64BIT_SUPPORT)
endif()

##
# Add a pre-build command to create the build-info
# This file is crated in the build directory!
##

SET ( SOURCES
   src/async_server.cpp
   src/client.cpp
 )


SET( INCLUDES
	include/async_server.hpp
	include/client.hpp
)


add_library( web_sock_shared SHARED ${SOURCES} ${SOURCES_LINUX} ${INCLUDES} )
add_library( web_sock_static STATIC ${SOURCES} ${SOURCES_LINUX} ${INCLUDES} )


set_source_files_properties( ${SOURCES} PROPERTIES COMPILE_FLAGS ${STRICT_CXX_FLAGS})



if(CMAKE_BUILD_TYPE STREQUAL debug)
   setup_boost( web_sock_shared )
else()
   setup_boost( web_sock_shared )
endif()

if( MSVC )

 if(CMAKE_BUILD_TYPE STREQUAL debug)
   set(CMAKE_SHARED_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMTD.lib\" )
 else()
   set(CMAKE_SHARED_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMT.lib\" )
 endif()

else()

endif()

if( MSVC )
   target_link_libraries (web_sock_shared glog)
endif()

if( WIN32 AND NOT MSVC )
   target_link_libraries( web_sock_shared ws2_32 )
endif()

# process headers pre compilation
if( NOT MSVC )
   if( ${PRECOMILE_HEADERS} STREQUAL ON )
      cotire(web_sock_shared)
   endif()
endif()



set_target_properties(web_sock_static PROPERTIES FRAMEWORK               TRUE
                                                     PUBLIC_HEADER           "${INCLUDES};${INFO_FILE}"   )

set_target_properties(web_sock_shared PROPERTIES FRAMEWORK               TRUE
                                                     PUBLIC_HEADER           "${INCLUDES};${INFO_FILE}"   )



install(
  TARGETS web_sock_shared
  # IMPORTANT: Add the web_sock library to the "export-set"
  EXPORT web_socket_targets
  RUNTIME DESTINATION "${INSTALL_LIB_DIR}" COMPONENT libraries
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT libraries
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT libraries
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
  COMPONENT libraries
  )


install(
  TARGETS web_sock_static
  # IMPORTANT: Add the web_sock library to the "export-set"
  EXPORT web_socket_targets
  RUNTIME DESTINATION "${INSTALL_LIB_DIR}" COMPONENT libraries
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT libraries
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT libraries
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
  COMPONENT libraries
  )

