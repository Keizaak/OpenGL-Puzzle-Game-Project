# +------------------------------------------------------------------+
# |  Projects Settings                                               |
# +------------------------------------------------------------------+
cmake_minimum_required (VERSION 2.8)
project(GLITTER)

# +------------------------------------------------------------------+
# |  Test system endian type                                         |
# +------------------------------------------------------------------+
include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
  add_definitions(-DIS_BIG_ENDIAN)
else()
  add_definitions(-DIS_LITTLE_ENDIAN)
endif()

# +------------------------------------------------------------------+
# |  Compilation flags                                               |
# +------------------------------------------------------------------+
set (CMAKE_CXX_STANDARD 11)
add_definitions(-Wall -Wextra)
add_definitions(-DRESOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

# +------------------------------------------------------------------+
# |  Load libraries                                                  |
# +------------------------------------------------------------------+
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(external)

# +------------------------------------------------------------------+
# |  Internal API library                                            |
# +------------------------------------------------------------------+

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(UTILS_SRC src/glApi.hpp
              src/glApi.cpp
              src/Application.hpp
              src/Application.cpp
              src/ObjLoader.hpp
              src/ObjLoader.cpp
              src/Image.hpp
              src/SimpleMaterial.hpp
              src/utils.hpp
              src/utils.cpp
              src/Serialize.hpp
              src/Serialize.cpp
              src/AttributeProperties.hpp game/application/GameApplication.cpp game/application/GameApplication.hpp)
add_library(utils ${UTILS_SRC})

# +------------------------------------------------------------------+
# |  glitter executable                                              |
# +------------------------------------------------------------------+

file(GLOB GLSLFiles shaders/*.glsl)

add_executable(puzzle
  ${GLSLFiles}
  game/main.cpp
  game/keyboard/command.cpp
  game/keyboard/command.h
  game/application/GameApplication.cpp
  game/application/GameApplication.hpp
  game/piece/Piece.cpp
  game/piece/Shape.cpp
  game/piece/Shape.hpp)
target_link_libraries(puzzle utils ${GLFW3_LIBRARIES} ${GLEW_LIBRARIES})

# +------------------------------------------------------------------+
# |  Doxygen Generation                                              |
# +------------------------------------------------------------------+

option(GLITTER_BUILD_DOCUMENTATION "Build the Doxygen documentation" ON)
if (GLITTER_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if (GLITTER_BUILD_DOCUMENTATION AND DOXYGEN_FOUND)
    configure_file(${GLITTER_SOURCE_DIR}/docs/Doxyfile.in
      ${GLITTER_BINARY_DIR}/docs/Doxyfile @ONLY)
    add_custom_target( doc_doxygen
      COMMAND ${DOXYGEN_EXECUTABLE} ${GLITTER_BINARY_DIR}/docs/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  endif()
endif()
