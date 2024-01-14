from cs50 import get_string


def main():
    # prompt the user for text
    text = get_string("Text: ")

    # count the number of letters, words, and sentences in the text
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # compute the Coleman-Liau index
    L = letters / words * 100
    S = sentences / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    # round to the nearest integer
    grade = round(index)

    # print the grade level
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def count_letters(text):
    # for each alphabetical char in text, add 1
    letters = sum(1 for char in text if char.isalpha())
    return letters


def count_words(text):
    # split() splits a string into a list of words
    words = len(text.split())
    return words


def count_sentences(text):
    # for any of . ! or ? in text, add 1
    sentences = sum(1 for char in text if char in ".!?")
    return sentences


main()
