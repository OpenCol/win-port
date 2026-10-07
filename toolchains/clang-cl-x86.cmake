# clang-cl targeting 32-bit x86 Windows with the MSVC runtime: the "MSVC x86 must also build"
# check from section 1. Needs the Windows SDK and MSVC libraries; set WINSDK_LIB/MSVC_LIB to
# their x86 library directories when cross-compiling (e.g. from xwin), or run on Windows.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86)
set(CMAKE_C_COMPILER clang-cl)
set(CMAKE_CXX_COMPILER clang-cl)
set(CMAKE_LINKER lld-link)
set(CMAKE_C_COMPILER_TARGET i686-pc-windows-msvc)
set(CMAKE_CXX_COMPILER_TARGET i686-pc-windows-msvc)
if(DEFINED ENV{WINSDK_LIB} AND DEFINED ENV{MSVC_LIB})
  set(CMAKE_EXE_LINKER_FLAGS_INIT "/libpath:$ENV{WINSDK_LIB} /libpath:$ENV{MSVC_LIB}")
endif()
