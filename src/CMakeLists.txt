# source files for the library
set(SOURCES
    SRIMData.cc
    SRIMtable.cc
)

# header files
set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/SRIMData.h
    ${PROJECT_SOURCE_DIR}/include/SRIMtable.h
)

file(GLOB SRIM_SOURCES ${PROJECT_SOURCE_DIR}/db/SRIM/*.cc)

# library setting
add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES} ${SRIM_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
      $<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${ROOT_LIBRARIES})

set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES
        PUBLIC_HEADER "${HEADERS}"
)

install(TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT SRIMlibTargets
    INCLUDES DESTINATION include
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/${CMAKE_PROJECT_NAME}
    LIBRARY DESTINATION lib
)

# add excecutable
function(SRIMbin bin_name)
    add_executable(${bin_name} ${bin_name}.cc)
    target_include_directories(${bin_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_link_libraries(${bin_name} ${CMAKE_PROJECT_NAME})

    install(TARGETS ${bin_name}
        RUNTIME DESTINATION bin
    )
endfunction()

SRIMbin(allinfo)
SRIMbin(eloss)
SRIMbin(newe)
SRIMbin(olde)
SRIMbin(range)
SRIMbin(straggling)
