cmake_minimum_required(VERSION 3.16)
project(tetriz CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB TETRIZ_SRC
  "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
)

if(NOT TETRIZ_SRC)
  message(FATAL_ERROR "No source files found in games/tetriz")
endif()

add_executable(tetriz ${TETRIZ_SRC})

# find_package(Curses REQUIRED) # Uncomment if ncurses is used
# target_link_libraries(tetriz PRIVATE ${CURSES_LIBRARIES})

install(TARGETS tetriz RUNTIME DESTINATION games/bin)
