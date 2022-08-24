from cs50 import get_string

# prompt the user to input the credit number
try:
    number = get_string("Number: ")
    
except:
    print("INVALID")
    
# slice the string in reverse order by setting stripe to -1 
# Extact the digit in string and put into a list
number_list = list(number[::-1])

# break the list into 2 set of digits 
last_digit = list(number_list[0::2])
seclast_digit = list(number_list[1::2])

# checksum of the digits as string in a list
checksum = sum(int(i) for i in last_digit) + sum((int(j) * 2) // 10 + int(j) * 2 % 10 for j in seclast_digit)

# check Luhn Algorithm
if checksum % 10 != 0:
    print("INVALID")
elif checksum % 10 == 0:
    # check if american express
    if len(number) == 15 and (int(number[:2]) == 34 or int(number[:2]) == 37):
        print("AMEX")
    # check if master card
    elif len(number) == 16 and 51 <= int(number[:2]) <= 55:
        print("MASTERCARD")
    # check if VISA
    elif len(number) in [13, 16] and int(number[:1]) == 4:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
    
