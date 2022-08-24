#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sen(string text);

int main(void)
{
    // get user input of the text
    string text = get_string("Text: ");

    // count the letters
    int no_of_letter = count_letters(text);

    // count the words
    int no_of_word = count_words(text);

    // count the sentences
    int no_of_sen = count_sen(text);

    float index, L, S;

    L = (no_of_letter / (float)no_of_word) * 100;
    S = (no_of_sen / (float)no_of_word) * 100;

    // compute the readability index and grade
    index = 0.0588 * L - 0.296 * S - 15.8;

    int grade = round(index);

    // print the grade
    if (grade < 2)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 2 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        return 1;
    }


}

// compute the no. of letters
int count_letters(string text)
{
    int sum = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (islower(text[i]) || isupper(text[i]))
        {
            sum++;
        }
    }

    return sum;
}

// compute the no. of words
int count_words(string text)
{
    int sum = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            sum++;
        }

    }

    // +1 for the last word in the sentence
    return sum + 1;

}

// compute the no. of sentences
int count_sen(string text)
{
    int sum = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sum++;
        }
    }

    return sum;

}