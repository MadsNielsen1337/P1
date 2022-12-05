
add_executable(main main.c
                    readFiles.c
                    UI.c
                    routes.c
                    trains.c
                    graph.c
                    time_calc.c)

add_library(trains main.c)
add_library(graph main.c)
add_library(UI main.c)
add_library(readFiles main.c)
add_library(routes main.c)
target_link_libraries(main trains graph UI readFiles routes)
