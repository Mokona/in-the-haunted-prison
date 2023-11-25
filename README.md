## To build for AgonLight

```bash
mkdir build
cd build
cmake -DZCCTARGET=agon -DCMAKE_TOOLCHAIN_FILE=/snap/z88dk/current/share/z88dk/cmake/Toolchain-zcc.cmake ..
make
```

You need to adapt `-DCMAKE_TOOLCHAIN_FILE` to your z88dk installation path.

## About generated files

The generated files are built into the cmake build folder, but they are expected by
the sources to be in the project folder. So you need to call the tools by hand
to generate them in the project folder.

Not practical and error-prone. Waiting for a better solution.
