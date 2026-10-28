cmake_minimum_required(VERSION 3.16)
project(LaserVaultHeist LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

# ---------------------------------------------------------------- GLFW
# Uses external/glfw if you vendored it, otherwise downloads it at configure time.
if(EXISTS ${CMAKE_SOURCE_DIR}/external/glfw/CMakeLists.txt)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  add_subdirectory(external/glfw)
else()
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(glfw)
endif()

# ---------------------------------------------------------------- GLM
if(EXISTS ${CMAKE_SOURCE_DIR}/external/glm/CMakeLists.txt)
  add_subdirectory(external/glm)
else()
  FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(glm)
endif()
if(NOT TARGET glm::glm)
  add_library(glm::glm ALIAS glm)
endif()

# ---------------------------------------------------------------- GLAD (must be vendored)
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)
  message(FATAL_ERROR
    "GLAD not found. Generate it at https://glad.dav1d.de (Language C/C++, gl 3.3, Profile Core, "
    "Generate a loader ON) and unzip it so that external/glad/include/glad/glad.h, "
    "external/glad/include/KHR/khrplatform.h and external/glad/src/glad.c exist.")
endif()
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

# ---------------------------------------------------------------- Dear ImGui
if(EXISTS ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp)
  set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
else()
  FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.90.9
                       GIT_SHALLOW TRUE SOURCE_SUBDIR does_not_exist)
  FetchContent_MakeAvailable(imgui)
  set(IMGUI_DIR ${imgui_SOURCE_DIR})
endif()
add_library(imgui STATIC
  ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw)

# ---------------------------------------------------------------- Game
file(GLOB GAME_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(LaserVaultHeist ${GAME_SOURCES})
target_compile_definitions(LaserVaultHeist PRIVATE GLFW_INCLUDE_NONE
  SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")
target_link_libraries(LaserVaultHeist PRIVATE glad glfw glm::glm imgui)

# copy shaders next to the executable so it also runs when moved
add_custom_command(TARGET LaserVaultHeist POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:LaserVaultHeist>/shaders)
