cmake_minimum_required(VERSION 3.10)
project(cavoke_game)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_ALL OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES ON CACHE BOOL "cavoke games" FORCE)
add_subdirectory(cavoke)

set(CAVOKE_H_DIR cavoke/cavoke-dev-lib CACHE PATH "Directory with cavoke.h library" FORCE)
set(CAVOKE_DEV_SERVER_H_DIR cavoke/cavoke-dev-server-lib CACHE PATH "Directory with game dev server library" FORCE)
include_directories(${CAVOKE_H_DIR})
include_directories(${CAVOKE_DEV_SERVER_H_DIR})

add_executable(my_cavoke_game
        my_cavoke_game.cpp
        ${CAVOKE_H_DIR}/cavoke.cpp)

add_executable(my_cavoke_game_server
        my_cavoke_game.cpp
        ${CAVOKE_DEV_SERVER_H_DIR}/cavoke.cpp
        ${CAVOKE_DEV_SERVER_H_DIR}/controllers/logic_controller.cpp)

if (USE_EXTERNAL_NLOHMANN)
    target_link_libraries(my_cavoke_game PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(my_cavoke_game_server PRIVATE nlohmann_json::nlohmann_json)
else ()
    target_link_libraries(my_cavoke_game PRIVATE nlohmann_json)
    target_link_libraries(my_cavoke_game_server PRIVATE nlohmann_json)
endif ()

# Link Drogon
if (USE_EXTERNAL_DROGON)
    target_link_libraries(my_cavoke_game_server PRIVATE Drogon::Drogon)
else ()
    target_link_libraries(my_cavoke_game_server PRIVATE drogon)
endif ()

# Link Boost
find_package(Boost 1.71 REQUIRED filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(my_cavoke_game_server PRIVATE ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
