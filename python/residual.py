#!/usr/bin/env python3

import os, sys
import numpy as np
import pandas as pd
from math import log2, exp

def normalize(mat):
    normalized = np.array([100 * log2(val) for val in mat])
    return normalized

def sigmoid(x):
    return 1 / (1 + exp(-x))

def main():
    x_tickers = [sys.argv[i] for i in range(1, len(sys.argv) - 1)]
    y_ticker = sys.argv[-1]

    x_dfs = []
    for i in range(len(x_tickers)):
        df = pd.read_csv("./data/{}.csv" .format(x_tickers[i]))
        x_dfs.append({"dates": np.array(df["dates"]), "adjusted close": np.array(df["adjusted close"])})

    df = pd.read_csv("./data/{}.csv" .format(y_ticker))
    y_df = {"dates": np.array(df["dates"]), "adjusted close": np.array(df["adjusted close"])}

    os.system("rm -rf ./temp/residual")
    os.system("rm -rf ./temp/y_out")

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
            residual_out = open("./temp/residual", "a")
            for j in range(len(x_dfs)):
                k = list(x_dfs[j]["dates"]).index(y_df["dates"][i])
                # normalize (past 20 days: 0 ... 24)
                x_norm = normalize(x_dfs[j]["adjusted close"][k:k+25])
                y_norm = normalize(y_df["adjusted close"][i:i+25])
                x_norm -= x_norm.mean() # standardize
                y_norm -= y_norm.mean() # standardize
                # compute residual
                residual = y_norm - x_norm
                # z-score normalization
                residual = (residual - residual.mean()) / np.std(residual)
                for val in residual:
                    residual_out.write(str(val) + " ")
                residual_out.write("\n")
            # compute label (buy-sell signal based on sigmoid of y's 1-day return %)
            y_out = open("./temp/y_out", "a")
            signal = sigmoid((y_df["adjusted close"][i+25] - y_df["adjusted close"][i+24]) * 100 / y_df["adjusted close"][i+24])
            y_out.write(str(signal) + " \n")

    # save list
    with open("./models/{}/linear/pairs" .format(y_ticker), "w+") as f:
        for i in range(len(x_tickers)):
            f.write(x_tickers[i])
            if i != len(x_tickers) - 1:
                f.write("\n")


if __name__ == "__main__":
    main()
