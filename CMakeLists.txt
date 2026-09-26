cmake_minimum_required(VERSION 3.20)
project(legoland C)

# One executable, built from every translation unit under src/legoland/.
# (The match target; runnability is a non-goal — see cmake/msvc6-toolchain.cmake.)
file(GLOB LEGOLAND_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/legoland/*.c")

add_executable(legoland ${LEGOLAND_SOURCES})
set_target_properties(legoland PROPERTIES SUFFIX ".exe")

# movie_pool.c was compiled without optimisation in the original (frame pointers,
# 0xCC padding between functions) but with intrinsics: memset is inlined as rep stosd.
set_source_files_properties(src/legoland/movie_pool.c PROPERTIES COMPILE_OPTIONS "/Od;/Oi")
