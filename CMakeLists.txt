cmake_minimum_required(VERSION 3.2.2)


set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.140.tar.gz"
    SHA1 "f2c30348c05d0d424976648ce3560044e007496c"
)

project(explorer)
set(EXPLORER_VERSION_MAJOR 0)
set(EXPLORER_VERSION_MINOR 1)
set(EXPLORER_VERSION_PATCH 0)
set(EXPLORER_VERSION_TWEAK 0)
set(EXPLORER_VERSION
${EXPLORER_VERSION_MAJOR}.${EXPLORER_VERSION_MINOR}.${EXPLORER_VERSION_PATCH}.${EXPLORER_VERSION_TWEAK})
set(EXPLORER_VERSION_STRING "v${EXPLORER_VERSION}")

hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost CONFIG REQUIRED system filesystem)
hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

add_executable(explorer ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} -pthread Catch::Catch Boost::system Boost::filesystem)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES main.cpp DESTINATION bin)

if(BUILD_TESTS)
enable_testing()
file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
target_link_libraries(check Catch::Catch Boost::system Boost::filesystem)
add_test(NAME check COMMAND check "-s" "-r" "compact" "--use-colour" "yes")
endif()

include(CPackConfig.cmake)