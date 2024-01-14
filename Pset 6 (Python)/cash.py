from cs50 import get_float


def main():
    cents = ask()

    # calculate how many quarters you should give customer
    quarters = calculate_quarters(cents)
    # subtract the value of those quarters from cents
    cents = cents - (quarters * 25)

    dimes = calculate_dimes(cents)
    cents = cents - (dimes * 10)

    nickels = calculate_nickels(cents)
    cents = cents - (nickels * 5)

    pennies = cents

    # sum the number of quarters, dimes, nickels, and pennies used, round it and print
    print(round(quarters + dimes + nickels + pennies))


def ask():
    # prompt the user for change owed IN DOLLARS
    while True:
        change = get_float("Cents: ")
        if change > 0:
            # return IN CENTS
            return change * 100


def calculate_quarters(cents):
    # initialise value
    quarters = 0
    # trade 1 quarter for 25 cents
    while cents >= 25:
        quarters += 1
        cents = cents - 25
    return quarters


def calculate_dimes(cents):
    dimes = 0
    while cents >= 10:
        dimes += 1
        cents = cents - 10
    return dimes


def calculate_nickels(cents):
    nickels = 0
    while cents >= 5:
        nickels += 1
        cents = cents - 5
    return nickels


main()
