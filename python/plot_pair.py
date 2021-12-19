#!/usr/bin/env python3

import sys
import numpy as np
import matplotlib.pyplot as plt

x_ticker = sys.argv[1]
y_ticker = sys.argv[2]
slope = float(sys.argv[3])
bias = float(sys.argv[4])

x, y = [], []
with open("./temp/pair", "r") as f:
    for line in f.readlines():
        pair = [float(val) for val in line.split(" ")[:-1]]
        x.append(pair[0])
        y.append(pair[1])

yhat = [slope * k + bias for k in np.arange(min(x) * 0.90, max(x) * 1.1, 0.001)]

plt.title("{}-{}" .format(y_ticker, x_ticker))
plt.scatter(x, y, 2)
plt.scatter(np.arange(min(x) * 0.90, max(x) * 1.1, 0.001), yhat, 2, color="red")
plt.savefig("./models/{}/linear/{}.png" .format(y_ticker, x_ticker))
