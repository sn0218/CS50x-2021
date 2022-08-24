#include <stdio.h>
#include <cs50.h>

int getdigit(long n);
int checksum(long n);

int main(void)
{
    long number;
    int first_digit;
    int second_digit;
    int first2digits;
    
    //prompt the user to input the credit number
    do
    {
        number = get_long("Number: ");
    }
    while (number < 1);

    //use the customized function to check the length and Luhn sum of credit number
    int x = getdigit(number);
    int sum = checksum(number);
    
    while (number >= 10)
    {
        second_digit = number % 10;
        number = number / 10;
        first_digit = number;
    }

    first2digits = first_digit * 10 + second_digit;
    
    //check Luhn Algorithm
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else if (sum % 10 == 0)
    {
        //check if american express
        if (x == 15 && (first2digits == 34 || first2digits == 37))
        {
            printf("AMEX\n");

        }
        //check if master card
        else if (x == 16 && (first2digits >= 51 && first2digits <= 55))
        {
            printf("MASTERCARD\n");

        }
        //check if visa card
        else if ((x == 16 && first_digit == 4) || (x == 13 && first_digit == 4))
        {
            printf("VISA\n");
        }
    
        else
        {   
            printf("INVALID\n");

        }
    }
    else
    {
        printf("INVALID\n");

    }
    
}

// calculate length of credit number
int getdigit(long n)
{
    int d = 0;

    while (n != 0)
    {
        n = n / 10;
        d++;
    }
    return d;
}

//calculate the checksum
int checksum(long n)
{
    int mod1;
    int mod2;
    int sum_mod1 = 0;
    int sum_mod2 = 0;
    int total = 0;
    
    while (n > 0)
    {
        //get the last digit
        mod1 = n % 10;
        //remove the last digit
        n = n / 10;
        // get the second last digit
        mod2 = n % 10;
        //remove the second last digit
        n = n / 10;
        
        sum_mod1 += mod1;
        
        int temp = mod2 * 2;
        
        if (temp >= 10)
        {
            temp = temp / 10 + temp % 10;
            sum_mod2 += temp;
        }
        else
        {
            sum_mod2 += mod2 * 2;  
        }
        
    }
    
    return sum_mod1 + sum_mod2;

}