"""
Modified from security-estimates/Kyber_failure.py,
which is available at https://github.com/pq-crystals/security-estimates
"""
import operator as op
from math import factorial as fac
from math import sqrt, log, ceil
import sys
from proba_util import *

def prob_final_error(sig, n, k, q, p, pp, numCBD, hs, isTimer=False):
    """ construct the final error distribution in our encryption scheme
    :param ps: parameter set (ParameterSet)
    """
    #print("Param: sig="+str(sig)+", n="+str(n)+", k="+str(k)+", q="+str(q)+", p="+str(p)+", p'="+str(pp)+", CBD="+str(numCBD)+", hs="+str(hs))
    #print(f"Sizes: pk={32+n*k*log(q, 2)/8:.0f}, ctxt={n*(k*log(p, 2)+log(pp, 2))/8:.0f}")
    #, sk={16*ceil(hs/16)+32} bytes")
    
    ##############
    # initialize #
    ##############
    # LWE error (dGaussian)
    if sig==1.01:
        D_e = {-3: 5.0/1024, -2: 57.0/1024, -1: 248.0/1024, 0:404.0/1024, 1: 248.0/1024, 2: 57.0/1024, 3: 5.0/1024}
    elif sig==1.06:
        D_e = {-3: 7.0/1024, -2: 65.0/1024, -1: 247.0/1024, 0:384.0/1024, 1: 247.0/1024, 2: 65.0/1024, 3: 7.0/1024}
    elif sig==1.1:
        D_e = {0: 372.0/1024, 1: 246.0/1024, -1: 246.0/1024, 2: 71.0/1024, -2: 71.0/1024, 3: 9.0/1024, -3: 9.0/1024}
    elif sig==1.2:
        D_e = {0: 340.0/1024, 1: 241.0/1024, -1: 241.0/1024, 2: 85.0/1024, -2: 85.0/1024, 3: 15.0/1024, -3:15.0/1024, 4: 1.0/1024, -4: 1.0/1024}
    elif sig==1.45:
        D_e = {0: 562.0/2048, 1:444.0/2048, 2:218.0/2048, 3: 67.0/2048, 4:13.0/2048, 5: 2.0/2048, -1:444.0/2048, -2:218.0/2048, -3: 67.0/2048, -4:13.0/2048, -5: 2.0/2048}
    
    # LWR secret for ctxt (CBD or modifiedCBD)
    if (numCBD != 68) & (numCBD != 58):
        D_r = build_centered_binomial_law(numCBD)
    # (previously 0)
    elif numCBD==68:
        # 0 with 6/8 = 3/4, +-1 with 1/8
        D_r = {0: 0.75, 1: 0.125, -1: 0.125}
    # (previously -1)
    elif numCBD == 58:
        # 0 with 5/8, +-1 with 3/16
        D_r = {0: 0.625, 1: 0.1875, -1: 0.1875}
    
    # LWR error for ctxt    (ModSwitch q->p)
    D_e1 = build_mod_switching_error_law(q, p)
    
    # LWR error for ctxt    (ModSwitch q->pp)
    D_e2 = build_mod_switching_error_law(q, pp)
    
    ###########
    # combine #
    ###########

    D_er = iter_law_convolution(law_product(D_e, D_r), k*n)         # <e, r>
    D_e1s = iter_law_convolution(D_e1, hs)                          # <e1, s> = e1+...+e1, hs times.
    
    D_er_e1s = law_convolution(D_er, D_e1s) 
    #convolution_remove_dependency(D_er, D_e1s, q, p)     # <e, r> + <e1, s>
    
    D = law_convolution(D_er_e1s, D_e2)                             # <e, r> + <e1, s> + e2
    bound = q/4.

    if isTimer:
        D = law_convolution(D, D)
        bound = q/2.

    prob = tail_probability(D, bound)                                # Pr[ |<e, r> + <e1, s> + e2| > q/4 ]
    
    if prob!=0:
        prob = log(n*prob,2)                                        # for each n coefficients
    
    return prob

