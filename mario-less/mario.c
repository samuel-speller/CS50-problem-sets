#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //do while loop to ask for height until height is between 1 and 8

    int height;
    do
    {
        height = get_int("What's the pyramid's height? \n");
    }
    while (height > 8 || height < 1);

    //add a line break to space it out and make it look nicer
    printf("\n");

    //for loop to print pyramid

    //set up the number of rows relating to the user choice of 'height'
    for (int i = 0; i < height; i++)
    {
        //print a number of spaces to push the # to the right
        for (int k = 0; k < height - i - 1; k++)
        {
            printf(" ");
        }
        //print a number of # on each row
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        //next row
        printf("\n");
    }

}