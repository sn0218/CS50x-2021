#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);
    
    for (int i = 1; i <= h; i++)
    {
        //print the space 
        for (int j = 1; j <= h - i ; j++)
        {
            printf(" ");
        }
        //print the hash
        for (int k = 1; k <= i ; k++)
        {
            printf("#");
        }
        //print the gap
        for (int m = 0; m < 1 ; m++)
        {
            printf("  ");
        }
        
        //print the hash
        for (int n = 1; n <= i ; n++)
        {
            printf("#");
        }
        printf("\n");

    }
}