set(mysql-concpp_DIR "/mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit" CACHE STRING "change it")
find_package(mysql-concpp REQUIRED CONFIG)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(jsoncpp REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

file(GLOB sources CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
# 	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=1)
# elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
# 	target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_MODE=1)
# endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  include
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${HIREDIS_INCLUDE_DIRS}
  ${mysql-concpp_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost::system
  Boost::filesystem
  JsonCpp::JsonCpp
  protobuf::libprotobuf
  gRPC::grpc++
  ${HIREDIS_LIBRARIES}
  "${mysql-concpp_DIR}/lib64/libmysqlcppconn.so.10"
  message
)

list(APPEND RPATH_LIST "${mysql-concpp_DIR}/lib64" "$ORIGIN/../lib")
string(REPLACE ";" ":" RPATH_STRING "${RPATH_LIST}")

set_target_properties(${PROJECT_NAME} PROPERTIES
  INSTALL_RPATH ${RPATH_STRING}
)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

install(DIRECTORY ${mysql-concpp_DIR}/lib64/plugin
  DESTINATION /usr/local/mysql/lib
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
  DESTINATION /
)
