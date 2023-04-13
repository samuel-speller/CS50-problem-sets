# TODO

import re
from cs50 import get_string
number = -1

# ask user for card number
while int(number) < 0:
    number = get_string('Number: ')

# luhns algorithm

# iterate over every second value in the card number, starting from the numbers second to last digit

# set sums to start at zero
sum1 = 0
sum2 = 0


for i in range(len(number) - 2, -1, -2):
    # set dig to string so we can iterate over the digit in each sum
    dig = str(2 * int(number[i]))
    sum_dig = 0
    for j in range(len(dig)):
        sum_dig = sum_dig + int(dig[j])
    sum1 = sum1 + sum_dig

# compute second sum
for i in range(len(number) - 1, -1, -2):
    sum2 = sum2 + int(number[i])

sum_tot = sum1 + sum2

if sum_tot % 10 != 0:
    print('INVALID')

else:
    # use syntax to get the first 2 numbers from 'number'
    if len(number) == 15 and (number[0:2] == '34' or number[0:2] == '37'):
        print('AMEX')

    elif len(number) == 16 and (number[0:2] == '51' or number[0:2] == '51' or number[0:2] == '53' or number[0:2] == '54' or number[0:2] == '55'):
        print('MASTERCARD')

    elif len(number) > 12 and len(number) < 17 and number[0] == '4':
        print('VISA')

    else:
        print('INVALID')