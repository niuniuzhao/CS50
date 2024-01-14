#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // prompt user for credit card number w get_long
    long cn;
    do
    {
        cn = get_long("Credit card number: ");
    }
    while (cn < 0 || cn > 9999999999999999);

    // checksum
    // the second-to-last digit is ((cn % 100) / 10), the fourth-to-last digit is ((cn % 10000) / 1000)
    // and so on
    // manual labour solution -.-

    // get all the even-place digits, time them by 2
    int a = 2 * ((cn % 100) / 10);
    int b = 2 * ((cn % 10000) / 1000);
    int c = 2 * ((cn % 1000000) / 100000);
    int d = 2 * ((cn % 100000000) / 10000000);
    int e = 2 * ((cn % 10000000000) / 1000000000);
    int f = 2 * ((cn % 1000000000000) / 100000000000);
    int g = 2 * ((cn % 100000000000000) / 10000000000000);
    int h = 2 * ((cn % 10000000000000000) / 1000000000000000);
    // break them into digits again, sum the digits
    int sum2 = a % 10 + a / 10 + b % 10 + b / 10 + c % 10 + c / 10 + d % 10 + d / 10 + e % 10 + e / 10 + f % 10 + f / 10 + g % 10 +
               g / 10 + h % 10 + h / 10;
    // sum all the odd-place digits
    int sum1 = (cn % 10) + ((cn % 1000) / 100) + ((cn % 100000) / 10000) + ((cn % 10000000) / 1000000) +
               ((cn % 1000000000) / 100000000) + ((cn % 100000000000) / 10000000000) + ((cn % 10000000000000) / 1000000000000) +
               ((cn % 1000000000000000) / 100000000000000);
    // sum both, and check if last place is 0
    int zero = (sum2 + sum1) % 10;

    // output: AMEX\n or MASTERCARD\n or VISA\n or INVALID\n
    // American Express uses 15-digit numbers; start with 34 or 37
    // MasterCard uses 16-digit numbers; start with 51, 52, 53, 54, or 55
    // Visa uses 13- and 16-digit numbers; start with 4
    if (zero == 0 && ((cn >= 340000000000000 && cn <= 349999999999999) || (cn >= 370000000000000 && cn <= 379999999999999)))
    {
        printf("AMEX\n");
    }
    else if (zero == 0 && cn >= 5100000000000000 && cn <= 5599999999999999)
    {
        printf("MASTERCARD\n");
    }
    else if (zero == 0 && ((cn >= 4000000000000 && cn <= 4999999999999) || (cn >= 4000000000000000 && cn <= 4999999999999999)))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
