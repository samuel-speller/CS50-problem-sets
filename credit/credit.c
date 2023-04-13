#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //ask for card number
    long card;
    do
    {
        card = get_long("Card Number: ");
    }
    while (card < 0);

    //count length
    int i = 0;
    long cl = card;

    while (cl > 0)
    {
        cl = cl / 10;
        i++;
    }

    //now i represents the legnth of the card number

    //check if it is a valid length (has to be 13, 15 or 16)

    if (i != 13 && i != 15 && i != 16)
    {
        printf("INVALID\n");
        return (0);
    }


    //checksum

    //sum1 is the sum of all the alternating numbers starting with the last digit
    //sum2 is the sum of all the digits of the other numbers multiplied by 2
    //d1 represets the alternating numbers starting from the last digit
    //d2 represents the alternating numbers starting from the second to last digit
    //d21 and d22 are used to seperate the two digits when multiplying d2 by 2

    long cs = card;
    int sum1 = 0;
    int sum2 = 0;
    int d1 = 0;
    int d2 = 0;
    int d21 = 0;
    int d22 = 0;
    int total = 0;

    while (cs > 0)
    {
        //find last digit
        d1 = cs % 10;

        //add to sum1
        sum1 = sum1 + d1;

        //remove last digit
        cs = cs / 10;


        //find second to last digit, now the last digit
        d2 = cs % 10;

        //multiply by 2 and add the resulting numbers digits to sum 2
        d2 = d2 * 2;
        d21 = d2 / 10;
        d22 = d2 % 10;
        sum2 = sum2 + d21 + d22;

        //remove the last digit
        cs = cs / 10;

        //now back to the start of the loop to repeat for the rest of the digits in the card number


    }

    //find total to check for luhns algorithm
    total = sum1 + sum2;

    //check last digit of total is 0
    if (total % 10 != 0)
    {
        printf("INVALID\n");
        return (0);
    }

    //now check for AMEX, mastercard or VISA

    //find the first numbers from the card number

    long first2 = card;

    while (first2 > 100)
    {
        first2 = first2 / 10;
    }

    //check card type and print if valid number

    if (first2 % 10 < 6 && first2 / 10 == 5 && first2 > 0)
    {
        printf("MASTERCARD\n");
    }

    else if (first2 / 10 == 3 && (first2 % 10 == 4 || first2 % 10 == 7))
    {
        printf("AMEX\n");
    }

    else if (first2 / 10 == 4)
    {
        printf("VISA\n");
    }

    else
    {
        printf("INVALID\n");
    }


}