#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//call funtions
bool onlydigits(string string);
char rotate(char c, int k);

int main(int argc, string argv[])
{
    //create error message if more than 1 command line argument is used

    if (argc < 2 || argc > 2)
    {
        printf("ERROR: You need to type 1 integer for the key:\n");

        return 1;
    }

    //create error message if incorecct charater is typed

    if (onlydigits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");

        return 1;
    }

    //convert user input into an int
    int k = atoi(argv[1]);

    //request plain text input from user
    string ptxt = get_string("plain text: ");

    //perform cipher and print

    printf("ciphertext: ");

    for (int i = 0; i < strlen(ptxt); i++)
    {
        printf("%c", rotate(ptxt[i], k));
    }
    printf("\n");

}



//funtion that will test if a string only contains numeric characters and return true if that's the case
bool onlydigits(string string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] < 48 || string[i] > 57)
        {
            return false;
        }

    }

    return true;

}

//funtion to rotate a charater by an integer
char rotate(char c, int k)
{
    char rot;

    //check for capital letter and rotate preserving case
    if (c > 64 && c < 91)
    {
        rot = ((c - 64 + k) % 26) + 64;
    }

    //check for lowercase letter and rotate preserving case
    else if (c > 96 && c < 123)
    {
        rot = ((c - 96 + k) % 26) + 96;
    }

    //do not rotate if not a letter
    else
    {
        rot = c;
    }

    return rot;

}