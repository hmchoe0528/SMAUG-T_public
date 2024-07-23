# SMAUG-T_public
SMAUG-T reference code (public release). Visit our [official website](https://www.kpqc.cryptolab.co.kr/smaug-t).


# Build

To build the SMAUG-T library and executables, there are the following prerequisites for KAT:

- [OpenSSL](https://www.openssl.org/)

Using Makefile, you can build libraries and executables for each implementation. 
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

If the build was successful, result files will be generated in the `build` directory. To run each test, execute as follows.

```bash
$ ./smaug-t1-main
$ ./smaug-t1-speed
$ ./PQCgenKAT_smaug-t1
```


# Citing

A conference version for SMAUG is available:

    Cheon, J.H., Choe, H., Hong, D., Yi, M. (2024). SMAUG: Pushing Lattice-Based Key Encapsulation Mechanisms to the Limits. 
    In: Carlet, C., Mandal, K., Rijmen, V. (eds) Selected Areas in Cryptography – SAC 2023. SAC 2023. 
    Lecture Notes in Computer Science, vol 14201. Springer, Cham. https://doi.org/10.1007/978-3-031-53368-6_7

To cite SMAUG-T specifications, possibly with or without versions: 

    Cheon, J.H., Choe, H., Choi J., Hong, D., Hong, J., Jung, C., Kang, H., Lee, J., Lim, S., Park, A., Park, S., Seong, H., Shin, J. (2024). 
    SMAUG-T: Algorithm Specifications and Supporting Documentation v3.0.

Or via BibTeX: 

    @techreport{SMAUG-T-v3.0,
    author = {Cheon, Jung Hee and Choe, Hyeongmin and Choi, Joongeun and Hong, Dongyeon and Hong, Jeongdae and Jung, Chi-Gon and Kang, 
    Honggoo and Lee, Janghyun and Lim, Seonghyuck and Park, Aesun and Park, Seunghwan and Seong, Hyoeun and Shin, Junbum},
    title = {SMAUG-T: Algorithm Specifications and Supporting Documentation v3.0},
    year = {2024},
    }

# License

The codes and the specifications are under the MIT license. 

# Acknowledgements

SMAUG-T is submitted to the Korean Post-Quantum Cryptography competition. 
