# SMAUG_public
SMAUG reference code (public release). 
Visit our [official website] (https://www.kpqc.cryptolab.co.kr/smaug).


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

# Citing

A conference version will be available soon. A pre-print is available as

    | Cryptology ePrint Archive, Report 2023/739, 2023. https://eprint.iacr.org/2023/739

# License

The codes and the specifications are under the MIT license

# Acknowledgements

SMAUG is submitted to the Korean Post-Quantum Cryptography competition. 
