import csv
import sys


def main():

    # ensure correct usage of command-line
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # create a list of dict
    persons = []

    # read csv containing the STR counts of individuals into memory
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)

        for person in reader:
            for key in person:
                if key != 'name':
                    person[key] = int(person[key])

            persons.append(person)

    # read txt file containing the sequences into a string
    with open(sys.argv[2], "r") as file2:
        sequence = file2.read()

    # Store STR in the list
    STR_list = reader.fieldnames[1:]

    STR_counts = []
    # Store the STR counts in the list
    for STR in STR_list:
        STR_count = longest_STR(STR, sequence)
        STR_counts.append(STR_count)

    name = []
    match = False

    # compare against data
    for person in persons:
        # convert the data value in each person into list and compare it with STR_counts
        if STR_counts == list(person.values())[1:]:
            match = True
            name = (list(person.values())[0])

    if match == True:
        print(name)
    else:
        print("No match")


# computing longest STR count
def longest_STR(target, sequence):

    # initialize the counters
    longest_count = 0
    count = 0

    # iterate over the DNA sequence
    for i in range(len(sequence)):

        # search for consecutive STR
        while target == sequence[i:i + len(target)]:
            count += 1
            i = i + len(target)

        if count > longest_count:
            longest_count = count
        else:
            count = 0

    return longest_count


if __name__ == "__main__":
    main()
