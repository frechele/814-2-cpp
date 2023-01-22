# Target name
set(target solver)

# Define
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Sources
file(GLOB header_dir
    ${root_dir}/Includes/solver)

file(GLOB_RECURSE headers
    ${header_dir}/*.hpp)

file(GLOB_RECURSE sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(${target} ${headers} ${sources})
target_link_libraries(${target} game)
