set(INIT_SOURCES
    update.cc
    Updatedb.cc
)

# update excecutable
add_executable(update ${INIT_SOURCES})

target_include_directories(update
    PRIVATE ${PROJECT_SOURCE_DIR}/init)

target_link_libraries(update ${ROOT_LIBRARIES})

install(TARGETS update
    RUNTIME DESTINATION bin
)