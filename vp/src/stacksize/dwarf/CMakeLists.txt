add_library(dwarf elf_file.cpp)

target_include_directories(dwarf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dwarf PRIVATE dw)
