#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

bool check_repeat(string key);
bool check_alphabet(string key);
void encrpt(string text, string key);

int main(int argc, string argv[])
{
    // check the number of arg
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //check the length of the key
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check the validity of alphabetic key
    if (check_alphabet(argv[1]))
    {
        // check for repeated characters
        if (!check_repeat(argv[1]))
        {
            // prompt the user to input
            string plaintext = get_string("plaintext:  ");
            encrpt(plaintext, argv[1]);
            return 0;
            
        }
        else
        {
            printf("Key must not contain repeated character.\n");
            return 1;
        }
    }
    else
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }





}

// encrpt the plaintext to ciphertext
void encrpt(string text, string key)
{
    int charcount = strlen(text);
    char ctext[charcount];
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char arr2[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    
    // loop the character in the text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (islower(text[i]))
        {
            for (int j = 0, m = 26; j < m; j++)
            {
                // map the ctext to the key in lowercase
                if (text[i] == arr[j])
                {
                    ctext[i] = tolower(key[j]);
                    break;
                }
            }
        }
        else if (isupper(text[i]))
        {
            for (int k = 0, p = 26; k < p; k++)
            {
                // map the ctext to the key in uppercase
                if (text[i] == arr2[k])
                {
                    ctext[i] = toupper(key[k]);
                    break;
                }
            }
        }
        // keep the non-alphabetic letter to the same
        else
        {
            ctext[i] = text[i];
        }
    }
    
    //declaration of char ctext[charcount] which can be print as string
    printf("ciphertext: %s\n", ctext);
    
}

// check the key if contain alphabetic char
bool check_alphabet(string key)
{
    int checker = 0;

    //check no.of alphabetic char
    for (int i = 0, n = strlen(key); i < n; i++)
    {

        // check each element in key if alpabetic letter or not
        if (isupper(key[i]) || islower(key[i]))
        {
            checker++;
        }
    }

    if (checker == 26)
    {
        return true;
    }
    
    return false;
}

//check repeated characters in key
bool check_repeat(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (key[i] == key[j])
            {
                return true;
            }
        }
    }
    return false;
}
