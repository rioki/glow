#  OpenGL Object Wrapper

The OpenGL Object Wrapper implements RAII style resource management for OpenGL
resources.

## Implemented Wrappers

* [Texture](glow/Texture.h)
* [Shader](glow/Shader.h)
* [Vertex Buffer](glow/VertexBuffer.h)
* [Frame Buffer](glow/FrameBuffer.h)

### ToDO

* Uniform Buffer
* Shader Storage Buffer
* Geometry & Compute Shaders

## Dependencies

The glow library is built on top of C++17 and builds on top of 
[The OpenGL Extention Wrangler Library][glew] and [OpenGL Mathematics][glm].

## Building

### Windows

The easyest way to build glow on Windows is with [vcpkg] and [Visual Studio 2019][vs19]. 

First you need to get glew and glm through vcpkg.

	vcpkg install glew:x86-windows glew:x64-windows glm:x86-windows glm:x64-windows

Then you can open glow.sln in Visual Studio 2019 and build the library.

## License

    Copyright 2016-2019 Sean Farrell <sean.farrell@rioki.org>
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
	
[glew]: http://glew.sourceforge.net/
[glm]: https://glm.g-truc.net
[vcpkg]: https://github.com/microsoft/vcpkg
[vs19]: https://visualstudio.microsoft.com/vs/