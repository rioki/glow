cmake_minimum_required(VERSION 3.8)
project(rioki-glow CXX)

set(CMAKE_SHARED_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_PREFIX)

find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(HEADERS_GLOW
  glow/defines.h
  glow/FrameBuffer.h
  glow/fwd.h
  glow/glow.h
  glow/Parameters.h
  glow/pch.h
  glow/Pipeline.h
  glow/Shader.h
  glow/Texture.h
  glow/util.h
  glow/VertexBuffer.h
)

set(SOURCES_GLOW
  glow/FrameBuffer.cpp
  glow/Parameters.cpp
  glow/pch.cpp
  glow/Pipeline.cpp
  glow/Shader.cpp
  glow/Texture.cpp
  glow/util.cpp
  glow/VertexBuffer.cpp
)

# glow library
add_library(rioki-glow ${SOURCES_GLOW})
target_link_libraries(rioki-glow PUBLIC GLEW::GLEW)
target_link_libraries(rioki-glow PUBLIC glm::glm)
set_target_properties(rioki-glow PROPERTIES
    CXX_STANDARD 20
    PUBLIC_HEADER "${HEADERS_GLOW}"
)

# install
target_include_directories(rioki-glow PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glow>
    $<INSTALL_INTERFACE:include/rioki/glow>)

install(TARGETS ${PROJECT_NAME}
    EXPORT litehtmlTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/rioki/glow
)
