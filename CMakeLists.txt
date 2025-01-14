cmake_minimum_required(VERSION 3.20)
project(DiscordBotProject)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/lib/boost_1_87_0)
include_directories(${CMAKE_SOURCE_DIR}/lib/json/single_include)
include_directories(${CMAKE_SOURCE_DIR}/lib/dpp/include)

link_directories(${CMAKE_SOURCE_DIR}/lib/dpp/build/library)


file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_link_libraries(${PROJECT_NAME} dpp)