# DFP larger than 2^-60
#print(prob_final_error(256, 2, 1024, 256, 256, 2, 140))
#print(prob_final_error(256, 2, 1024, 256, 128, 2, 140))
#print(prob_final_error(1.06, 256, 2, 1024, 256, 256, 2, 120)) # -56
#print(prob_final_error(1.06, 256, 2, 1024, 512, 256, 2, 120)) # -66

# Too large ctxt
#print(prob_final_error(256, 2, 2048, 256, 256, 2, 140))
#print(prob_final_error(256, 2, 2048, 256, 128, 2, 140))

# Little bit low DFP
#print(prob_final_error(256, 2, 2048, 256, 16 , 2, 140))

# DFP larger than 2^-110
#print(prob_final_error(256, 2, 2048, 256, 256, 3, 140))
#print(prob_final_error(256, 2, 2048, 256, 128, 3, 140))
#print(prob_final_error(256, 2, 2048, 256, 64 , 3, 140))

#######################
# Good sizes and DFPs #
#######################
# Candidates
"""
# TiMER
print("\n"+"*"*19 + "\n****** TiMER ******\n" + "*"*19)
print("\n"+"**New TiMER param 1: \nProp : ")
#print("Security: MLWE: 120.0 & MLWR: 127.9 & DFP:", prob_final_error(1.06, 256, 2, 1024, 256, 8  ,  1, 140, isTimer=True))
#print("Security: MLWE: 120.0 & MLWR: 124.4 & DFP:", prob_final_error(1.06, 256, 2, 1024, 256, 8  , 58, 140, isTimer=True))
print("Security: MLWE: 120.0 & MLWR: 119.7 & DFP:", prob_final_error(1.06, 256, 2, 1024, 256, 8  ,  68, 140, isTimer=True))

print("\n"+"**New TiMER param 2: \nProp : ")
#print("Security: MLWE: 120.3 & MLWR: 127.9 & DFP:", prob_final_error(1.2, 256, 2, 1024, 256, 8  ,  1, 120, isTimer=True))
#print("Security: MLWE: 120.3 & MLWR: 124.4 & DFP:", prob_final_error(1.2, 256, 2, 1024, 256, 8  , 58, 120, isTimer=True))
print("Security: MLWE: 120.3 & MLWR: 119.7 & DFP:", prob_final_error(1.2, 256, 2, 1024, 256, 8  ,  68, 120, isTimer=True))

print("\n"+"**New TiMER param 3: \nProp : ")
#print("Security: MLWE: 122.9  & MLWR: 127.9 & DFP:", prob_final_error(1.45, 256, 2, 1024, 256, 8  ,  1, 114, isTimer=True))
#print("Security: MLWE: 122.9  & MLWR: 124.4 & DFP:", prob_final_error(1.45, 256, 2, 1024, 256, 8  , 58, 114, isTimer=True))
print("Security: MLWE: 122.9  & MLWR: 119.7 & DFP:", prob_final_error(1.45, 256, 2, 1024, 256, 8  ,  68, 114, isTimer=True))
#"""
"""
# SMAUG-T128
print("\n"+"*"*19 + "\n*** SMAUG-T 128 ***\n" + "*"*19)
print("\n"+"**New 128 param 1: \nProp : Larger ctxt than before // Use CBD(0)")
print("Security: MLWE: 120.0 & MLWR: 119.7 & DFP:", prob_final_error(1.06, 256, 2, 1024, 256, 32 , 68, 140))
print("\n"+"**New 128 param 2: \nProp : A bit larger DFP than before // Use CBD(0)")
print("Security: MLWE: 120.0 & MLWR: 119.7 & DFP:", prob_final_error(1.06, 256, 2, 1024, 256, 64 , 68, 140))

# SMAUG-T192
print("\n"+"*"*19 + "\n*** SMAUG-T 192 ***\n" + "*"*19)
print("\n"+"**New 192 param 1: \nProp : Very low DFP // Use CBD(-1) // Smaller ctxt than before")
print("Security: MLWE: 180.2 & MLWR: 182.5 & DFP:", prob_final_error(1.06, 256, 3, 2048, 512, 16 , 58, 264))
print("\n"+"**New 192 param 2: \nProp : Use CBD(1) // A bit large DFP (but much lower than before) // Smaller ctxt than before")
print("Security: MLWE: 180.2 & MLWR: 186.5 & DFP:", prob_final_error(1.06, 256, 3, 2048, 512, 16 ,  1, 264))

# SMAUG-T256
print("\n"+"*"*19 + "\n*** SMAUG-T 256 ***\n" + "*"*19)
print("\n"+"**New 256 param: \nProp : Use k=4!!! // Use CBD(-1) // Much lower DFP than before // Much smaller sizes than before")
print("Security: MLWE: 250.1 & MLWR: 254.5 & DFP:", prob_final_error(1.06, 256, 4, 2048, 512, 128, 58, 348))
#"""

