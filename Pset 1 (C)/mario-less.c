#include <cs50.h>
#include <stdio.h>

void print_row(int spaces, int bricks);

int main(void)
{
    // Prompt the user for the pyramid's height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1);

    // Print a pyramid of that height
    // loop through every ROW
    for (int i = 1; i <= n; i++)
    {
        print_row(n - i, i);
    }
}

// function for each ROW (VOID because it's just printing)
// the number of # on each row just is the number of rows (n).
// the number of spaces on each row is the max number of # i.e. rows (n) - current number of # i.e. rows (i).
void print_row(int spaces, int bricks)
{
    // Print spaces
    for (int j = 0; j < spaces; j++)
    {
        printf(" ");
    }
    // Print bricks
    for (int k = 0; k < bricks; k++)
    {
        printf("#");
    }
    printf("\n");
}