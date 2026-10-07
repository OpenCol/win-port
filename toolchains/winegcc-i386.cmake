# Winelib, 32-bit x86: native Linux code against Wine's user32/gdi32 (milestone W8).
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i386)
set(CMAKE_C_COMPILER winegcc)
set(CMAKE_CXX_COMPILER wineg++)
set(CMAKE_C_FLAGS_INIT "-m32")
set(CMAKE_CXX_FLAGS_INIT "-m32")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-m32")
