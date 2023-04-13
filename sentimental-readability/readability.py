# TODO

from cs50 import get_string


def main():
    text = get_string('Text: ')
    index = cl_index(text)

    if index < 1:
        print('Before Grade 1')

    elif index >= 16:
        print('Grade 16+')

    else:
        print(f'Grade: {round(index)}')


# define count letters function
def count_letters(text):
    letters = 0
    for i in range(len(text)):
        if text[i].isalpha() == True:
            letters += 1
    return letters


# define count words function
def count_words(text):
    words = 0
    for i in range(len(text)):
        if text[i] == ' ':
            words += 1
    return (words + 1)


# define count sentences function
def count_sentences(text):
    sentences = 0
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            sentences += 1
    return sentences


# define c l index
def cl_index(text):
    # average number of letters per 100 words in text
    L = (count_letters(text)/count_words(text)) * 100

    # average number of sentences per 100 words in text
    S = (count_sentences(text)/count_words(text)) * 100

    index = (0.0588 * L) - (0.296 * S) - 15.8

    return index


# call main
if __name__ == "__main__":
    main()