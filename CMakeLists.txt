# win-port: a native Win32 Colonization. The roadmap's section 1 fixes the decisions this
# file encodes; the toolchain files in toolchains/ pick the compiler.
#
#   cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchains/mingw-i686.cmake && cmake --build build
#
# W0 builds only the host self-test. The game's sources (src/game, src/gfx, src/platform)
# join the build at W1 (compile) and W2 (link), as their gates are met.
cmake_minimum_required(VERSION 3.16)
project(win_port C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 98)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4 AND NOT WIN_PORT_ALLOW_64BIT)
  message(FATAL_ERROR "the first architecture is 32-bit x86 (roadmap section 1); 64-bit is W9. "
                      "Pass -DWIN_PORT_ALLOW_64BIT=ON to configure anyway.")
endif()

add_executable(port_selftest src/host/selftest.c)
target_include_directories(port_selftest PRIVATE include)
if(MSVC)
  target_compile_options(port_selftest PRIVATE /W4)
else()
  target_compile_options(port_selftest PRIVATE -Wall -Wextra)
endif()
