#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //ask for height
    int height;
    do
    {
        height = get_int("What height is the pyramid? ");
    }
    while (height < 1 || height > 8);

    //loop to change lines
    for (int i = 0; i < height; i++)
    {
        //print spaces to shift first pyramid
        for (int k = 0; k < height - i - 1; k++)
        {
            printf(" ");
        }

        //print first pyramid
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        //print gap between pyramids
        printf("  ");

        //print second pyramid
        for (int h = 0; h <= i; h++)
        {
            printf("#");
        }

        //move to next line
        printf("\n");
    }

}