"""
This is not used anymore,
after using the modified CBD and refined parameters.
"""
# new version with pk and c2 compression
from math import log
from gmpy2 import mpq, mpz
from pandas import read_excel
from gmpy2 import mpq, mpz
from distList import distList, multiple

# Give distribution of <e, s>,
# where s is ternary and has a hamming weight h,
# and e has a symmetric probability w.r.t. center. 
def prob_es(h, prob_given):
    temp = distList(prob_given)
    return multiple(temp, h)

# Run
def runSmaugError(ver, n, k, q, p, T, hs, hr, prob_read):
    """ Calculate the failure probability of Smaug
    :param n: ring dimension
    :param k: dimension over ring
    :param q: modulus q
    :param p: modulus p
    :param prob_read: error distribution with CDF
    """
    data_name = ver
    data_name = str(ver) + "_n" + str(n) + "_k" + str(k) +"_q" + str(q) + "_p" + str(p) + "_T" + str(T) + "_hs" + str(hs) + "_hr" + str(hr)

    prob_given = []
    for i in range(0, len(prob_read), 1):
        temp = prob_read[i]
        if temp > 0:
            prob_given.append(mpz(temp))

    # Calculate <e, r>
    er = prob_es(hr, prob_given)
    
    # Make e1_list and e2_list
    e1_list = []
    e2_list = []
    for i in range(0, int(q/2/p)):
        e1_list.append(2)
    e1_list.append(1)
    
    for i in range(0, int(q/T)):
        e2_list.append(1)

    # Calculate <e1, s>
    es = multiple(distList(e1_list), hs)
    
# This can be used for public key truncation, which is not 
# used in the current SMAUG parameter sets. 
#    # Calculate <eb, r>
#    eb_list = []
#    for i in range(0, 1):
#        eb_list.append(2)
#    eb_list.append(1)
#    ebr = multiple(distList(eb_list), hr)

    # Make e2
    e2 = distList(e2_list, int(q/2/T-1))
    
    # sum_dist = <e,r> + <e1, s> + <eb, r> + e2
    sum_dist = er + es
#    sum_dist = sum_dist + ebr
    sum_dist = sum_dist + e2

    # Calculate probability
    bound = int(q/4)
    sum_over_bound = mpz(sum((sum_dist.prob[:sum_dist.zero - bound+1])) + sum((sum_dist.prob[sum_dist.zero + bound+2:])))
    answer = mpq(sum_over_bound, sum_dist.summation)

    return log(float(answer), 2) + log(n*k, 2)