"""
print("\n Note 1. \n CBD (Centered Binomial Distribution) and modified CBD is used: \n CBD(1)  = {0:4/8, 1:1/4, -1:1/4} \n CBD(58) = {0:5/8, -1:3/16, 1:3/16} (previously -1) \n CBD(68)  = {0:6/8, -1:1/8, 1:1/8} (previously 0)")
print("\n Note 2. \n Sigma is stddev of dGaussian, which is fixed as 1.0625 for all parameter sets.")
#"""

#######
# 128 #
#######
## 128 param with sig = 1.06
# q=1024
#print(prob_final_error(1.06, 256, 2, 1024, 256, 64 ,  0, 140))  # -123  // LWE 120.0  // LWR 119.7  // 672 + 704
#print(prob_final_error(1.06, 256, 2, 1024, 256, 32 ,  0, 140))  # -116  // LWE 120.0  // LWR 119.7  // 672 + 672
#print(prob_final_error(1.06, 256, 2, 1024, 256, 64 , -1, 140))  # -102  // LWE 120.0  // LWR 124.4
#print(prob_final_error(1.06, 256, 2, 1024, 256, 64 ,  1, 140))  # -87   // LWE 120.0  // LWR 127.9

#print(prob_final_error(1.06, 256, 2, 1024, 512, 64 ,  0, 140))  # -198  // LWE 120.0  // LWR 106.6
#print(prob_final_error(1.06, 256, 2, 1024, 512, 64 , -1, 140))  # -144  // LWE 120.0  // LWR 108.8 
#print(prob_final_error(1.06, 256, 2, 1024, 512, 64 ,  1, 140))  # -118  // LWE 120.0  // LWR 111.0

# q=2048
#print(prob_final_error(1.06, 256, 2, 2048, 256, 256,  0, 460))  # -67   // LWE 120.0  // LWR 120.3
#print(prob_final_error(1.06, 256, 2, 2048, 256, 256, -1, 460))  # -65   // LWE 120.0  // LWR 125.0
#print(prob_final_error(1.06, 256, 2, 2048, 256, 256,  1, 460))  # -63   // LWE 120.0  // LWR 128.5

# q=2048, p=512 or larger -> no possibilities with CBD<=1
#print(prob_final_error(1.06, 256, 2, 2048, 512, 8  ,  0, 460))  # -132  // LWE 120.3  // LWR 108.3  // 736 + 672
#print(prob_final_error(1.06, 256, 2, 2048, 512, 8  , -1, 460))  # -121  // LWE 120.3  // LWR 112.4  // 736 + 672
#print(prob_final_error(1.06, 256, 2, 2048, 512, 16 ,  1, 460))  # -151  // LWE 120.3  // LWR 115.0

## 128 param with sig = 1.1
# q=1024
#print(prob_final_error(1.1, 256, 2, 1024, 256, 64 ,  0, 134))  # -122  // LWE 120.0  // LWR 119.7   // 672 + 704
#print(prob_final_error(1.1, 256, 2, 1024, 256, 32 ,  0, 134))  # -115  // LWE 120.0  // LWR 119.7   // 672 + 672
#print(prob_final_error(1.1, 256, 2, 1024, 256, 512, -1, 134))  # -103
#print(prob_final_error(1.1, 256, 2, 1024, 256, 512,  1, 134))  # -87

