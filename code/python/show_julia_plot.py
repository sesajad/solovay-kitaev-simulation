import matplotlib.pyplot as plt
import numpy as np

with open('../julia/log-errors.txt', 'r') as f:
    data = [[float(v) for v in l.strip().split(',')] for l in f.readlines()]

pos = list(range(1, len(data) + 1))
means = [sum(x)/len(x) for x in data]

m,b = np.polyfit(pos, means, 1)

plt.violinplot(data, pos, showmeans=True, showextrema=True)
plt.plot(pos, [p * m + b for p in pos])
plt.ylabel("-log Ɛ*")
plt.xlabel("n")
plt.show()
