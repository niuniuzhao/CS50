#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool is_valid(string s);
void convert(string s, string key);

int main(int argc, string argv[])
{
    // Accept one and only one command-line argument
    if (argc != 2)
    {
        printf("ENTER KEY WITH 26 UNIQUE CHARACTERS\n");
        return 1;
    }

    // Make sure argv[1] is valid
    if (is_valid(argv[1]) == false)
    {
        printf("ENTER KEY WITH 26 UNIQUE CHARACTERS\n");
        return 1;
    }

    // prompt the user for a string of plaintext
    string plaintext = get_string("plaintext: ");

    // convert KEY to all UPPERCASE
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        argv[1][i] = toupper(argv[1][i]);
    }

    // print 'ciphertext'
    printf("ciphertext: ");
    // print ciphered text
    convert(plaintext, argv[1]);

    // exit
    return 0;
}

bool is_valid(string s)
{
    // the key is invalid if

    // not containing 26 characters
    if (strlen(s) != 26)
    {
        return false;
    }
    // containing any character that is not an alphabetic character
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isalpha(s[i]))
        {
            return false;
        }
    }
    // not containing each letter exactly once
    for (int i = 0, n = strlen(s); i < n - 1; i++)
    {
        // check all characters to the right of the current character for repetition
        for (int j = i + 1; j < n; j++)
        {
            if (s[j] == s[i])
            {
                return false;
            }
        }
    }
    // if all tests passed
    return true;
}

void convert(string s, string key)
{
    // go through each character and substitute with the corresponding character in KEY
    // for each uppercase letter, get position of letter in the alphabet - look at corresponding position in the key - GRAB
    // for each lowercase letter, get position of letter in the alphabet - look at corresponding position in the key - GRAB - change
    // to lowercase
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // in the case of uppercase characters
        if (isupper(s[i]))
        {
            // since we converted KEY to all UPPERCASE,
            // we calculate the character in at nth position of the alphabet -->
            // go to nth position in the KEY sequence and grab whatever character is there
            char ciphered = key[s[i] - 'A'];
            // PRINTTTTT
            printf("%c", ciphered);
        }
        // in the case of lowercase characters
        else if (islower(s[i]))
        {
            // similarly we calculate the character in at nth position of the alphabet -->
            // go to nth position in the KEY sequence and grab whatever character is there
            char ciphered = key[s[i] - 'a'];
            // since we converted KEY to all UPPERCASE, we need to convert this character back to lowercase
            ciphered = ciphered + 32;
            // PRIIIIINT
            printf("%c", ciphered);
        }
        // in the case of non-characters
        else
        {
            // don't do anything
            printf("%c", s[i]);
        }
    }
    // new line

    printf("\n");
}
