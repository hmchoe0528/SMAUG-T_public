from pandas import read_excel
from math import log
from gmpy2 import mpq, mpz
from RLizard import prob_s, prob_er

# Parameters of RLizard
class RLizardv2ParameterSet:
    def __init__(self, Data):
        self.n = Data["n"]
        self.hs = Data["hs"]
        self.hr = Data["hr"]
        self.p = Data["p"]
        self.p2 = Data["p2"]
        self.q = Data["q"]
        self.label = "q" + str(self.q)

# Run
def runRLizardv2Error(q):
    data_name = "q"+str(q)
    # Read Excel file ( /inputs.xlsx)
    prob_read = read_excel('inputs.xlsx', sheet_name="R2_distribution")
    const_read = read_excel('inputs.xlsx', sheet_name="R2_parameter")

    # Set Parameters
    LPS = RLizardv2ParameterSet(Data=const_read[data_name])


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
    bound = LPS.q / 4 - LPS.q / (2 * LPS.p2)
    sum_over_bound = sum(final_dist.prob[:final_dist.zero - bound]) + sum(final_dist.prob[final_dist.zero + bound:])
    answer = mpq(sum_over_bound, final_dist.summation)

    # Output = log_2 (answer)
    print("===== RLizard2_" + str(LPS.q) + " =====")
    print("n = " + str(LPS.n))
    print("h_s = " + str(LPS.hs) + ", h_r = " + str(LPS.hr))
    print("p = " + str(LPS.p) + ", p2 = " + str(LPS.p2) + ", q = " + str(LPS.q))
    print("Error(log2) : " + str(log(float(answer), 2)))
    print("===========\n")