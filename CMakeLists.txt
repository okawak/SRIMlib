if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "DO NOT BUILD in-tree.")
endif()

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
#set(CMAKE_CXX_STANDARD 17)
project(SRIMlib)

# Default installation dir
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "..." FORCE)
endif()

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ROOT 6 installation
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net Physics Minuit Minuit2 Geom Gui Genetic)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# external link directories
link_directories(${ROOTSYS}/lib)

# subdirectory
add_subdirectory(init)
add_subdirectory(src)

# example code
add_executable(example example.cc)
target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example ${ROOT_LIBRARIES} ${CMAKE_PROJECT_NAME})

install(TARGETS example
    RUNTIME DESTINATION bin
)

# config file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.thisSRIMlib.sh.in"
    "${CMAKE_SOURCE_DIR}/thisSRIMlib.sh"
    @ONLY
)


# cmake configuration
install(EXPORT SRIMlibTargets
    FILE SRIMlibTargets.cmake
    DESTINATION lib/cmake/SRIMlib
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.SRIMlibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SRIMlibConfig.cmake"
    @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SRIMlibConfig.cmake"
    DESTINATION lib/cmake/SRIMlib
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ".SRIMlibConfig.cmake.in"
    "SRIMlibConfig.cmake"
    INSTALL_DESTINATION lib/cmake/SRIMlib
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/SRIMlibConfig.cmake"
    DESTINATION lib/cmake/SRIMlib
)

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)

add_custom_target(copy-compile-commands ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
  COMMENT "Copying compile_commands.json to root directory"
)
