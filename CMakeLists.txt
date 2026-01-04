cmake_minimum_required(VERSION 3.8)
project(rioki_glow CXX)

set(PROJECT_VERSION 0.2.1)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 1)

set(CMAKE_SHARED_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_PREFIX)

find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(HEADERS
  glow/defines.h
  glow/FrameBuffer.h
  glow/fwd.h
  glow/glow.h
  glow/pch.h
  glow/Shader.h
  glow/Texture.h
  glow/util.h
  glow/VertexBuffer.h
)

set(SOURCES
  glow/FrameBuffer.cpp
  glow/pch.cpp
  glow/Shader.cpp
  glow/Texture.cpp
  glow/util.cpp
  glow/VertexBuffer.cpp
)

# glow library
add_library(rioki_glow ${SOURCES})
target_link_libraries(rioki_glow PUBLIC GLEW::GLEW)
target_link_libraries(rioki_glow PUBLIC glm::glm)
set_target_properties(rioki_glow PROPERTIES
    CXX_STANDARD 20
    PUBLIC_HEADER "${HEADERS}"
)

# install
install(
  TARGETS rioki_glow
  EXPORT rioki_glowTargets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/rioki/glow
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/rioki_glowConfigVersion.cmake"
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(${PROJECT_SOURCE_DIR}/rioki_glowConfig.cmake.in
                                rioki_glowConfig.cmake
                                INSTALL_DESTINATION share/rioki_glow)

install(FILES
        "${PROJECT_BINARY_DIR}/rioki_glowConfig.cmake"
        "${PROJECT_BINARY_DIR}/rioki_glowConfigVersion.cmake"
        DESTINATION share/rioki_glow
)

install(EXPORT rioki_glowTargets
        NAMESPACE rioki_glow::
        DESTINATION share/rioki_glow
)
