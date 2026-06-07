# MSVC6-via-wibo toolchain for the legoland matching decomp.
# The compiler/linker are the original MSVC6 tools (downloaded by setup.py into
# toolchain/), driven through wibo by path-rewriting wrapper scripts.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86)

get_filename_component(_tc_dir "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(CMAKE_C_COMPILER "${_tc_dir}/wrappers/cl")
set(CMAKE_C_COMPILER_ID_RUN TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)

set(CMAKE_C_OUTPUT_EXTENSION ".obj")
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_INCLUDE_FLAG_C "/I")
set(CMAKE_C_DEFINE_FLAG "/D")
set(CMAKE_DEPFILE_FLAGS_C "")

# Match the original build: /O2. /Z7 embeds CodeView debug in each .obj (so there
# is no shared vc60.pdb to race on across parallel compiles); LINK /DEBUG then
# produces the program PDB that reccmp reads.
set(CMAKE_C_FLAGS_INIT "/nologo /W3")
set(CMAKE_C_FLAGS_DEBUG_INIT "")
set(CMAKE_C_FLAGS_RELEASE_INIT "")

# Fully explicit rules (bypass any compiler-id-specific CMake machinery).
set(CMAKE_C_COMPILE_OBJECT
    "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> /O2 /Z7 /c /Fo<OBJECT> <SOURCE>")

# Runnability is a non-goal: /NODEFAULTLIB (stubs reference nothing external),
# a dummy /ENTRY so the image links, /DEBUG for the PDB reccmp needs.
set(CMAKE_C_LINK_EXECUTABLE
    "${_tc_dir}/wrappers/link /nologo /DEBUG /INCREMENTAL:NO /SUBSYSTEM:WINDOWS <LINK_FLAGS> /OUT:<TARGET> <OBJECTS>")
