add_library(rtos
	riot.cpp)

target_include_directories(rtos PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rtos PRIVATE dw elf)
