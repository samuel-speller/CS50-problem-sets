#import what i need
import random
import sys
from pyfiglet import Figlet
figlet = Figlet()

# exit if the user hasn't entered the correct number of command line arguments

# define the legnth of argv as a variable as we will be using it a few times
l = len(sys.argv)

# set a variable for the list of fonts
fontlist = figlet.getFonts()

if l == 1:
    # get user input
    text = input('text to convert: ')

    # get a random font from the font list
    random_font = random.choice(fontlist)

    # set font
    figlet.setFont(font=random_font)

    # print text in the random font
    print(figlet.renderText(text))

elif l == 3:
    if sys.argv[2] in fontlist and (sys.argv[1] == '-f' or sys.argv[1] ==
                                    '--font'):
        # get user input
        text = input('text to convert: ')

        # print text in font specified in argv
        figlet.setFont(font=sys.argv[2])
        print(figlet.renderText(text))
    else:
        print('specify font using -f or --font followed by the name of the \
              desired font')
        sys.exit(1)
else:
    print('use no command line arguments for a random font or specify font \s
           using -f or --font followed by the name of the desired font')
    sys.exit(1)