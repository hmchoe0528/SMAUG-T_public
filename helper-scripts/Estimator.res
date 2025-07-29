The lattice estimator may run for three cost models: MATZOV, ADPS16, and ADPS16_quantum, but you can also add your own models. 
It may take some time. You can turn off the lattice-estimator by setting run_lwe and run_lwr False.
The DFP computation only works for selected sigmas, e.g., 1.0625 or 1.453713.
SparseCBD with param numCBD = 48, 58, and 68:
 spCBD(48) = {0:4/8, 1:1/4,  -1:1/4} = CBD(1) 
 spCBD(58) = {0:5/8, 1:3/16, -1:3/16} 
 spCBD(68) = {0:6/8, 1:1/8,  -1:1/8}

ADPS16_classical
== TiMER-KS: LWE ==
Algorithm functools.partial(<function dual_hybrid at 0x7fe26fec1bd0>, red_cost_model=<reduction.ADPS16 object at 0x7fe2c7227fd0>, mitm_optimization=True) on LWEParameters(n=512, q=1024, Xs=D(σ=0.52), Xe=D(σ=1.06), m=512, tag='LWE') failed with β = 79 > d = 76
arora-gb             :: rop: ≈2^829.4, m: ≈2^360.2, dreg: 106, t: 4, mem: ≈2^475.0, tag: arora-gb, ↻: ≈2^354.4, ζ: 373, |S|: ≈2^246.9, prop: ≈2^-105.3
bkw                  :: rop: ≈2^144.7, m: ≈2^132.7, mem: ≈2^133.7, b: 13, t1: 0, t2: 13, ℓ: 12, #cod: 420, #top: 1, #test: 91, tag: coded-bkw
usvp                 :: rop: ≈2^120.0, red: ≈2^120.0, δ: 1.003908, β: 411, d: 869, tag: usvp
bdd                  :: rop: ≈2^120.9, red: ≈2^120.0, svp: ≈2^119.7, β: 411, η: 410, d: 884, tag: bdd
bdd_hybrid           :: rop: ≈2^121.3, red: ≈2^120.0, svp: ≈2^120.6, β: 411, η: 413, ζ: 0, |S|: 1, d: 955, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^166.5, red: ≈2^165.5, svp: ≈2^165.4, β: 410, η: 2, ζ: 217, |S|: ≈2^201.1, d: 738, prob: ≈2^-43.6, ↻: ≈2^45.8, tag: hybrid
dual                 :: rop: ≈2^125.9, mem: ≈2^89.4, m: 407, β: 431, d: 919, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^122.7, mem: ≈2^114.2, m: 388, β: 399, d: 860, ↻: 51, ζ: 40, h1: 8, tag: dual_hybrid
== TiMER-KS: LWR ==
Algorithm functools.partial(<function dual_hybrid at 0x7fe26fec1bd0>, red_cost_model=<reduction.ADPS16 object at 0x7fe2c7227fd0>, mitm_optimization=True) on LWEParameters(n=512, q=1024, Xs=D(σ=0.50), Xe=D(σ=1.12, μ=-0.50), m=768, tag='LWR') failed with β = 79 > d = 75
Algorithm <lwe_bkw.CodedBKW object at 0x7fe26fea4550> on LWEParameters(n=512, q=1024, Xs=D(σ=0.50), Xe=D(σ=1.12, μ=-0.50), m=768, tag='LWR') failed with Amplifying for μ≠0 not implemented.
arora-gb             :: rop: ≈2^693.6, dreg: 83, mem: ≈2^553.0, t: 1, m: ≈2^150.2, tag: arora-gb, ↻: ≈2^140.6, ζ: 234, |S|: 1, prop: ≈2^-138.4
usvp                 :: rop: ≈2^119.7, red: ≈2^119.7, δ: 1.003914, β: 410, d: 874, tag: usvp
bdd                  :: rop: ≈2^121.2, red: ≈2^119.7, svp: ≈2^120.6, β: 410, η: 413, d: 843, tag: bdd
bdd_hybrid           :: rop: ≈2^121.2, red: ≈2^119.7, svp: ≈2^120.6, β: 410, η: 413, ζ: 0, |S|: 1, d: 955, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^162.3, red: ≈2^161.4, svp: ≈2^161.3, β: 400, η: 2, ζ: 228, |S|: ≈2^195.4, d: 718, prob: ≈2^-42.4, ↻: ≈2^44.6, tag: hybrid
dual                 :: rop: ≈2^125.9, mem: ≈2^89.0, m: 400, β: 431, d: 912, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^121.9, mem: ≈2^112.8, m: 380, β: 396, d: 851, ↻: 59, ζ: 41, h1: 7, tag: dual_hybrid

