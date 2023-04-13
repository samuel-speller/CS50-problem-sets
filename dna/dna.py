import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print('incorrect number of command line arguments.')
        sys.exit(1)

    # TODO: Read database file into a variable
    with open(sys.argv[1], 'r') as f_data:
        csv_reader = csv.DictReader(f_data)
        data_list = list(csv_reader)   # a list of dictionaries with each person having their own dictionary

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], 'r') as f_dna:
        dna = f_dna.read()

    # TODO: Find longest match of each STR in DNA sequence

    # Create a list of subsequences obtained from the database file
    data_list_keys = list(data_list[0].keys())

    # The first entry in the list of keys will be the name of the person so we need to omit
    # that in order to get a list of subsequences

    del data_list_keys[0]
    subsequences = data_list_keys

    # Use the longest match function to create a list of the longest match for each subsequence
    longest_match_list = []

    for i in range(len(subsequences)):
        longest_match_list.append(longest_match(dna, subsequences[i]))

    # TODO: Check database for matching profiles

    # Variable to keep track of if we've found a match
    match_counter = 0

    for i in range(len(data_list)):
        sequence_match_counter = 0
        for j in range(len(subsequences)):
            # Need to covert the dictionary value to an int as it is stored as a string,
            # hence the use of int()
            if longest_match_list[j] == int(data_list[i][subsequences[j]]):
                sequence_match_counter += 1

        if sequence_match_counter == len(subsequences):
            print(data_list[i]['name'])
            match_counter += 1  # Add to match counter if we've found a match
        else:
            sequence_match_counter = 0

    if match_counter == 0:
        print('No match')

    return


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