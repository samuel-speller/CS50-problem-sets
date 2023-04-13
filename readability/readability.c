#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


//call functions
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //request text from user
    string txt = get_string("Text: ");

    //print text back to user (remove '//' to enable)
    //printf("%s\n", txt);

    //find number of letters
    double letters = count_letters(txt);
    //printf("%i letters\n", letters);

    //find number of words
    double words = count_words(txt);
    //printf("%i words\n", words);

    //find number of sentences
    double sentences = count_sentences(txt);
    //printf("%i sentences\n", sentences);

    //putting it all together

    //average letters per 100 words
    double L = (letters / words) * 100;

    //average number of sentences per 100 words
    double S = (sentences / words) * 100;

    double index = 0.0588 * L - 0.296 * S - 15.8;
    int indexround = round(index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index > 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %i\n", indexround);
    }


}

//define functions


//funtion that counts letters in a string. It only includes uppercase and lowercase letters and excludes spaces, punctuation etc
int count_letters(string text)
{
    int letters = 0;
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        //check if a character is a letter and if so count it
        if (isalpha(text[i]) != 0)
        {
            letters++;
        }

    }

    return letters;
}

//function that counts the words in a sentence
int count_words(string text)
{
    //set words = 1 because there is always going to be 1 more word than spaces in a sentence (using the assumptions set out in the task)
    int words = 1;
    int i;

    //for loop to count the number of spaces
    for (i = 0; i < strlen(text); i++)
    {
        if (text[i] == 32)
        {
            words++;
        }
    }

    return (words);
}

//function to count sentences
int count_sentences(string text)
{
    int sentences = 0;
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            sentences++;
        }
    }

    return (sentences);
}
