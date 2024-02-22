from math import log, floor, sqrt, log2 #, prod
from scipy.special import gammaln
import numpy as np

def log2_multinom(c):
    return (gammaln(c.sum()+1) - gammaln(c+1).sum()) / log(2)

def meet_lwe_rep0(n, q, w):
    n2 = floor(n/2)
    
    w2 = floor(w/2)
    w4 = floor(w/4)
    w8 = floor(w/8)
    
    # Compute log_2 of L^(1) = S^(1) / R^(1), 
    # where S^(1) = (n choose w/4, w/4, n-w/2) and
    # R^(1) = (w/2 choose w/4, w/4). 
    logS1 = log2_multinom(np.array([w4, w2-w4, n-w2]))
    logR1 = 2*log2_multinom(np.array([w4, w2-w4]))
    logL1 = logS1 - logR1

    # Compute log_2 of L^(2) = S^(2), 
    # where S^(2) = n/2 choose w/8, w/9, n/2-w/4
    logL2 = log2_multinom(np.array([w8, w4-w8, n-n2-w4]))

    # Compute T_l for the list construction, 
    # where T_l = max(L^(1), L^(2))
    logT_l = max(logL1, logL2)

    # Compute T_g for the guessing, 
    # where T_g = 3^(r/2) = 3^( log_q(R^(1))/2 )
    # which leads to log_2 (T_g) = 0.5*log_2(R^(1))*log_2(3)/log_2(q)
    logT_g = 0.5*floor(logR1/log2(q))*log2(3)

    return (logT_l+logT_g, logT_l, logT_g, logT_l)

def meet_lwe_rep1(n, q, w, e_list):
    (e1, e2, e3) = e_list
    w0 = floor(w/2)
    w1 = floor(w0/2)+e1
    w2 = floor(w1/2)+e2
    if e3!=0:
        w3 = floor(w2/2)+e3

    # Compute log_2 of R^(1) and S^(1), 
    # where R^(1) = (w0 choose w0/2, w0/2)**2 * (n-2*w0 choose e1, e1, n-2*w0-2*e1)
    # and S^(1) = ( n choose w1, w1, n-2*w1)
    logR1 = 2*log2_multinom(np.array([floor(w0/2), w0-floor(w0/2)])) + log2_multinom(np.array([e1, e1, n-2*w0-2*e1]))
    logS1 = log2_multinom(np.array([w1, w1, n-2*w1]))

    # Compute log_2 of R^(2) and S^(2)
    logR2 = 2*log2_multinom(np.array([floor(w1/2), w1-floor(w1/2)])) + log2_multinom(np.array([e2, e2, n-2*w1-2*e2]))
    logS2 = log2_multinom(np.array([w2, w2, n-2*w2]))

    # Compute log_2 of R^(3) and S^(3)
    if e3!=0: 
        logR3 = 2*log2_multinom(np.array([floor(w2/2), w2-floor(w2/2)])) + log2_multinom(np.array([e3, e3, n-2*w2-2*e3]))
        logS3 = log2_multinom(np.array([w3, w3, n-2*w3]))
    else:
        logR3 = 1

    # Compute log_2 of L^(4) to L^(1) and T^(0)
    if e3!=0:
        logL4 = logS3 / 2.0
        logL3 = 2*logL4 - floor(logR3/log2(q))*log2(q)
    else: 
        logL3 = logS2 / 2.0
    logL2 = 2*logL3 - floor(logR2/log2(q))*log2(q) + floor(logR3/log2(q))*log2(q)
    logL1 = 2*logL2 - floor(logR1/log2(q))*log2(q) + floor(logR2/log2(q))*log2(q)
    logT0 = 2*logL1 - n + floor(logR1/log2(q))
    
    # Compute T_l for the list construction
    if e3!=0:
        logT_l = max(logT0, logL1, logL2, logL3, logL4)
    else:
        logT_l = max(logT0, logL1, logL2, logL3)

    # Compute T_g for the guessing,
    logT_g = 0.5*floor(logR1/log2(q))*log2(3)

    # compute M for memory consumption
    if e3!=0:
        logM = max(logL1, logL2, logL3, logL4)
    else:
        logM = max(logL1, logL2, logL3)

    return (logT_l+logT_g, logT_l, logT_g, logM)

