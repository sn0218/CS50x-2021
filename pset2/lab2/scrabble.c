#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int score = 0;
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    
    // check each character in word
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // compare each lowercase character in word with arr
        if (islower(word[i]))
        {
            for (int j = 0, m = 26; j < m ; j++)
            {
                if (word[i] == arr[j])
                {
                    score = score + POINTS[j];
                }
            }
        }
        
        // convert uppercase letter to lowercase letter 
        // compare each character in word with arr
        else if (isupper(word[i]))
        {
            char letter;
            
            letter = tolower(word[i]);
            
            for (int k = 0, m = 26; k < m ; k++)
            {
                if (letter == arr[k])
                {
                    score = score + POINTS[k];
                }
            }

        }
        
        // ignore non-letter character
        else
        {
            score = score + 0;
        }
    }

    return score;
}