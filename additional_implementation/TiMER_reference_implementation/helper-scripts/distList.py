from gmpy2 import mpq, mpz
from math import log

# Distribution List P = distList(list, loc_zero) is defined by
# P(loc_zero) = Pr(0)
# P(loc_zero + i) = Pr(i) for all i
# All list members are represented by integers proportion to probability
# If loc_zero is None, then input list should be [Pr(0), Pr(1), ...]
# and distList becomes Pr(-i) = Pr(i) for all i.
# ex, P(0) = 1/2, P(-1)=P(1) = 1/4, then it can be defined as distList([2, 1], None)
class distList:
    def __init__(self, list, loc_zero=None):
        if loc_zero is None:
            self.prob = []
            for i in range(len(list)):
                self.prob.append(list[len(list) - 1 - i])
            for i in range(len(list) - 1):
                self.prob.append(list[i + 1])
            self.zero = len(list) - 1
        else:
            self.prob = list
            self.zero = loc_zero

        self.length = len(self.prob)
        self.summation = sum(self.prob)

    # addition
    # For R = P + Q, R[n] = sum_{i} P[i] * Q[n-i]
    def __add__(self, other):
        answer_list = [mpz(0) for i in range(self.length + other.length - 1)]
        answer = distList(answer_list, loc_zero=1)
        for i in range(self.length):
            for j in range(other.length):
                answer.prob[i + j] += self.prob[i] * other.prob[j]

        answer.zero = self.zero + other.zero
        answer.summation = sum(answer.prob)

        temp = self.summation * other.summation - answer.summation
        if temp != 0:
            print("error", log(mpq(temp, answer.summation),2))

        return answer

    def printDist(self):
        for i in range(self.length):
            if i is self.zero:
                print(f"  {self.prob[i]}   ", end="")
            else:
                print(self.prob[i], end=' ')
        print('')

# Identity = distribution with Pr(0) = 1
IDENTITY = distList([1])

def double(self):
    return self + self


# return bit decomposition of n
def binary(n):
    answer_list = []
    i = 0
    while(n > 1):
        answer_list.append(n % 2)
        n = (n - answer_list[i]) / 2
        i += 1
    answer_list.append(1)
    return answer_list

# return sum of n dist_list's as if repeated squaring
def multiple(dist_list, n):
    bin_list = binary(n)
    binLoc = len(bin_list) - 1

    answer = IDENTITY
    while(binLoc >= 0):
        # print(location)
        if bin_list[binLoc] == 1:
            temp = dist_list
        else:
            temp = IDENTITY
        if binLoc > 0:
            answer = double(answer + temp)
        else:
            answer += temp
        binLoc -= 1
    return answer
