from math import log
from gmpy2 import mpq, mpz
from RLizard import prob_s, prob_er

# ver = 'RLizard1'
# # RLizard1_n1024_q1024_p256_hs128_hr128
# n = 1024
# hs = 128
# hr = 128
# p = 256
# p2 = p
# q = 1024
# prob_read = {0: 382, 1: 247, 2: 67, 3: 7}

# ver = 'RLizard2'
# RLizard2_n512_q1024_p256_hs168_hr172
# n = 512
# hs = 168
# hr = 172
# p = 256
# p2 = 32
# q = 1024
# prob_read = {0: 382, 1: 247, 2: 67, 3: 7}

# Run
def runSmaugError(ver, n, k, q, p, hs, hr, prob_read):
    """ Calculate the failure probability of Smaug
    :param n: ring dimension
    :param k: dimension over ring
    :param q: modulus q = 1024 in smaug
    :param p: modulus p = 256 in smaug
    :param p2: equal to p in smaug
    :param prob_read: error distribution with CDF
    """
    p2 = p
    hs = int(hs / k)
    hr = int(hr / k)

    data_name = ver
    # data_name = str(ver) + "_n" + str(n) + "_k" + str(k) +"_q" + str(q) + "_p" + str(p) + "_hs" + str(hs*k) + "_hr" + str(hr*k)

    prob_given = []
    for i in range(0, 4, 1):
        temp = prob_read[i]
        if temp >= 0:
            prob_given.append(mpz(temp))

    # Calculate <s,f>
    s_hs = prob_s(hs)

    # Calculate <e,r>
    er_hr = prob_er(hr, prob_given)

    # sum_dist = <e,r> + <s,f>
    sum_dist = s_hs + er_hr

    # final_dist, considering k
    final_dist = sum_dist
    for i in range(k-1):
        final_dist = final_dist + sum_dist

    # Calculate probability
    bound = int(q / 4 - q / (2 * p2))
    sum_over_bound = sum(final_dist.prob[:final_dist.zero - bound]) + sum(final_dist.prob[final_dist.zero + bound:])
    answer = mpq(sum_over_bound, final_dist.summation)

    return str(log(float(answer), 2))

    # Output = log_2 (probability)
    # print("==== " + data_name + " ====")
    # print("n = " + str(n) + "*"+str(k)+", h_s = " + str(hs*k) + ", h_r = " + str(hr*k))
    # print("h_r = " + str(hr*k)+", h_s = " + str(hs*k)+",,,,"+str(log(float(answer), 2)))
    # print("Error(log2) : " + str(log(float(answer), 2)) + "\n")
    # print(str(log(float(answer), 2)))

