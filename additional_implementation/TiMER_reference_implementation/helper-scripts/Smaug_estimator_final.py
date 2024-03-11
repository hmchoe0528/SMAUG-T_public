from nd import NoiseDistribution, stddevf
from lwe_parameters import LWEParameters
from lwe import estimate
from reduction import *
import math
from Meet_LWE import meet_lwe
from smaug_dfp_compression import runSmaugError


# run_estimator = 0 (don't run), 1 (lwe only), 2 (lwr only) or 3 (both)
def expSmaugParams(n, k, q, p, T, hs, hr, sigma = 1.0625, run_estimator = 0, run_meet_lwe = True, run_dfp = True, tag = "SmaugExp"):
    
    ############
    # Core-SVP #
    ############
    if run_estimator > 0:
        red_cost_models = [ADPS16("classical"), ADPS16("quantum")]#MATZOV("classical"), 
        model_names     = ["ADPS16_classical", " ADPS16_quantum "]#"MATZOV_classical", 
    
        list_attack = ['arora-gb', 'bkw', 'usvp', 'bdd', 'bdd_hybrid', 'bdd_mitm_hybrid', 'dual', 'dual_hybrid']
        
        Smaug_LWE = LWEParameters(n=n*k,
                q=q,
                Xs=NoiseDistribution.SparseTernary(n*k, int(hs / 2), int(hs / 2)),
                Xe=NoiseDistribution.DiscreteGaussian(sigma),
                m=n*k,
                tag="LWE",
                )
        
        Smaug_LWR = LWEParameters(n=n*k,
                q=q,
                Xs=NoiseDistribution.SparseTernary(n*k, int(hr / 2), int(hr / 2)),
                Xe=NoiseDistribution.UniformMod(int(q / p)),
                m=n*k,
                tag="LWR",
                )
        
        core_svp_list = []
        for red_cost_model, model_name in zip(red_cost_models, model_names):
            print("\n"+model_name)
            list_rop = []
            
            # run lwe
            if run_estimator != 2:
                print(f"== {tag}: LWE ==")
                resLWE = estimate(Smaug_LWE, red_cost_model=red_cost_model, jobs=40)
                for x in list_attack:
                    if x in resLWE:
                        list_rop.append(math.log2(resLWE[x].rop))
            # run lwr
            if run_estimator != 1:
                print(f"== {tag}: LWR ==")
                resLWR = estimate(Smaug_LWR, red_cost_model=red_cost_model, jobs=40)
                for x in list_attack:
                    if x in resLWR:
                        list_rop.append(math.log2(resLWR[x].rop))
            
            core_svp_list.append(min(list_rop))
        
        print("\n" + "="*20 + "\n===   "+tag+"   ===\n" + "="*20 + "\nn="+str(n)+", k="+str(k)+", q="+str(q)+", p="+str(p)+", sigma="+str(sigma)+", hs="+str(hs)+", hr="+str(hr)+"\n"+"=" * 20)
        idx = 0
        print(f"Core-SVP hardness:")
        for model_name in model_names:
            print(f"({model_name}) {core_svp_list[idx]:.1f}")
            idx = idx + 1
    
    if run_estimator ==0:
        print("\n" + "="*20 + "\n===   "+tag+"   ===\n" + "="*20 + "\nn="+str(n)+", k="+str(k)+", q="+str(q)+", p="+str(p)+", sigma="+str(sigma)+", hs="+str(hs)+", hr="+str(hr)+"\n"+"=" * 20)

    ############
    # Meet-LWE #
    ############
    if run_meet_lwe == True:
        meet_lwe_cost = meet_lwe(n*k, q, min(hs, hr))
        print(f"Meet-LWE (Rep-2):  {meet_lwe_cost[0]:.1f}, mem: {meet_lwe_cost[3]:.1f}")

    #######
    # DFP #
    #######
    if run_dfp == True:  
        if sigma == 1.0625:
            error = runSmaugError(tag, n, k, q, p, T, hs, hr, {0: 382, 1: 247, 2: 67, 3: 7}, tag)
        elif sigma == 1.453713:
            error = runSmaugError(tag, n, k, q, p, T, hs, hr, {0: 562, 1: 444, 2: 218, 3: 67, 4: 13, 5: 1}, tag)

        print(f"Decryption failure prob: 2^{error:.1f}")
    
    ########
    # SIZE #
    ########
    print(f"Sizes: pk={32+n*k*math.log(q, 2)/8:.0f}, ctxt={n*(k*math.log(p, 2)+math.log(T, 2))/8:.0f}, sk={16*math.ceil(hs/16)+32} bytes")
    print("=" * 20)

# How to run:
# run_estimator = 0 (don't run), 1 (lwe only), 2 (lwr only) or 3 (both)
# run_meet_lwe = True or False
# sigma = 1.0625 (default), 1.453713 (SMAUG-192 and TiMER)

# TiMER
expSmaugParams(256, 2, 1024, 256, 8, 100, 132, sigma=1.453713, run_estimator=3, run_meet_lwe=True, tag = "TiMER")


# Smaug128
#expSmaugParams(256, 2, 1024, 256, 32, 140, 132, run_estimator=3, run_meet_lwe=True, tag = "Smaug128")

# Samug192
#expSmaugParams(256, 3, 2048, 256, 256, 198, 151, sigma=1.453713, run_estimator=3, run_meet_lwe=True, tag = "Smaug192")

# Smaug256
#expSmaugParams(256, 5, 2048, 256, 64, 176, 160, run_estimator=3, run_meet_lwe=True, tag = "Smaug256")
