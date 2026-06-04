cmake_minimum_required(VERSION 3.20)
project(legoland C)

# One executable, built from every translation unit under src/legoland/.
# (The match target; runnability is a non-goal — see cmake/msvc6-toolchain.cmake.)
file(GLOB LEGOLAND_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/legoland/*.c")

add_executable(legoland ${LEGOLAND_SOURCES})
set_target_properties(legoland PROPERTIES SUFFIX ".exe")
