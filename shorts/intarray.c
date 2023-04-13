#include <stdio.h>

int main(void)
{
    int i = 0;
    int intarray[100];

    while (i < 100)
    {
        intarray[i] = i;
        i++;
    }



    printf("%i\n", intarray[3]);

    printf("%i\n", intarray[0]);

    printf("%i\n", intarray[6]);


}