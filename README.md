# SMAUG_public
SMAUG reference code (public release)


# Build

To build SMAUG library and executables, there are the following prerequisites:

- [CMake](https://cmake.org/)
- [OpenSSL](https://www.openssl.org/)
- [googleTest](https://google.github.io/googletest/)

Using CMake, you can build libraries and executables in the `reference_inplementation` directory.

```bash
$ cmake -S . -B build
$ cmake --build build
```

# Run

If the build was successful, result files will be generated in the `build` directory. To run each test, execute as follows

```bash
# In the build/bin directory
$ ./smaug1-main
$ ./smaug1-benchmark
$ ./smaug1-kat-kem-main
```

If you want to run quick tests for each unit of the SMAUG, call

```bash
# In the build directory
$ ctest
```