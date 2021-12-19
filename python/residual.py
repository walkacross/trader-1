#!/usr/bin/env python3

import sys
import numpy as np
import pandas as pd
from math import log2

def normalize(mat):
    normalized = np.array([100 * log2(val) for val in mat])
    return normalized

def main():
    x_tickers = [sys.argv[i] for i in range(1, len(sys.argv) - 1)]
    y_ticker = sys.argv[-1]

    x_dfs = []
    for i in range(len(x_tickers)):
        df = pd.read_csv("./data/{}.csv" .format(x_tickers[i]))
        x_dfs.append({"dates": np.array(df["dates"]), "adjusted close": np.array(df["adjusted close"])})

    df = pd.read_csv("./data/{}.csv" .format(y_ticker))
    y_df = {"dates": np.array(df["dates"]), "adjusted close": np.array(df["adjusted close"])}

    for i in range(y_df["dates"].shape[0] - 25):
        common = True
        for y_date in y_df["dates"][i:i+25]: # 0 ... 24
            for j in range(len(x_dfs)):
                common = y_date in x_dfs[j]["dates"]
                if not common:
                    break
            if not common:
                break

        if common:
            out = open("./temp/residual", "a")
            for j in range(len(x_dfs)):
                k = list(x_dfs[j]["dates"]).index(y_df["dates"][i])
               # normalize (past 25 days: 0 ... 24)
                x_norm = normalize(x_dfs[j]["adjusted close"][k:k+25])
                y_norm = normalize(y_df["adjusted close"][i:i+25])
                x_norm -= x_norm.mean()
                y_norm -= y_norm.mean()
                # compute and write residual
                residual = y_norm - x_norm
                for val in residual:
                    out.write(str(val) + " ")
                out.write("\n")
            # compute label (1-day return of y)
            y_return = (y_df["adjusted close"][i+25] - y_df["adjusted close"][i+24]) / y_df["adjusted close"][i+24]
            out.write(str(y_return) + "\n/\n")

    # save list
    with open("./models/{}/linear/pairs" .format(y_ticker), "w+") as f:
        for i in range(len(x_tickers)):
            f.write(x_tickers[i])
            if i != len(x_tickers) - 1:
                f.write("\n")


if __name__ == "__main__":
    main()
