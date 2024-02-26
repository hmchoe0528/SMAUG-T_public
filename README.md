# SMAUG-T_public
SMAUG-T reference code (public release). Visit our [official website](https://www.kpqc.cryptolab.co.kr/smaug-T).


# Build

To build SMAUG-T library and executables, there are the following prerequisites for KAT:

- [OpenSSL](https://www.openssl.org/)

Using Makefile, you can build libraries and executables for each implemenations
The implementations included in each directory are as follows:

1. reference_inplementation (pure C): `SMAUG-T128`, `SMAUG-T192`, `SMAUG-T256` 
2. additional_inplementation (pure C): `TiMER`
3. optimized_inplementation (AVX2): `SMAUG-T128`, `SMAUG-T192`, `SMAUG-T256` 


\
Build in each implementation directory.

```bash
$ make 
$ make kat # It requires OpenSSL
```

# Run

If the build was successful, result files will be generated in the `build` directory. To run each test, execute as follows

```bash
# In the build/bin directory
$ ./smaug-t1-main
$ ./smaug-t1-speed
$ ./PQCgenKAT_smaug-t1
```


# Citing

A conference version will be available soon. A pre-print is available as

    Cryptology ePrint Archive, Report 2023/739, 2023. https://eprint.iacr.org/2023/739

# License

The codes and the specifications are under the MIT license. 

# Acknowledgements

SMAUG-T is submitted to the Korean Post-Quantum Cryptography competition. 
