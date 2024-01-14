import csv
import sys


def main():
    # TODO: check that there are 2 command-line arguments
    if len(sys.argv) != 3:
        sys.exit("Usage: dna.py CSV TXT")

    # TODO: import rows from csv (database) into a LIST of DICTs
    # create a LIST to store data
    persons = []

    with open(sys.argv[1]) as database:
        reader = csv.DictReader(database)
        # put each row (of csv file) into persons list
        for row in reader:
            persons.append(row)
        # store the fieldnames i.e. column titles for later use
        fields = reader.fieldnames

    # TODO: read txt (DNA sequence) file
    with open(sys.argv[2]) as sequence:
        dna = sequence.read()

    # TODO: find & store the longest run of each STR in a given DNA sequence i.e. txt
    # create a DICT
    matches = {}
    # loop through each STR but ignore "name", so start from index 1 i.e. second item
    for str in fields[1:]:
        # store result from running longest_match() in the corresponding value spot
        matches[str] = longest_match(dna, str)

    # TODO: check database for matching profiles
    # set overall status for database of all persons
    found = False
    # loop through each person in csv database
    for person in persons:
        # assume match until proven otherwise
        match = True
        # loop through each STR
        for str in fields[1:]:
            # if the longest run of current str doesn't match current person's data
            if int(matches[str]) != int(person[str]):
                match = False
                # go to next person
                break
        # if current person "passed" the match chek
        if match:
            # print current person
            print(person["name"])
            # change overall status for database of all persons
            found = True

    # after checking all persons
    if not found:
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
