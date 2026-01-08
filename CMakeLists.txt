cmake_minimum_required(VERSION 4.0)
project(HDC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wodr -Wpacked -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings")
set(CMAKE_CXX_FLAGS_DEBUG "-Ofast")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -DNDEBUG")

add_subdirectory(third_party/matplotplusplus)
add_library(HDC INTERFACE)
target_include_directories(HDC INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)
target_link_libraries(HDC INTERFACE matplot)


add_executable(HDC_executable main.cpp
        txt2data.h)
target_link_libraries(HDC_executable PRIVATE HDC matplot)


