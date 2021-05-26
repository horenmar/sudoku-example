cmake_minimum_required (VERSION 3.5)
project (sudoku-SAT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(deps/minisat)

#find_package(MiniSat 2.2 REQUIRED)

add_executable(sudoku-cli
    main.cpp
    solver.cpp
    # Headers for IDEs
    solver.hpp
)
target_link_libraries(sudoku-cli MiniSat::libminisat)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sudoku-cli)
