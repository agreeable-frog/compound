cmake_minimum_required(VERSION 3.7)

project(compound VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message( STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

message(STATUS "${PROJECT_NAME} Build type is : ${CMAKE_BUILD_TYPE}")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/stb)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(log4cplus REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cc)
add_library(bnahabra::renderer ALIAS ${PROJECT_NAME})

add_executable(test-${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cc)

target_compile_definitions(test-${PROJECT_NAME} PRIVATE SHADERS_PATH="${SHADERS_PATH}/")
target_compile_definitions(test-${PROJECT_NAME} PRIVATE LOG4CPLUS_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/test/log4cplus.properties")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

if(TARGET glm::glm)
set(${GLM_LIB} glm::glm)
else()
message(STATUS "Did not find imported target for glm")
set(${GLM_LIB} glm)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${GLM_LIB} Imgui::Imgui PRIVATE log4cplus::log4cplus stb::stb glfw ${CMAKE_DL_LIBS} ${OPENGL_LIBRARIES} GLEW::GLEW)
target_link_libraries(test-${PROJECT_NAME} PRIVATE log4cplus::log4cplus ${PROJECT_NAME})