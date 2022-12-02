
add_executable(main main.c)
add_library(trains main.c)
add_library(stations main.c)
add_library(graph main.c)
add_library(UI main.c)
target_link_libraries(main stations)
