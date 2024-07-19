from nd import NoiseDistribution, stddevf
from lwe_parameters import LWEParameters
from lwe import estimate
from reduction import *
import math
from Meet_LWE import meet_lwe
#from smaug_dfp_compression import runSmaugError
from Smaug_failure_timer import prob_final_error

# run_estimator = 0 (don't run), 1 (lwe only), 2 (lwr only) or 3 (both)
def expSmaugParams(n, k, q, p, T, hs, numCBD = 2, sigma = 1.0625, isTimer = False, run_lwe = False, run_lwr = False, run_meet_lwe = True, run_dfp = True, tag = "SmaugExp"):
    #print("\nRunning: n="+str(n)+", k="+str(k)+", q="+str(q)+", p="+str(p)+", p'="+str(T)+", sigma="+str(sigma)+", hs="+str(hs)+", numCBD="+str(numCBD)[0]+("/"+str(numCBD)[1] if len(str(numCBD))>1 else ""))
    if run_lwe==False:
        if run_lwr==False:
            run_estimator = 0
        else:
            run_estimator = 2
    else:
        if run_lwr==False:
            run_estimator = 1
        else:
            run_estimator = 3

    ############
    # Core-SVP #
    ############
    if run_estimator > 0:
        red_cost_models = [ADPS16("classical")]
        model_names     = ["ADPS16_classical"]
        list_attack = ['usvp', 'bdd', 'bdd_hybrid', 'bdd_mitm_hybrid', 'dual', 'dual_hybrid']

        #red_cost_models = [MATZOV("classical"), ADPS16("classical"), ADPS16("quantum")]
        #model_names     = ["MATZOV_classical", "ADPS16_classical", " ADPS16_quantum "]
        #list_attack = ['arora-gb', 'bkw', 'usvp', 'bdd', 'bdd_hybrid', 'bdd_mitm_hybrid', 'dual', 'dual_hybrid']
        
        Smaug_LWE = LWEParameters(n=n*k,
                q=q,
                Xs=NoiseDistribution.SparseTernary(n*k, int(hs / 2), int(hs / 2)),
                Xe=NoiseDistribution.DiscreteGaussian(sigma),
                m=n*k,
                tag="LWE",
                )
        
        Smaug_LWR = LWEParameters(n=n*k,
                q=q,
                Xs=NoiseDistribution.ModifiedCBD(numCBD),
                Xe=NoiseDistribution.UniformMod(int(q / p)),
                m=n*(k+1),
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
        
        print("\n" + "="*20 + "\n===   "+tag+"   ===\n" + "="*20 + "\nn="+str(n)+", k="+str(k)+", q="+str(q)+", p="+str(p)+", p'="+str(T)+", sigma="+str(sigma)+", hs="+str(hs)+", numCBD="+str(numCBD)+"\n"+"=" * 20)
        idx = 0
        print(f"Core-SVP hardness:")
        for model_name in model_names:
            print(f"({model_name}) {core_svp_list[idx]:.1f}")
            idx = idx + 1
    
    if run_estimator ==0:
        print("\n" + "="*20 + "\n===   "+tag+"   ===\n" + "="*20 + "\nn="+str(n)+", k="+str(k)+", q="+str(q)+", p="+str(p)+", p'="+str(T)+", sigma="+str(sigma)+", hs="+str(hs)+", numCBD="+str(numCBD)[0]+("/"+str(numCBD)[1] if len(str(numCBD))>1 else ""))

    ############
    # Meet-LWE #
    ############
    if run_meet_lwe == True:
        meet_lwe_cost = meet_lwe(n*k, q, hs)
        print(f"Meet-LWE (Rep-2):  {meet_lwe_cost[0]:.1f}, mem: {meet_lwe_cost[3]:.1f}")

    #######
    # DFP #
    #######
    if run_dfp == True:  
        if sigma == 1.0625:
            # 1.06 internally maps to "1.0625": {0:384, 1: 247, 2: 65, 3: 7}
            sig = 1.06
        elif sigma == 1.453713:
            # 1.45 internally maps to "1.453713": {0: 562, 1:444, 2:218, 3: 67, 4:13, 5: 2}
            sig = 1.45
        else:
            print("Currently only sigma = 1.0625 or 1.453713 are supported")
            
        error = prob_final_error(sig, n, k, q, p, T, numCBD, hs, isTimer)
        
        print(f"Decryption failure prob: 2^{error:.1f}")
    
    ########
    # SIZE #
    ########
    print(f"Sizes: pk={32+n*k*math.log(q, 2)/8:.0f}, ctxt={n*(k*math.log(p, 2)+math.log(T, 2))/8:.0f}, sk={16*math.ceil(hs/16)+32} bytes (may not be the case)")
    print("=" * 20)

print("Warning: The size of secret key may different a bit")
print("Warning: The lattice estimator may run for three cost models: MATZOV, ADPS16, and ADPS16_quantum. ")
print("It may take some time. You can turn off the lattice-estimator by setting run_lwe and run_lwr to be False.")
print("Warning: The DFP computation only works for some sigma, e.g., 1.0625 or 1.453713.")
print("(Modified)CBD is used: \n CBD(1)  = {0:4/8, 1:1/4, -1:1/4} \nmCBD(58) = {0:5/8, -1:3/16, 1:3/16} (previously -1) \nmCBD(68)  = {0:6/8, -1:1/8, 1:1/8} (previously 0)")

#########
# TiMER #
#########
# Round 2 param
#expSmaugParams(256, 2, 1024, 256, 8, hs=100, hr=132, sigma=1.0625, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "TiMER")

# Refined Params
#expSmaugParams(256, 2, 1024, 256, 8, 140, numCBD= 68, sigma=1.0625, isTimer=True, run_lwe=True, run_lwr=True, run_meet_lwe=True, run_dfp=True, tag = "TiMERNew1")
#expSmaugParams(256, 2, 1024, 256, 8, 120, numCBD=  1, sigma=1.2, isTimer=True, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "TiMERNew2")
#expSmaugParams(256, 2, 1024, 256, 8, 114, numCBD= 58, sigma=1.453713, isTimer=True, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "TiMERNew3")

############
# Smaug128 #
############
# Round 2 param
#expSmaugParams(256, 2, 1024, 256, 32, hs=140, hr=132, run_estimator=0, run_meet_lwe=False, tag = "Smaug128")

# Refined Params
#expSmaugParams(256, 2, 1024, 256, 32, 140, numCBD=68, sigma=1.0625, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "Smaug128New1")
#expSmaugParams(256, 2, 1024, 256, 64, 140, numCBD=68, sigma=1.0625, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "Smaug128New2")

############
# Smaug192 #
############
# Round 2 param
#expSmaugParams(256, 3, 2l_error048, 256, 256, hs=198, hr=151, sigma=1.453713, run_estimator=0, run_meet_lwe=False, tag = "Smaug192")

# Refined Params
#expSmaugParams(256, 3, 2048, 512, 16, 264, numCBD=58, sigma=1.0625, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "Smaug192New1")
#expSmaugParams(256, 3, 2048, 512, 16, 264, numCBD=1, sigma=1.0625, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "Smaug192New2")

############
# Smaug256 #
############
# Round 2 param
#expSmaugParams(256, 5, 2048, 256, 64, hs=176, hr=160, run_estimator=0, run_meet_lwe=False, tag = "Smaug256")

# Refined Params
expSmaugParams(256, 4, 2048, 512, 128, 348, numCBD=58, sigma=1.0625, run_lwe=True, run_lwr=True, run_meet_lwe=False, run_dfp=False, tag = "Smaug256New")