#print(prob_final_error(1.1, 256, 2, 1024, 512, 16 ,  0, 134))  # -149  // LWE 120.0  // LWR 106.6

# q=2048
#print(prob_final_error(1.1, 256, 2, 2048, 256, 512,  0, 456))  # -67

## 128 param with sig = 1.2, TRYYYY!
# q=1024
#print(prob_final_error(1.2, 256, 2, 1024, 256, 512,  0, 118))  # -121  // LWE 120.3  // LWR 119.7   // 672 + 800
#print(prob_final_error(1.2, 256, 2, 1024, 256, 512, -1, 118))  # -97
#print(prob_final_error(1.2, 256, 2, 1024, 256, 512,  1, 118))  # -80

# q=2048
#print(prob_final_error(1.2, 256, 2, 2048, 256, 512,  0, 370))  # -82


## 128 param with sig = 1.45, LWR 115.0
# q=1024
#print(prob_final_error(1.45, 256, 2, 1024, 256, 512,  0, 114))  # -95
#print(prob_final_error(1.45, 256, 2, 1024, 256, 512, -1, 114))  # -73
#print(prob_final_error(1.45, 256, 2, 1024, 256, 512,  1, 114))  # -59

# q=2048
#print(prob_final_error(1.45, 256, 2, 2048, 256, 512,  0, 260))  # -109


#######
# 192 #
#######
##
## 192 param with sig = 1.06
# q=1024
#print(prob_final_error(1.06, 256, 3, 1024, 256, 512, -1, 198))  # -71  // LWE 190.2   // LWR 200
#print(prob_final_error(1.06, 256, 3, 1024, 256, 512,  0, 192))  # -88  // LWE ~180
#print(prob_final_error(1.06, 256, 3, 1024, 256, 512,  1, 198))  # -59

#print(prob_final_error(1.06, 256, 3, 1024, 512, 512, -1, 198))  # -103  // LWE 190.2   // LWR 173.5
#print(prob_final_error(1.06, 256, 3, 1024, 512, 512,  0, 198))  # -136  // LWE 190.2   // LWR 173.1
#print(prob_final_error(1.06, 256, 3, 1024, 512, 512,  1, 198))  # -83 

# q=2048
#print(prob_final_error(1.06, 256, 3, 2048, 256, 512, -1, 264))  # -105
#print(prob_final_error(1.06, 256, 3, 2048, 512, 32 , -1, 264))  # -231   // LWE 180.2    // LWR 182.5  // 1088 + 1024 
#print(prob_final_error(1.06, 256, 3, 2048, 512, 16 , -1, 264))  # -203   // LWE 180.2    // LWR 182.5  // 1088 + 992 ***

#print(prob_final_error(1.06, 256, 3, 2048, 512, 8  , -1, 264))  # -150
#print(prob_final_error(1.06, 256, 3, 2048, 512, 512,  0, 264))  # -304   // LWE 180.2    // LWR 175.4
#print(prob_final_error(1.06, 256, 3, 2048, 512, 32 ,  1, 264))  # -201   // LWE 180.2    // LWR 186.5  // 1088 + 1024
#print(prob_final_error(1.06, 256, 3, 2048, 512, 16 ,  1, 264))  # -177   // LWE 180.2    // LWR 186.5  // 1088 + 992 ***

#print(prob_final_error(1.06, 256, 3, 2048, 512, 8  ,  1, 264))  # -131

##
## 192 param with sig = 1.1
# q=1024 no possible options-- 1.1, 1024, 512, CBD=0, 192 => -131, so no more for sig > 1.1
# q=2048
#print(prob_final_error(1.1, 256, 3, 2048, 256, 512,  0, 255))  # -112
#print(prob_final_error(1.1, 256, 3, 2048, 512, 512,  0, 255))  # -295   // LWE 180.2    // LWR 175

