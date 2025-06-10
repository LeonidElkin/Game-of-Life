add_executable(game_of_life_app game_of_life_main.cpp)
target_link_libraries(game_of_life_app PRIVATE SDL2::SDL2 SDL2::SDL2main cxxopts magic_enum::magic_enum game_of_life_lib)