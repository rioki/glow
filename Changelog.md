# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.1] - 2022-10-06

### Fixed

- fixed multi-pass shading for multiple consecutive passes
- fixed compile warnings

## [0.2.0] - 2022-08-08

### Added

- added CMake support
- made Win32 bits not interfere with Linux and Mac builds
- added Pipeline, that models a render pipeline / process
- added Parameters that encapsulate a set of uniform variables
- added support for 2 component and BGR(A) textures
- added support for all OpenGL texture component types

### Changed

- updated VisualStudio projects to VS2022
- changed consumption of GLEW to vcpkg
- move form C-assert to custom sanity check code with debugger tracing in Visual Studio

## [0.1.0] - 2018-03-18

### Added

- added init function that also setups GLEW
- added the Texture wrapper
- added the Shader wrapper
- added the VertexBuffer wrapper
- added the FrameBuffer wrapper
- added GL error utilities clear_errors, get_error

[Unreleased]: https://github.com/rioki/glow/compare/v0.2.1...master
[0.2.1]: https://github.com/rioki/glow/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/rioki/glow/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/rioki/glow/commits/v0.1.0
