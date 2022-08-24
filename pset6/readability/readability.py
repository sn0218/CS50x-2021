from cs50 import get_string

# get the user input of the text
text = get_string("Text: ")

# count the number of words by .split() to remove the whitespace char and return the copied string
word_count = len(text.split())

# count the number of letters by .isalpha() to check char in string that are not alphabetical letters
letter_count = sum(char.isalpha() for char in text)

# count the sentence by .count() to return the number of elements
sent_count = text.count('.') + text.count('!') + text.count('?')

L = (letter_count / word_count) * 100
S = (sent_count / word_count) * 100

# compute the Coleman-Liau index
index = 0.0588 * L - 0.296 * S - 15.8

grade = round(index)

if grade < 2:
    print("Before Grade 1")
elif 2 <= grade < 16:
    print("Grade " + str(grade))
elif grade > 16:
    print("Grade 16+")