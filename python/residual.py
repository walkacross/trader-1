#!/usr/bin/env python3

import sys
import pandas as pd

x_tickers = [sys.argv[i] for i in range(len(sys.argv) - 1)]
y_ticker = sys.argv[-1]

x_dfs = []
for i in range(len(x_tickers)):
    df = pd.read_csv("./data/{}.csv" .format(x_tickers[i])
    x_dfs.append({"dates": list(df["dates"]), "adjusted close": list(df["adjusted close"])})

df = pd.read_csv("./data/{}.csv" .format(y_ticker))
y_df = {"dates": list(df["dates"]), "adjusted close": list(df["adjusted close"])}

residuals = [[] for i in range(len(x_tickers))]
for i in range(len(y_df["dates"])):
    for k in range(len(x_dfs)):
        common = y_df["dates"][i] in x_dfs[k]["dates"]
        if not common:
            break
     
