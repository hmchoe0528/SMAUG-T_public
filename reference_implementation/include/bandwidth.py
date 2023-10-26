n = 256
rank = [2, 3, 4]

ctpoly = 1 * n

for k in rank:
    ctpoly_vec = ctpoly * k
    print("For rank {}, ciphertext_bytes = {}".format(k, ctpoly_vec + ctpoly))