def meet_lwe_rep2(n, q, w, e_list):
    (e110, e121, e210, e310) = e_list
    e120 = 0
    e220 = 0
    e221 = 0
    e222 = 0
    e320 = 0
    e321 = 0
    e322 = 0

    n00 = n-w
    n01 = floor(w / 2.0)
    n02 = 0

    n12 = e121 + e120
    n11 = floor(n01 / 2.0) + e110
    n10 = n - 2*n12 - 2*n11
    logS1 = log2_multinom(np.array([n12, n12, n11, n11, n10]))
    logR1 = 2*log2_multinom(np.array([e121, e121, floor(n01/2.0)-e121, n01-floor(n01/2.0)-e121])) 
    logR1 = logR1 + log2_multinom(np.array([e120, e120, e110, e110, n-2*n01-2*e120-2*e110]))
    
    n22 = e222 + e221 + e220
    n21 = floor(n11 / 2.0) + e210 + n12 - 2*e222
    n20 = n - 2*n22 - 2*n21

    logS2 = log2_multinom(np.array([n22, n22, n21, n21, n20]))
    logR2 = 2*log2_multinom(np.array([e222, e222, n12-2*e222]))
    logR2 = logR2 + 2*log2_multinom(np.array([e221, e221, floor(n11/2.0)-e221, n11 - floor(n11/2.0)-e221]))
    logR2 = logR2 + log2_multinom(np.array([e220, e220, e210, e210, n10-2*e220-2*e210]))

    n32 = e322 + e321 + e320
    n31 = floor(n21 / 2.0) + e310 + n22 - 2*e322
    n30 = n - 2*n32 - 2*n31
    
    logS3 = log2_multinom(np.array([n32, n32, n31, n31, n30]))
    logR3 = 2*log2_multinom(np.array([e322, e322, n22-2*e322]))
    logR3 = logR3 + 2*log2_multinom(np.array([e321, e321, floor(n21/2.0)-e321, n21 - floor(n21/2.0)-e321]))
    logR3 = logR3 + log2_multinom(np.array([e320, e320, e310, e310, n20-2*e320-2*e310]))

    logL4 = logS3 / 2.0
    logL3 = 2*logL4 - floor(logR3/log2(q))*log2(q)
    logL2 = 2*logL3 - floor(logR2/log2(q))*log2(q) + floor(logR3/log2(q))*log2(q)
    logL1 = 2*logL2 - floor(logR1/log2(q))*log2(q) + floor(logR2/log2(q))*log2(q)
    logT0 = 2*logL1 - n + floor(logR1/log2(q))

    logT_l = max(logT0, logL1, logL2, logL3, logL4)
    logT_g = 0.5*floor(logR1/log2(q))*log2(3)
    logM = max(logL1, logL2, logL3, logL4)

    return (logT_l+logT_g, logT_l, logT_g, logM)

def meet_lwe_rep1_for(n, q, w):
    cost = 1000
    for e1 in range(1, 25):
        for e2 in range(1, 10):
            for e3 in range(0, 5):
                e_list_tmp = (e1, e2, e3)
                cost_tmp = meet_lwe_rep1(n, q, w, e_list_tmp)[0]
                if cost > cost_tmp:
                    cost = cost_tmp
                    e_list_fin = e_list_tmp
    return (meet_lwe_rep1(n, q, w, e_list_fin), e_list_fin)


def meet_lwe_rep2_for(n, q, w):
    e_list = (round(w*0.05), round(w*0.001), round(w*0.026), round(w*0.006))
    cost = 1000
    for e0 in range(0, e_list[0]+10):
        for e1 in range(0, e_list[1]+5):
            for e2 in range(0,e_list[2]+5):
                for e3 in range(0, e_list[3]+2):
                    e_list_tmp = (e0, e1, e2, e3)
                    cost_tmp = meet_lwe_rep2(n, q, w, e_list_tmp)[0]
                    if cost > cost_tmp:
                        cost = cost_tmp
                        e_list_fin = (e0, e1, e2, e3)
    return (meet_lwe_rep2(n, q, w, e_list_fin), e_list_fin)

def meet_lwe(n, q, w):
#     tm0 = meet_lwe_rep0(n, q, w)
    tm1 = meet_lwe_rep1_for(n, q, w)
    tm2 = meet_lwe_rep2_for(n, q, w)
    if tm1[0][0] > tm2[0][0]:
        return tm2[0]
    else: 
        return tm1[0]

