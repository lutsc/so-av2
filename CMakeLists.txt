cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

project(so-av1 VERSION 1.0 LANGUAGES C)

file(GLOB SOURCES "src/*.c")

# add_executable(${PROJECT_NAME} ${SOURCES})

add_executable(main_worker 
  src/main_worker.c
  src/arguments.c
  src/pgm.c
)
add_executable(main_sender 
  src/main_sender.c
  src/arguments.c
  src/pgm.c
)

# target_include_directories(${PROJECT_NAME}
#   PRIVATE
#     ${CMAKE_SOURCE_DIR}/include
# )

target_include_directories(main_worker
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(main_sender
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

