from pandas import read_excel
from math import log
from gmpy2 import mpq, mpz

from distList import distList, multiple, binary

# Parameters of Lizard
class LizardParameterSet:
    def __init__(self, Data):
        self.category = int(Data["category"])
        self.m = Data["m"]
        self.n = Data["n"]
        self.h = Data["h"]
        self.RHO_INV = int(Data["RHO_INV"])
        self.p = Data["p"]
        self.q = Data["q"]
        self.label = "c" + str(self.category) + "n" + str(self.n)

# Give distribution of <s, f>
def prob_sf(RHO_INV, p, q, n):
    l = int(q / (2 * p))
    sf_list = [mpz(2) for i in range(2 * l + 1)]
    sf_list[0] = mpz(1)
    sf_list[2 * l] = mpz(1)
    sf_list[l] = mpz(4 * l * (RHO_INV - 1) + 2)
    sf = distList(sf_list, loc_zero=l)
    return multiple(sf, n)

# Give distribution of <e, r>
def prob_er(h, prob_given):
    temp = distList(prob_given)
    return multiple(temp, h)

# Run
def runLizardError(category, n):
    data_name = "c"+str(category)+"n"+str(n)
    prob_read = read_excel('inputs.xlsx', sheetname="distribution")
    const_read = read_excel('inputs.xlsx', sheetname="parameter")

    # Set Parameters
    LPS = LPS_c1r4 = LizardParameterSet(Data=const_read[data_name])

    prob_given = []
    # read distribution from excel file
    for i in range(len(prob_read[LPS.label])):
        temp = prob_read.loc[i, LPS.label]
        if temp >= 0:
            prob_given.append(mpz(prob_read.loc[i, LPS.label]))

    # Calculate <s,f>
    sf_n = prob_sf(LPS.RHO_INV, LPS.p, LPS.q, LPS.n)
    
    # Calculate <e,r>
    er_h = prob_er(LPS.h, prob_given)

    # final_dist = <e,r> + <s,f>
    final_dist = sf_n + er_h


    # Calculate answer
    bound = LPS.q / 4 - LPS.q / (2 * LPS.p)
    sum_over_bound = sum(final_dist.prob[:final_dist.zero - bound]) + sum(final_dist.prob[final_dist.zero + bound:])
    answer = mpq(sum_over_bound, final_dist.summation)

    # Output = log_2 (answer)
    print("===== CATEGORY" + str(LPS.category) + "_N" + str(LPS.n) + " =====")
    print("m = "+ str(LPS.m) + ", n = " + str(LPS.n))
    print("h = " + str(LPS.h) + ", rho = 1 / " + str(LPS.RHO_INV))
    print("p = " + str(LPS.p) + ", q = " + str(LPS.q))
    print("Error(log2) : " + str(log(float(answer), 2)))
    print("===========\n")