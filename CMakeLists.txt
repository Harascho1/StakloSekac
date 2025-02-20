cmake_minimum_required(VERSION 3.16)

# Naziv projekta
project(StakloRezac)

# Postavljanje standarda jezika
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 17)


# Dodavanje direktorijuma za linkovanje (biblioteke)
link_directories(
    ${PROJECT_SOURCE_DIR}/dependencies/lib
)

message(STATUS "Library path: ${PROJECT_SOURCE_DIR}/dependencies/lib")

# Rekurzivno sakupljanje izvorišnih fajlova iz src direktorijuma
file(GLOB_RECURSE SOURCES_GUI 
    ${PROJECT_SOURCE_DIR}/Renderer/source/*.cpp
    ${PROJECT_SOURCE_DIR}/Renderer/Shaders/source/*.cpp
    ${PROJECT_SOURCE_DIR}/Renderer/stb/stb_image.cpp
    ${PROJECT_SOURCE_DIR}/dependencies/src/glad.c
    ${PROJECT_SOURCE_DIR}/Renderer/main.cpp
)

# Postavi izlazni direktorijum za izvršni fajl
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Program)


# Kreiranje statičke biblioteke za dataPipe.c
add_library(DataPipe STATIC ${PROJECT_SOURCE_DIR}/pch/dataPipe.c)
target_include_directories(DataPipe PUBLIC ${PROJECT_SOURCE_DIR}/pch)
target_precompile_headers(DataPipe PUBLIC ${PROJECT_SOURCE_DIR}/pch/pchDataPipe.h)

# Kreiranje izvrsne datoteke Test
add_executable(StakloRezac_GUI ${SOURCES_GUI})
add_executable(StakloRezac_Test ${PROJECT_SOURCE_DIR}/Test/main.c)
add_executable(StakloRezac_Logic ${PROJECT_SOURCE_DIR}/Logic/main.cpp)
add_executable(StakloRezac ${PROJECT_SOURCE_DIR}/main.c)

# Dodavanje include direktorijuma
target_include_directories(StakloRezac_GUI PRIVATE
    ${PROJECT_SOURCE_DIR}/dependencies/include
    ${PROJECT_SOURCE_DIR}/dependencies/freetype
    ${PROJECT_SOURCE_DIR}/Renderer/glm
    ${PROJECT_SOURCE_DIR}/Renderer/glm/gtc
    ${PROJECT_SOURCE_DIR}/Renderer/include
    ${PROJECT_SOURCE_DIR}/Renderer/Shaders/include
    ${PROJECT_SOURCE_DIR}/Renderer/stb
)

target_include_directories(StakloRezac_Logic PRIVATE
    ${PROJECT_SOURCE_DIR}/Logic
)

target_include_directories(StakloRezac_Test PRIVATE
)

target_precompile_headers(StakloRezac_GUI PRIVATE 
    ${PROJECT_SOURCE_DIR}/pch/pch.h 
    ${PROJECT_SOURCE_DIR}/pch/pchDataPipe.h
)
target_precompile_headers(StakloRezac_Logic PRIVATE ${PROJECT_SOURCE_DIR}/pch/pchDataPipe.h)
target_precompile_headers(StakloRezac_Test PRIVATE ${PROJECT_SOURCE_DIR}/pch/pchDataPipe.h)

# Linkovanje sa GLFW i GLAD
target_link_libraries(StakloRezac_GUI glfw3 opengl32 freetype DataPipe)
target_link_libraries(StakloRezac_Test DataPipe)
target_link_libraries(StakloRezac_Logic DataPipe)

message(STATUS "DataPipe added to Test, Logic, and GUI targets.")

# Kopira glfw3.dll u direktorijum sa izvršnim fajlom
add_custom_command(TARGET StakloRezac_GUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/dependencies/glfw3.dll
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glfw3.dll)
add_custom_command(TARGET StakloRezac_GUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/dependencies/freetype.dll
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freetype.dll)

# Aktiviranje PCH

set_source_files_properties(${PROJECT_SOURCE_DIR}/dependencies/src/glad.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
