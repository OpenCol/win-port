# Winelib, 64-bit x86 (milestone W9; configure with -DWIN_PORT_ALLOW_64BIT=ON until then).
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER winegcc)
set(CMAKE_CXX_COMPILER wineg++)
