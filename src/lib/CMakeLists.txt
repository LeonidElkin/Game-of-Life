add_library(game_of_life_lib Game.cpp utils.cpp)
target_link_libraries(game_of_life_lib PRIVATE SDL2::SDL2 SDL2::SDL2main cxxopts magic_enum::magic_enum)
target_include_directories(game_of_life_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})