from cs50 import get_int


def main():
    height = get_height()
    # loop through every row
    for i in range(height):
        # print desired number of spaces and hashes in relation to i
        # nb i starts at 0, but pyramid should start at 1
        print(" " * (height - i - 1) + "#" * (i + 1))


def get_height():
    # reprompt until we have desired n
    while True:
        n = get_int("Height: ")
        if 1 <= n <= 8:
            return n


main()
