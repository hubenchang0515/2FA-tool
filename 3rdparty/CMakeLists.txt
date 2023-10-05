aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/quirc/lib QUIRC_SRC)
add_library(quirc STATIC ${QUIRC_SRC})
target_include_directories(quirc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/quirc/lib)
