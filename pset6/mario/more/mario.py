from cs50 import get_int

# Get integer of height from user input
while True:
    h = get_int("Height: ")

    if h in range(1, 9):
        break

# print the pyramid in concatenate the string

for i in range(1, h + 1):
    print(" " * (h - i) + "#" * i + "  " + "#" * i)

'''
# print the pyramid
for i in range(1, h + 1):
    # print space
    for j in range(h - i):
        print(" ", end="")
    # print hash
    for k in range(i):
        print("#", end="")
    # print gap
    for m in range(1):
        print("  ", end="")
    # print hash
    for n in range(i):
        print("#", end="")
    # print endline char
    print()
'''