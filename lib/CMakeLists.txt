
add_library( mhttpdxx server.cxx )
target_link_libraries( mhttpdxx microhttpd )

install(TARGETS mhttpdxx ARCHIVE DESTINATION lib)
