file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cc include/*.h)
add_library(message SHARED ${source})
target_include_directories(message PUBLIC "include/message")

install(TARGETS message
  LIBRARY DESTINATION lib
)