==================
==  TiMER-KS
==================
n=256, k=2, q=1024, p=256, p'=8, sigma=1.0625, hs=140, numCBD=68
====================
LWE/R Hardness (in Log_2):
 [Core-SVP] 115.6 (beta= 396.0)
 [ADPS16_classical] 119.7
Meet-LWE (in Log_2):
 [Rep-1] 181.1 with memory 156.5
 [Rep-2] 177.2 with memory 157.4
Decryption Failure Probability (in Log_2): 
 [DFP] -161.05
Sizes: 
 [PKE] pk=672, ctxt=608, sk=128 bytes
 [KEM] pk=672, ctxt=608, sk=832 bytes
========================================

ADPS16_classical
== Smaug128-KS: LWE ==
Algorithm functools.partial(<function dual_hybrid at 0x7fe26fec1bd0>, red_cost_model=<reduction.ADPS16 object at 0x7fe2c7227fd0>, mitm_optimization=True) on LWEParameters(n=512, q=1024, Xs=D(σ=0.52), Xe=D(σ=1.06), m=512, tag='LWE') failed with β = 79 > d = 76
arora-gb             :: rop: ≈2^829.4, m: ≈2^360.2, dreg: 106, t: 4, mem: ≈2^475.0, tag: arora-gb, ↻: ≈2^354.4, ζ: 373, |S|: ≈2^246.9, prop: ≈2^-105.3
bkw                  :: rop: ≈2^144.7, m: ≈2^132.7, mem: ≈2^133.7, b: 13, t1: 0, t2: 13, ℓ: 12, #cod: 420, #top: 1, #test: 91, tag: coded-bkw
usvp                 :: rop: ≈2^120.0, red: ≈2^120.0, δ: 1.003908, β: 411, d: 869, tag: usvp
bdd                  :: rop: ≈2^120.9, red: ≈2^120.0, svp: ≈2^119.7, β: 411, η: 410, d: 884, tag: bdd
bdd_hybrid           :: rop: ≈2^121.3, red: ≈2^120.0, svp: ≈2^120.6, β: 411, η: 413, ζ: 0, |S|: 1, d: 955, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^166.5, red: ≈2^165.5, svp: ≈2^165.4, β: 410, η: 2, ζ: 217, |S|: ≈2^201.1, d: 738, prob: ≈2^-43.6, ↻: ≈2^45.8, tag: hybrid
dual                 :: rop: ≈2^125.9, mem: ≈2^89.4, m: 407, β: 431, d: 919, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^122.7, mem: ≈2^114.2, m: 388, β: 399, d: 860, ↻: 51, ζ: 40, h1: 8, tag: dual_hybrid
== Smaug128-KS: LWR ==
Algorithm functools.partial(<function dual_hybrid at 0x7fe26fec1bd0>, red_cost_model=<reduction.ADPS16 object at 0x7fe2c7227fd0>, mitm_optimization=True) on LWEParameters(n=512, q=1024, Xs=D(σ=0.50), Xe=D(σ=1.12, μ=-0.50), m=768, tag='LWR') failed with β = 79 > d = 75
Algorithm <lwe_bkw.CodedBKW object at 0x7fe26fea4550> on LWEParameters(n=512, q=1024, Xs=D(σ=0.50), Xe=D(σ=1.12, μ=-0.50), m=768, tag='LWR') failed with Amplifying for μ≠0 not implemented.
arora-gb             :: rop: ≈2^693.6, dreg: 83, mem: ≈2^553.0, t: 1, m: ≈2^150.2, tag: arora-gb, ↻: ≈2^140.6, ζ: 234, |S|: 1, prop: ≈2^-138.4
usvp                 :: rop: ≈2^119.7, red: ≈2^119.7, δ: 1.003914, β: 410, d: 874, tag: usvp
bdd                  :: rop: ≈2^121.2, red: ≈2^119.7, svp: ≈2^120.6, β: 410, η: 413, d: 843, tag: bdd
bdd_hybrid           :: rop: ≈2^121.2, red: ≈2^119.7, svp: ≈2^120.6, β: 410, η: 413, ζ: 0, |S|: 1, d: 955, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^162.3, red: ≈2^161.4, svp: ≈2^161.3, β: 400, η: 2, ζ: 228, |S|: ≈2^195.4, d: 718, prob: ≈2^-42.4, ↻: ≈2^44.6, tag: hybrid
dual                 :: rop: ≈2^125.9, mem: ≈2^89.0, m: 400, β: 431, d: 912, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^121.9, mem: ≈2^112.8, m: 380, β: 396, d: 851, ↻: 59, ζ: 41, h1: 7, tag: dual_hybrid

