# red5

Recreation of the classic X-wing game engine.  Status: **SUPER ALPHA**

It reads data from the original game's resources files and optimizes it for running in a custom engine.

File formation information mostly comes from this fantastic repository: [LfdReader](https://github.com/MikeG621/LfdReader)

## Building

The project uses [vcpkg](http://github.com/microsoft/vcpkg) for dependency management.  Install dependencies with the following command:

```
vcpkg install sdl2 spdlog
```

For generating the CMake project be sure to include this value:

```
cmake -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>\scripts\buildsystems\vcpkg.cmake
```

## Other information

Coming soon...
