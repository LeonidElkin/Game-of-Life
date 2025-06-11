add_executable(gol game_of_life_main.cpp)
target_link_libraries(gol PRIVATE SDL2::SDL2 SDL2::SDL2main game_of_life_lib)