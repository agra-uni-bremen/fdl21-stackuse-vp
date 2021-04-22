add_library(elf elf_file.cpp)

target_include_directories(elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(elf PRIVATE dw elf)
