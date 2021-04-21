subdirs(rtos)

add_library(stacksize
	functions.cpp
	stack_usage.cpp)

target_include_directories(stacksize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(stacksize PRIVATE dw elf)