#print(prob_final_error(1.1, 256, 3, 2048, 512, 16 , -1, 255))  # -199   // LWE 180.2    // LWR 182.5   // 1088 + 992 ***
#print(prob_final_error(1.1, 256, 3, 2048, 512, 8  , -1, 255))  # -148
#print(prob_final_error(1.1, 256, 3, 2048, 512, 32 ,  1, 255))  # -194   // LWE 180.2    // LWR 186.5   // 1088 + 1024
#print(prob_final_error(1.1, 256, 3, 2048, 512, 16 ,  1, 255))  # -178   // LWE 180.2    // LWR 186.5   // 1088 + 992

# Security too low, so no more for sig > 1.1
#print(prob_final_error(1.1, 256, 3, 2048,1024, 512,  0, 255))  # -457   // LWE 180.2    // LWR 159.7
#print(prob_final_error(1.1, 256, 3, 2048,1024, 512, -1, 255))  # -213   // LWE 180.2    // LWR 159.8
#print(prob_final_error(1.1, 256, 3, 2048,1024, 8  ,  1, 255))  # -176   // LWE 180.2    // LWR 162.5

##
## 192 param with sig = 1.2
# q=2048
#print(prob_final_error(1.2, 256, 3, 2048, 256, 512,  0, 225))  # -125

#print(prob_final_error(1.2, 256, 3, 2048, 512, 512,  0, 225))  # -292   // LWE 180.5    // LWR 175.4
#print(prob_final_error(1.2, 256, 3, 2048, 512, 16   -1, 225))  # -191   // LWE 180.5    // LWR 182.5 // 1088 + 992 ***
#print(prob_final_error(1.2, 256, 3, 2048, 512, 512,  1, 225))  # -205   // LWE 180.5    // LWR 175.4

#print(prob_final_error(1.2, 256, 3, 2048,1024, 512,  0, 225))  # -415   // LWE 180.5    // LWR 159.7
#print(prob_final_error(1.2, 256, 3, 2048,1024, 512, -1, 225))  # -415   // LWE 180.5    // LWR 159.8
#print(prob_final_error(1.2, 256, 3, 2048,1024, 16 ,  1, 225))  # -210   // LWE 180.5    // LWR 186.5 // 1088 + 1088


#######
# 256 #
#######
# 256 param with sig = 1.06, SECURE!
#print(prob_final_error(1.06, 256, 4, 2048, 512, 512, -1, 348)) # -194.0  // 1440
#print(prob_final_error(1.06, 256, 4, 2048, 512, 256, -1, 348)) # -193.3  // 1408
#print(prob_final_error(1.06, 256, 4, 2048, 512, 128, -1, 348)) # -191    // 1376 ***
#print(prob_final_error(1.06, 256, 4, 2048, 512,  64, -1, 348)) # -186    // 1344
#print(prob_final_error(1.06, 256, 4, 2048, 512,  32, -1, 348)) # -175    // 1312
#print(prob_final_error(1.06, 256, 4, 2048, 512,  16, -1, 348)) # -153    // 1280

# 256 param with sig = 1.1, SECURE!
#print(prob_final_error(1.1, 256, 4, 2048, 512, 512, -1, 340)) # -189.8  // 1440 
#print(prob_final_error(1.1, 256, 4, 2048, 512, 256, -1, 340)) # -189.2  // 1408
#print(prob_final_error(1.1, 256, 4, 2048, 512, 128, -1, 340)) # -187    // 1376
#print(prob_final_error(1.1, 256, 4, 2048, 512,  64, -1, 340)) # -182    // 1344
#print(prob_final_error(1.1, 256, 4, 2048, 512,  32, -1, 340)) # -171    // 1312
#print(prob_final_error(1.1, 256, 4, 2048, 512,  16, -1, 340)) # -150    // 1280

# 256 param with sig = 1.2, SECURE!
#print(prob_final_error(1.2, 256, 4, 2048, 512, 512, -1, 320)) # -176.9  // 1440
#print(prob_final_error(1.2, 256, 4, 2048, 512, 256, -1, 320)) # -176.3  // 1408
#print(prob_final_error(1.2, 256, 4, 2048, 512, 128, -1, 320)) # -172    // 1376
#print(prob_final_error(1.2, 256, 4, 2048, 512,  64, -1, 320)) # -170    // 1344


