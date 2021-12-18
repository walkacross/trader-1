
from alpaca_trade_api.rest import REST, TimeFrame

api = REST()

df = api.get_bars("AAPL", TimeFrame.Day, "2021-06-08", "2021-06-08", adjustment='raw').df

print(df)
