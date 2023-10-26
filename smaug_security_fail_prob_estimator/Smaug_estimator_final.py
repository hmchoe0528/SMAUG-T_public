from nd import NoiseDistribution, stddevf
from lwe_parameters import LWEParameters
from lwe import estimate
from reduction import *
import math
from smaug import runSmaugError

red_cost_models = [ADPS16("classical"), ADPS16("quantum")]
model_names = ["ADPS16_classical", "ADPS16_quantum"]

list_attack = ['arora-gb', 'bkw', 'usvp', 'bdd', 'bdd_hybrid', 'bdd_mitm_hybrid', 'dual', 'dual_hybrid']

"""parameters """
n = 256
q = 1024
sigma = 1.0625
""" Smaug128 parameters """
k128 = 2
p128 = 256
hr128 = 132
hs128 = 140
Smaug128_LWE = LWEParameters(n=256 * k128,
                             q=q,
                             Xs=NoiseDistribution.SparseTernary(256 * k128, int(hs128 / 2), int(hs128 / 2)),
                             Xe=NoiseDistribution.DiscreteGaussian(sigma),
                             m=256 * k128,
                             tag="Smaug128_LWE",
                             )
Smaug128_LWR = LWEParameters(n=256 * k128,
                             q=q,
                             Xs=NoiseDistribution.SparseTernary(256*k128, int(hr128/2), int(hr128/2)),
                             Xe=NoiseDistribution.UniformMod(int(q / p128)),
                             m=256 * k128,
                             tag="Smaug128_LWR",
                             )
""" Smaug192 parameters """
k192 = 3
p192 = 256
hr192 = 147
hs192 = 150
Smaug192_LWE = LWEParameters(n=256 * k192,
                             q=q,
                             Xs=NoiseDistribution.SparseTernary(256 * k192, int(hs192 / 2), int(hs192 / 2)),
                             Xe=NoiseDistribution.DiscreteGaussian(sigma),
                             m=256 * k192,
                             tag="Smaug192_LWE",
                             )
Smaug192_LWR = LWEParameters(n=256 * k192,
                             q=q,
                             Xs=NoiseDistribution.SparseTernary(256*k192, int(hr192/2), int(hr192/2)),
                             Xe=NoiseDistribution.UniformMod(int(q / p192)),
                             m=256 * k192,
                             tag="Smaug192_LWR",
                             )
""" Smaug256 parameters """
k256 = 5
p256 = 256
hr256 = 140
hs256 = 145
Smaug256_LWE = LWEParameters(n=256 * k256,
                             q=q,
                             Xs=NoiseDistribution.SparseTernary(256 * k256, int(hs256 / 2), int(hs256 / 2)),
                             Xe=NoiseDistribution.DiscreteGaussian(sigma),
                             m=256 * k256,
                             tag="Smaug256_LWE",
                             )
Smaug256_LWR = LWEParameters(n=256 * k256,
                             q=q,
                             Xs=NoiseDistribution.SparseTernary(256*k256, int(hr256/2), int(hr256/2)),
                             Xe=NoiseDistribution.UniformMod(int(q / p256)),
                             m=256 * k256,
                             tag="Smaug256_LWR",
                             )
core_svp_list = []

for red_cost_model, model_name in zip(red_cost_models, model_names):
    print("\n"+"=" * 30+"\n{}\n".format(model_name)+"=" * 30)

    print("\n=== Smaug128 ===\nn="+str(n)+", k="+str(k128)+", q="+str(q)+", p="+str(p128)+", sigma="+str(sigma)+", hr="+str(hr128)+", hs="+str(hs128))
    print("= Smaug128_LWE =")
    rLWE128 = estimate(Smaug128_LWE, red_cost_model=red_cost_model, jobs=16)
    print("= Smaug128_LWR =")
    rLWR128 = estimate(Smaug128_LWR, red_cost_model=red_cost_model, jobs=16)
    list_rop128 = []
    for x in list_attack:
        if x in rLWE128:
            list_rop128.append(math.log2(rLWE128[x].rop))
        if x in rLWR128:
            list_rop128.append(math.log2(rLWR128[x].rop))
    print(model_name.split("_")[1]+" CORE-SVP hardness of Smaug128: "+str(min(list_rop128)))
    core_svp_list.append(min(list_rop128))

    print("\n=== Smaug192 ===\nn="+str(n)+", k="+str(k192)+", q="+str(q)+", p="+str(p192)+", sigma="+str(sigma)+", hr="+str(hr192)+", hs="+str(hs192))
    print("= Smaug192_LWE =")
    rLWE192 = estimate(Smaug192_LWE, red_cost_model=red_cost_model, jobs=16)
    print("= Smaug192_LWR =")
    rLWR192 = estimate(Smaug192_LWR, red_cost_model=red_cost_model, jobs=16)
    list_rop192 = []
    for x in list_attack:
        if x in rLWE192:
            list_rop192.append(math.log2(rLWE192[x].rop))
        if x in rLWR192:
            list_rop192.append(math.log2(rLWR192[x].rop))
    print("===\n"+model_name.split("_")[1]+" CORE-SVP hardness of Smaug192: "+str(min(list_rop192))+"\n===")
    core_svp_list.append(min(list_rop192))

    print("\n=== Smaug256 ===\nn="+str(n)+", k="+str(k256)+", q="+str(q)+", p="+str(p256)+", sigma="+str(sigma)+", hr="+str(hr256)+", hs="+str(hs256))
    print("= Smaug256_LWE =")
    rLWE256 = estimate(Smaug256_LWE, red_cost_model=red_cost_model, jobs=16)
    print("= Smaug256_LWR =")
    rLWR256 = estimate(Smaug256_LWR, red_cost_model=red_cost_model, jobs=16)
    list_rop256 = []
    for x in list_attack:
        if x in rLWE256:
            list_rop256.append(math.log2(rLWE256[x].rop))
        if x in rLWR256:
            list_rop256.append(math.log2(rLWR256[x].rop))
    print(model_name.split("_")[1]+" CORE-SVP hardness of Smaug256: "+str(min(list_rop256)))
    core_svp_list.append(min(list_rop256))


print("\n"+"=" * 30+"\nClassical Core-SVP hardness\n"+"=" * 30)
print("Smaug128: "+str(core_svp_list[0]))
print("Smaug192: "+str(core_svp_list[1]))
print("Smaug256: "+str(core_svp_list[2]))
print("=" * 30+"\nQuantum Core-SVP hardness\n"+"=" * 30)
print("Smaug128: "+str(core_svp_list[3]))
print("Smaug192: "+str(core_svp_list[4]))
print("Smaug256: "+str(core_svp_list[5]))
print("=" * 30+"\nDecryption failure probability\n"+"=" * 30)
error128 = runSmaugError('Smaug128', 256, k128, q, p128, hs128, hr128, {0: 384, 1: 247, 2: 66, 3: 7})
error192 = runSmaugError('Smaug192', 256, k192, q, p192, hs192, hr192, {0: 384, 1: 247, 2: 66, 3: 7})
error256 = runSmaugError('Smaug256', 256, k256, q, p256, hs256, hr256, {0: 384, 1: 247, 2: 66, 3: 7})
print("Decryption failure probability for Smaug128: 2^"+str(error128))
print("Decryption failure probability for Smaug192: 2^"+str(error192))
print("Decryption failure probability for Smaug256: 2^"+str(error256))
