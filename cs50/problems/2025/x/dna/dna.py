import csv
from sys import argv


def main():

    # TODO: Check for command-line usage
    if(len(argv) != 3):
         print("usage: python dna.py [database] [sequences]")
         return 0

    # TODO: Read database file into a variable
    database_path = argv[1]

    dna_database = []
    with open(database_path) as file:
        reader = csv.DictReader(file)
        for row in reader:
            dna_database.append(row)

    # TODO: Read DNA sequence file into a variable
    sequences_path = argv[2]
    sequence = ''

    with open(sequences_path) as file:
        sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    subsequences = {
        "AGATC": 0,
        "TTTTTTCT": 0,
        "AATG": 0,
        "TCTAG": 0,
        "GATA": 0,
        "TATC": 0,
        "GAAA": 0,
        "TCTG": 0
    }

    for subsequence in subsequences:
        longest = longest_match(sequence, subsequence)
        subsequences[subsequence] = longest

    # TODO: Check database for matching profiles
    person = {}
    for p in dna_database:
        found = True
        keys = iter(p)
        next(keys)
        for key in keys:
            if(int(p[key]) != subsequences[key]):
                found = False
                break
        if(not found): continue
        person = p

    print(person.get("name") or "No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence"""
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