# ###################
# # Parameters Sets #
# ###################
# outdated parameters
# smaug128 = [256*2, 1024, min(140, 132)]
# smaug192 = [256*3, 1024, min(150, 147)]
# smaug256 = [256*5, 1024, 145]
# print("WARNING: SMAUG256 uses hr=hs=145") 
# # smaug256 = [256*5, 1024, min(145, 140)]
#
# new parameters, v1.0
# smaug128 = [256*2, 1024, min(140, 132)]
# smaug192 = [256*3, 1024, min(198, 151)]
# smaug256 = [256*5, 1024, min(176, 160)]
#
# ###########################
# # Meet-LWE attack (Rep-0) #
# ###########################
# print("="*16)
# print(" Meet-LWE Rep-0 ")
# print("="*16)
# 
# tm128 = meet_lwe_rep0(smaug128[0], smaug128[1], smaug128[2])
# tm192 = meet_lwe_rep0(smaug192[0], smaug192[1], smaug192[2])
# tm256 = meet_lwe_rep0(smaug256[0], smaug256[1], smaug256[2])
# print(f"SMAUG128: time= {tm128[0]:4.1f} = {tm128[1]:4.1f} + {tm128[2]:4.1f}, memory= {tm128[3]:4.1f}")
# print(f"SMAUG192: time= {tm192[0]:4.1f} = {tm192[1]:4.1f} + {tm192[2]:4.1f}, memory= {tm192[3]:4.1f}")
# print(f"SMAUG256: time= {tm256[0]:4.1f} = {tm256[1]:4.1f} + {tm256[2]:4.1f}, memory= {tm256[3]:4.1f}")
# 
# ###########################
# # Meet-LWE attack (Rep-1) #
# ###########################
# print("\n"+"="*16)
# print(" Meet-LWE Rep-1 ")
# print("="*16)
# 
# tm128 = meet_lwe_rep1_for(smaug128[0], smaug128[1], smaug128[2])
# tm192 = meet_lwe_rep1_for(smaug192[0], smaug192[1], smaug192[2])
# tm256 = meet_lwe_rep1_for(smaug256[0], smaug256[1], smaug256[2])
# print(f"SMAUG128: time= {tm128[0][0]:4.1f} = {tm128[0][1]:4.1f} + {tm128[0][2]:4.1f}, memory= {tm128[0][3]:4.1f} with {tm128[1]}")
# print(f"SMAUG192: time= {tm192[0][0]:4.1f} = {tm192[0][1]:4.1f} + {tm192[0][2]:4.1f}, memory= {tm192[0][3]:4.1f} with {tm192[1]}")
# print(f"SMAUG256: time= {tm256[0][0]:4.1f} = {tm256[0][1]:4.1f} + {tm256[0][2]:4.1f}, memory= {tm256[0][3]:4.1f} with {tm256[1]}")
# 
# ###########################
# # Meet-LWE attack (Rep-2) #
# ###########################
# print("")
# print("="*16)
# print(" Meet-LWE Rep-2 ")
# print("="*16)
# 
# tm128 = meet_lwe_rep2_for(smaug128[0], smaug128[1], smaug128[2])
# tm192 = meet_lwe_rep2_for(smaug192[0], smaug192[1], smaug192[2])
# tm256 = meet_lwe_rep2_for(smaug256[0], smaug256[1], smaug256[2])
# print(f"SMAUG128: time= {tm128[0][0]:4.1f} = {tm128[0][1]:4.1f} + {tm128[0][2]:4.1f}, memory= {tm128[0][3]:4.1f} with {tm128[1]}")
# print(f"SMAUG192: time= {tm192[0][0]:4.1f} = {tm192[0][1]:4.1f} + {tm192[0][2]:4.1f}, memory= {tm192[0][3]:4.1f} with {tm192[1]}")
# print(f"SMAUG256: time= {tm256[0][0]:4.1f} = {tm256[0][1]:4.1f} + {tm256[0][2]:4.1f}, memory= {tm256[0][3]:4.1f} with {tm256[1]}")
# 
# tm_exp = meet_lwe_rep2_for(256*5, 2048, 250)
# print(f"exp: time= {tm_exp[0][0]:4.1f} = {tm_exp[0][1]:4.1f} + {tm_exp[0][2]:4.1f}, memory= {tm_exp[0][3]:4.1f} with {tm_exp[1]}")
