# TODO

from cs50 import get_float

change = -1.00

while change < 0:
    change = get_float('Change owed: ')

coin_types = [0.25, 0.1, 0.05, 0.01]
coins = 0

for i in coin_types:
    if change >= i:
        coin = int(change / i)
        coins = coins + coin
        change = change - (coin * i)

        # need to round the change value to the nearest 2dp due to floating point imprecision
        change = round(change, 2)

print(int(coins))