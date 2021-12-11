#!/usr/bin/env python3

import sys
import pandas as pd

x_ticker = sys.argv[1]
y_ticker = sys.argv[2]

x_raw = pd.read_csv("./data/{}.csv" .format(x_ticker))
y_raw = pd.read_csv("./data/{}.csv" .format(y_ticker))

x = {"dates": list(x_raw["dates"]), "adjusted close": list(x_raw["adjusted close"])}
y = {"dates": list(y_raw["dates"]), "adjusted close": list(y_raw["adjusted close"])}

pair_data = []

for i in range(0, len(y["dates"]) - 1):
    if (y["dates"][i] in x["dates"]) and (y["dates"][i+1] in x["dates"]):
        k0 = x["dates"].index(y["dates"][i])
        k1 = x["dates"].index(y["dates"][i+1])
        x_return = (x["adjusted close"][k1] - x["adjusted close"][k0]) / x["adjusted close"][k0]
        y_return = (y["adjusted close"][i+1] - y["adjusted close"][i]) / y["adjusted close"][i]
        pair_data.append("{} {} " .format(x_return, y_return))

out = open("./temp/pair", "w+")

if len(pair_data) >= len(y["adjusted close"]) * 0.70:
    for i in range(len(pair_data)):
        out.write(pair_data[i])
        if i != len(pair_data) - 1:
            out.write("\n")
else:
    out.writeline("999 999 ")

