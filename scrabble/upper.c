#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int i = 0;
    char ch;
    char word[] = "hello";
    string wordup;

    //need to get the toupper function working, this might work but haven't checked
    while (word[i])
    {
        ch = word[i];
        wordup[i] = (toupper(ch));
        i++;
    }

    printf("%s/n", wordup);
}