==================
==  Smaug128-KS
==================
n=256, k=2, q=1024, p=256, p'=32, sigma=1.0625, hs=140, numCBD=68
====================
LWE/R Hardness (in Log_2):
 [Core-SVP] 115.6 (beta= 396.0)
 [ADPS16_classical] 119.7
Meet-LWE (in Log_2):
 [Rep-1] 181.1 with memory 156.5
 [Rep-2] 177.2 with memory 157.4
Decryption Failure Probability (in Log_2): 
 [DFP] -118.29
Sizes: 
 [PKE] pk=672, ctxt=672, sk=128 bytes
 [KEM] pk=672, ctxt=672, sk=832 bytes
========================================

ADPS16_classical
== Smaug192-KS: LWE ==
arora-gb             :: rop: ≈2^inf, m: ≈2^567.4, dreg: 202, t: 4, mem: ≈2^908.9, tag: arora-gb, ↻: ≈2^561.6, ζ: 505, |S|: ≈2^424.0, prop: ≈2^-135.4
bkw                  :: rop: ≈2^213.7, m: ≈2^201.1, mem: ≈2^202.1, b: 18, t1: 0, t2: 17, ℓ: 17, #cod: 658, #top: 0, #test: 111, tag: coded-bkw
usvp                 :: rop: ≈2^180.2, red: ≈2^180.2, δ: 1.002926, β: 617, d: 1313, tag: usvp
bdd                  :: rop: ≈2^181.0, red: ≈2^180.2, svp: ≈2^179.9, β: 617, η: 616, d: 1332, tag: bdd
bdd_hybrid           :: rop: ≈2^181.3, red: ≈2^180.2, svp: ≈2^180.5, β: 617, η: 618, ζ: 0, |S|: 1, d: 1417, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^253.8, red: ≈2^252.9, svp: ≈2^252.7, β: 617, η: 2, ζ: 294, |S|: ≈2^319.4, d: 1123, prob: ≈2^-70.5, ↻: ≈2^72.7, tag: hybrid
dual                 :: rop: ≈2^187.5, mem: ≈2^132.0, m: 612, β: 642, d: 1380, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^181.9, mem: ≈2^172.7, m: 586, β: 602, d: 1307, ↻: 56, ζ: 47, h1: 13, tag: dual_hybrid
== Smaug192-KS: LWR ==
Algorithm <lwe_bkw.CodedBKW object at 0x7fe26fea4550> on LWEParameters(n=768, q=2048, Xs=D(σ=0.71), Xe=D(σ=1.12, μ=-0.50), m=1024, tag='LWR') failed with Amplifying for μ≠0 not implemented.
usvp                 :: rop: ≈2^186.9, red: ≈2^186.9, δ: 1.002849, β: 640, d: 1365, tag: usvp
bdd                  :: rop: ≈2^188.0, red: ≈2^187.2, svp: ≈2^186.9, β: 641, η: 640, d: 1350, tag: bdd
bdd_hybrid           :: rop: ≈2^188.0, red: ≈2^187.2, svp: ≈2^186.9, β: 641, η: 640, ζ: 0, |S|: 1, d: 1437, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^296.1, red: ≈2^295.2, svp: ≈2^295.0, β: 641, η: 2, ζ: 240, |S|: ≈2^333.0, d: 1197, prob: ≈2^-105.8, ↻: ≈2^108.0, tag: hybrid
dual                 :: rop: ≈2^194.5, mem: ≈2^137.4, m: 644, β: 666, d: 1412, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^186.5, mem: ≈2^182.5, m: 625, β: 638, d: 1361, ↻: 1, ζ: 32, tag: dual_hybrid

==================
==  Smaug192-KS
==================
n=256, k=3, q=2048, p=512, p'=16, sigma=1.0625, hs=264, numCBD=48
====================
LWE/R Hardness (in Log_2):
 [Core-SVP] 175.8 (beta= 602.0)
 [ADPS16_classical] 180.2
