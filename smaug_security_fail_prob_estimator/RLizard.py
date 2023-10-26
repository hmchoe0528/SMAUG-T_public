from pandas import read_excel
from math import log
from gmpy2 import mpq, mpz

from distList import distList, multiple

# Parameters of RLizard
class RLizardParameterSet:
    def __init__(self, Data):
        self.category = Data["category"]
        self.n = Data["n"]
        self.hs = Data["hs"]
        self.hr = Data["hr"]
        self.p = Data["p"]
        self.q = Data["q"]
        self.label = "c" + str(self.category) + "n" + str(self.n)

# Give distribution of <h, s>
def prob_s(hs):
    s_list = [1, 0, 1]
    s = distList(s_list, loc_zero=1)
    return multiple(s, hs)

# Give distribution of <e, r>
def prob_er(h, prob_given):
    temp = distList(prob_given)
    return multiple(temp, h)

# Run
def runRLizardError(category, n):
    data_name = "c" + str(category) + "n" + str(n)
    # Read Excel file ( /inputs.xlsx)
    prob_read = read_excel('inputs.xlsx', sheet_name='R_distribution')
    const_read = read_excel('inputs.xlsx', sheet_name='R_parameter')

    # Set Parameters
    LPS = RLizardParameterSet(Data=const_read[data_name])


    prob_given = []
    for i in range(len(prob_read[LPS.label])):
        temp = prob_read.loc[i, LPS.label]
        if temp >= 0:
            prob_given.append(mpz(prob_read.loc[i, LPS.label]))


    # Calculate <s,f>
    s_hs = prob_s(LPS.hs)

    # Calculate <e,r>
    er_hr = prob_er(LPS.hr, prob_given)


    # final_dist = <e,r> + <s,f>
    final_dist = s_hs + er_hr


    # Calculate answer
    bound = LPS.q / 4 - LPS.q / (2 * LPS.p)
    sum_over_bound = sum(final_dist.prob[:final_dist.zero - bound]) + sum(final_dist.prob[final_dist.zero + bound:])
    answer = mpq(sum_over_bound, final_dist.summation)

    # Output = log_2 (answer)
    print("===== CATEGORY" + str(LPS.category) + "_N" + str(LPS.n) + " =====")
    print("n = " + str(LPS.n))
    print("h_s = " + str(LPS.hs) + ", h_r = " + str(LPS.hr))
    print("p = " + str(LPS.p) + ", q = " + str(LPS.q))
    print("Error(log2) : " + str(log(float(answer), 2)))
    print("===========\n")