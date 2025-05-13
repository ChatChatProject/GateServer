cmake_minimum_required(VERSION 3.22)
project(GateServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type specified, defaulting to Debug.")
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-inline -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(mysql-concpp_DIR "/mysql-connector-c++-9.2.0-linux-glibc2.28-x86-64bit")

find_package(mysql-concpp REQUIRED CONFIG)
if(mysql-concpp_FOUND)
        message(STATUS "Found mysql-concpp: ${mysql-concpp_VERSION}")
endif()
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(jsoncpp REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

set(proto_file "${CMAKE_CURRENT_SOURCE_DIR}/src/message.proto")
set(proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/message.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/message.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/message.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/src/message.grpc.pb.h")
add_custom_command(
  OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
  COMMAND /usr/bin/protoc
    --proto_path="${CMAKE_CURRENT_SOURCE_DIR}/src"
    --grpc_out="${CMAKE_CURRENT_SOURCE_DIR}/src"
    --cpp_out="${CMAKE_CURRENT_SOURCE_DIR}/src"
    --plugin=protoc-gen-grpc="/usr/bin/grpc_cpp_plugin"
    "message.proto"
  DEPENDS "${proto_file}"
  COMMENT "start generate grpc and pb file"
)
add_library(grpc_shared_lib SHARED ${proto_srcs} ${grpc_srcs})

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_MODE=1)
endif()


target_include_directories(${PROJECT_NAME} PRIVATE
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
  grpc_shared_lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  INSTALL_RPATH "${mysql-concpp_DIR}/lib64"
)

install(TARGETS grpc_shared_lib
  DESTINATION /lib
)

install(TARGETS ${PROJECT_NAME}
  DESTINATION /bin
)

install(DIRECTORY ${mysql-concpp_DIR}/lib64/plugin
  DESTINATION /usr/local/mysql/lib
)

set(CMAKE_INSTALL_PREFIX "")

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
  DESTINATION / 
)
