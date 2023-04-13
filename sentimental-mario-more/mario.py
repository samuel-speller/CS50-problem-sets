# TODO

from cs50 import get_int

# define height outside of the range so we start the while loop
height = -1

while height > 8 or height < 1:
    # prompt user for input
    height = get_int('Height: ')

# print pyramid
for i in range(height):
    print(' '*(height-i-1) + '#'*(i+1) + '  ' + '#'*(i+1))