#!/usr/bin/env python3

import sys
from time import sleep
from datetime import datetime
from pandas_datareader.data import DataReader

ticker = sys.argv[1]

with open("./apikey", "r") as f:
    key = f.readline()

raw = DataReader(ticker, "av-daily-adjusted", "1900-01-01", datetime.today().strftime("%Y-%m-%d"), api_key=key)
raw.to_csv("./data/{}.csv" .format(ticker))

lines = open("./data/{}.csv" .format(ticker)).readlines()
lines[0] = "dates,open,high,low,close,adjusted close,volume,dividend amount,split coefficient\n"
out = open("./data/{}.csv" .format(ticker), "w+")
out.writelines(lines)
