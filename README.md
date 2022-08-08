# OpenGL Object Wrapper

The OpenGL Object Wrapper implements RAII style resource management for OpenGL
resources.

## Implemented Wrappers

Glow provides wrappers for the following OpenGL objects:

* [Texture](glow/Texture.h)
* [Shader](glow/Shader.h)
* [Vertex Buffer](glow/VertexBuffer.h)
* [Frame Buffer](glow/FrameBuffer.h)

## Rendering Infrastucture

Glow provides the [Pipeline](glow/Pipeline.h) class that implements
a multi stage rendering algorithm. Specify the render passes with add_pass,
add geometry, lights and set the camera matrices and execute.

To facilitate state the [Parameters](glow/Parameters.h) provide the means
to associate uniform variables to values without a specific shader.

## Dependencies

The glow library is built on top of C++17 and builds on top of 
[The OpenGL Extention Wrangler Library][glew] and [OpenGL Mathematics][glm].

## Building

The primary development of glow is done with [Visual Studio 2022][vs]. 
Dependencies are pulled with [vcpkg], folow the [Get started with vcpkg] guide
to setup your build environment.

CMake is available, but mostly serves are vcpkg integration.

## License

The glow libary is provided under the MIT license, see [LICENSE.txt](LICENSE.txt)
for details.

[glew]: http://glew.sourceforge.net/
[glm]: https://glm.g-truc.net
[vcpkg]: https://github.com/microsoft/vcpkg
[vs]: https://visualstudio.microsoft.com/vs/
[Get started with vcpkg]: https://vcpkg.io/en/getting-started.html