#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool alphacheck(char c);
bool fullalphabet(string s);



int main(int argc, string argv[])
{
    //define key
    string key = argv[1];

    //error message if user inputs no command line arguments or more than 1 command line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");

        return 1;
    }

    //error message if user inputs cipher key of incorrect length (26 letters)
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");

        return 1;
    }

    //error message if user inputs anything other than letters
    for (int i = 0; i < strlen(key); i++)
    {
        if (alphacheck(key[i]) != true)
        {
            printf("Please input a key containing 26 letters.\n");

            return 1;
        }
    }

    //error message if all 26 letters of the alphabet aren't used
    if (fullalphabet(key) == false)
    {
        printf("please use each letter of the alphabet once.\n");

        return 1;
    }

    //prompt user for text to encrypt

    string ptxt = get_string("plaintext: ");

    //perform cipher

    int keyplace;
    char cipherc;


    //iterate over the text
    for (int i = 0; i < strlen(ptxt); i++)
    {
        //for upper case letters
        if (ptxt[i] > 64 && ptxt[i] < 91)
        {
            keyplace = ptxt[i] - 65;
            cipherc = toupper(key[keyplace]);
            ptxt[i] = cipherc;
        }

        //for lower case letters
        else if (ptxt[i] > 96 && ptxt[i] < 123)
        {
            keyplace = ptxt[i] - 97;
            cipherc = tolower(key[keyplace]);
            ptxt[i] = cipherc;
        }
    }

    //output ciphertext

    printf("ciphertext: %s\n", ptxt);

}





//CUSTOM FUNCTIONS//

//function to check if a charater is a letter
bool alphacheck(char c)
{
    if (c > 64 && c < 91)
    {
        return true;
    }

    else if (c > 96 && c < 123)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//function to test that 26 different letters have been typed in a string of length 26 (case insensitive)
bool fullalphabet(string s)
{
    char c;
    int count = 0;


    for (int i = 0; i < 26; i++)
    {

        for (int j = 0; j < 26; j++)
        {
            //convert character in key to lowercase
            c = tolower(s[j]);

            //check if the letter is an a. If it is add to count. If there is another a in the string then count will be greater than 1. Iterate this method over the entire alphabet using the i loop.
            if (c == i + 97)
            {
                count = count + 1;
            }

        }

        if (count != 1)
        {
            return false;
        }

        //return count to 0 after each pass through the string
        count = 0;
    }

    return true;

}