Meet-LWE (in Log_2):
 [Rep-1] 304.3 with memory 267.1
 [Rep-2] 295.6 with memory 259.1
Decryption Failure Probability (in Log_2): 
 [DFP] -179.21
Sizes: 
 [PKE] pk=1088, ctxt=992, sk=192 bytes
 [KEM] pk=1088, ctxt=992, sk=1312 bytes
========================================

ADPS16_classical
== Smaug256-KS: LWE ==
arora-gb             :: rop: ≈2^inf, m: ≈2^752.1, dreg: 268, t: 4, mem: ≈2^inf, tag: arora-gb, ↻: ≈2^746.3, ζ: 677, |S|: ≈2^562.1, prop: ≈2^-182.0
bkw                  :: rop: ≈2^269.0, m: ≈2^256.0, mem: ≈2^257.0, b: 23, t1: 0, t2: 16, ℓ: 22, #cod: 863, #top: 0, #test: 163, tag: coded-bkw
usvp                 :: rop: ≈2^251.1, red: ≈2^251.1, δ: 1.002289, β: 860, d: 1749, tag: usvp
bdd                  :: rop: ≈2^252.3, red: ≈2^251.4, svp: ≈2^251.1, β: 861, η: 860, d: 1731, tag: bdd
bdd_hybrid           :: rop: ≈2^252.6, red: ≈2^251.4, svp: ≈2^251.7, β: 861, η: 862, ζ: 0, |S|: 1, d: 1850, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^342.5, red: ≈2^341.5, svp: ≈2^341.4, β: 861, η: 2, ζ: 418, |S|: ≈2^460.7, d: 1432, prob: ≈2^-87.9, ↻: ≈2^90.1, tag: hybrid
dual                 :: rop: ≈2^260.5, mem: ≈2^185.0, m: 775, β: 892, d: 1799, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^250.1, mem: ≈2^240.5, m: 739, β: 833, d: 1690, ↻: 92, ζ: 73, h1: 20, tag: dual_hybrid
== Smaug256-KS: LWR ==
Algorithm <lwe_bkw.CodedBKW object at 0x7fe26fea4550> on LWEParameters(n=1024, q=2048, Xs=D(σ=0.61), Xe=D(σ=1.12, μ=-0.50), m=1280, tag='LWR') failed with Amplifying for μ≠0 not implemented.
arora-gb             :: rop: ≈2^inf, dreg: 158, mem: ≈2^inf, t: 1, m: ≈2^528.6, tag: arora-gb, ↻: ≈2^518.3, ζ: 491, |S|: ≈2^184.3, prop: ≈2^-331.8
usvp                 :: rop: ≈2^254.9, red: ≈2^254.9, δ: 1.002264, β: 873, d: 1731, tag: usvp
bdd                  :: rop: ≈2^255.8, red: ≈2^254.9, svp: ≈2^254.6, β: 873, η: 872, d: 1751, tag: bdd
bdd_hybrid           :: rop: ≈2^256.2, red: ≈2^254.9, svp: ≈2^255.5, β: 873, η: 875, ζ: 0, |S|: 1, d: 1860, prob: 1, ↻: 1, tag: hybrid
bdd_mitm_hybrid      :: rop: ≈2^358.7, red: ≈2^357.9, svp: ≈2^357.5, β: 873, η: 2, ζ: 416, |S|: ≈2^467.0, d: 1444, prob: ≈2^-100.8, ↻: ≈2^103.0, tag: hybrid
dual                 :: rop: ≈2^264.3, mem: ≈2^187.4, m: 784, β: 905, d: 1808, ↻: 1, tag: dual
dual_hybrid          :: rop: ≈2^254.5, mem: ≈2^245.2, m: 750, β: 850, d: 1714, ↻: 63, ζ: 60, h1: 19, tag: dual_hybrid

==================
==  Smaug256-KS
==================
n=256, k=4, q=2048, p=512, p'=128, sigma=1.0625, hs=348, numCBD=58
====================
LWE/R Hardness (in Log_2):
 [Core-SVP] 243.2 (beta= 833.0)
 [ADPS16_classical] 250.1
Meet-LWE (in Log_2):
 [Rep-1] 411.7 with memory 366.5
 [Rep-2] 401.4 with memory 353.1
Decryption Failure Probability (in Log_2): 
 [DFP] -194.16
Sizes: 
 [PKE] pk=1440, ctxt=1376, sk=256 bytes
 [KEM] pk=1440, ctxt=1376, sk=1728 bytes
========================================
