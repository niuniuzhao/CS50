#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);
char rotate(char c, int n);

int main(int argc, string argv[])
{
    // Make sure program was run with just one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Make sure every character in argv[1] is a digit
    if (only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert argv[1] from a 'string' to an 'int'
    int key = atoi(argv[1]);

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // Print 'ciphertext'
    printf("ciphertext: ");

    // Print ciphertext by applying rotate to each character
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char rotated = rotate(plaintext[i], key);
        printf("%c", rotated);
    }
    // Go to next line
    printf("\n");
}

bool only_digits(string s)
{
    // Make sure every character in argv[1] is a digit
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

char rotate(char c, int n)
{
    // Rotate each character (only if it's an alphabetical character) via key
    // Formula: ciphertext = (plaintext + key) % 26
    // In the case of UPPERCASE
    if (isupper(c))
    {
        c = (c - 'A' + n) % 26 + 'A';
        return c;
    }
    // In the case of lowercase
    else if (islower(c))
    {
        c = (c - 'a' + n) % 26 + 'a';
        return c;
    }
    // In the case of not a character
    else
    {
        return c;
    }
}